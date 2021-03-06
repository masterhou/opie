/**********************************************************************
** Copyright (C) 2000-2002 Trolltech AS.  All rights reserved.
**
** This file is part of the Qtopia Environment.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/
#define QTOPIA_INTERNAL_LANGLIST
#include <qpe/qpedebug.h>
#include <qpe/global.h>
#include <qpe/qdawg.h>
#include <qpe/qpeapplication.h>
#include <qpe/resource.h>
#include <qpe/storage.h>
#include <qpe/applnk.h>
#include <qpe/qcopenvelope_qws.h>
#include <qpe/config.h>

#include <qlabel.h>
#include <qtimer.h>
#include <qmap.h>
#include <qdict.h>
#include <qdir.h>
#include <qmessagebox.h>
#include <qregexp.h>

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <qwindowsystem_qws.h> // for qwsServer
#include <qdatetime.h>


//#include "quickexec_p.h"

class Emitter : public QObject {
    Q_OBJECT
public:
    Emitter( QWidget* receiver, const QString& document )
    {
    connect(this, SIGNAL(setDocument(const QString&)),
        receiver, SLOT(setDocument(const QString&)));
    emit setDocument(document);
    disconnect(this, SIGNAL(setDocument(const QString&)),
           receiver, SLOT(setDocument(const QString&)));
    }

signals:
    void setDocument(const QString&);
};


class StartingAppList : public QObject {
    Q_OBJECT
public:
    static void add( const QString& name );
    static bool isStarting( const QString name );
private slots:
    void handleNewChannel( const QString &);
private:
    StartingAppList( QObject *parent=0, const char* name=0 ) ;

    QDict<QTime> dict;
    static StartingAppList *appl;
};

StartingAppList* StartingAppList::appl = 0;

StartingAppList::StartingAppList( QObject *parent, const char* name )
    :QObject( parent, name )
{
#if QT_VERSION >= 232 && defined(QWS)
    connect( qwsServer, SIGNAL( newChannel(const QString&)),
         this, SLOT( handleNewChannel(const QString&)) );
#endif
    dict.setAutoDelete( true );
}

void StartingAppList::add( const QString& name )
{
#if QT_VERSION >= 232  && !defined(QT_NO_COP)
    if ( !appl )
    appl = new StartingAppList;
    QTime *t = new QTime;
    t->start();
    appl->dict.insert( "QPE/Application/" + name, t );
#endif
}

bool StartingAppList::isStarting( const QString name )
{
#if QT_VERSION >= 232  && !defined(QT_NO_COP)
    if ( appl ) {
    QTime *t  = appl->dict.find( "QPE/Application/" + name );
    if ( !t )
        return false;
    if ( t->elapsed() > 10000 ) {
        // timeout in case of crash or something
        appl->dict.remove( "QPE/Application/" + name );
        return false;
    }
    return true;
    }
#endif
    return false;
}

void StartingAppList::handleNewChannel( const QString & name )
{
#if QT_VERSION >= 232  && !defined(QT_NO_COP)
    dict.remove( name );
#endif
}

static bool docDirCreated = false;
static QDawg* fixed_dawg = 0;
static QDict<QDawg> *named_dawg = 0;

static QString qpeDir()
{
    QString dir = getenv("OPIEDIR");
    if ( dir.isEmpty() ) dir = "..";
    return dir;
}

static QString dictDir()
{
    return qpeDir() + "/etc/dict";
}

/*!
  \class Global global.h
  \brief The Global class provides application-wide global functions.

  The Global functions are grouped as follows:
  \tableofcontents

  \section1 User Interface

  The statusMessage() function provides short-duration messages to the
  user. The showInputMethod() function shows the current input method,
  and hideInputMethod() hides the input method.

  \section1 Document related

  The findDocuments() function creates a set of \link doclnk.html
  DocLnk\endlink objects in a particular folder.

  \section1 Filesystem related

  Global provides an applicationFileName() function that returns the
  full path of an application-specific file.

  The execute() function runs an application.

  \section1 Word list related

  A list of words relevant to the current locale is maintained by the
  system. The list is held in a \link qdawg.html DAWG\endlink
  (implemented by the QDawg class). This list is used, for example, by
  the pickboard input method.

  The global QDawg is returned by fixedDawg(); this cannot be updated.
  An updatable copy of the global QDawg is returned by addedDawg().
  Applications may have their own word lists stored in \l{QDawg}s
  which are returned by dawg(). Use addWords() to add words to the
  updateable copy of the global QDawg or to named application
  \l{QDawg}s.

  \section1 Quoting

  The shellQuote() function quotes a string suitable for passing to a
  shell. The stringQuote() function backslash escapes '\' and '"'
  characters.

  \section1 Hardware

  The implementation of the writeHWClock() function depends on the AlarmServer
  implementation. If the AlarmServer is using atd the clock will be synced to
  hardware. If opie-alarm is used the hardware clock will be synced before
  suspending the device. opie-alarm is used by iPAQ and Zaurii implementation

  \ingroup qtopiaemb
*/

/*!
  \internal
*/
Global::Global()
{
}

/*!
  Returns the unchangeable QDawg that contains general
  words for the current locale.

  \sa addedDawg()
*/
const QDawg& Global::fixedDawg()
{
    if ( !fixed_dawg ) {
        if ( !docDirCreated )
            createDocDir();

        fixed_dawg = new QDawg;
        QString dawgfilename = dictDir() + "/dawg";
        QString words_lang;
        QStringList langs = Global::languageList();
        for (QStringList::ConstIterator it = langs.begin(); it!=langs.end(); ++it) {
            QString lang = *it;
            words_lang = dictDir() + "/words." + lang;
            QString dawgfilename_lang = dawgfilename + "." + lang;
            if ( QFile::exists(dawgfilename_lang) ||
                 QFile::exists(words_lang) ) {
                dawgfilename = dawgfilename_lang;
                break;
            }
        }
        QFile dawgfile(dawgfilename);

        if ( !dawgfile.exists() ) {
            QString fn = dictDir() + "/words";
            if ( QFile::exists(words_lang) )
                fn = words_lang;
            QFile in(fn);
            if ( in.open(IO_ReadOnly) ) {
                fixed_dawg->createFromWords(&in);
                if (dawgfile.open(IO_WriteOnly))
                    fixed_dawg->write(&dawgfile);
                dawgfile.close();
            }
        } else
            fixed_dawg->readFile(dawgfilename);
    }

    return *fixed_dawg;
}

/*!
  Returns the changeable QDawg that contains general
  words for the current locale.

  \sa fixedDawg()
*/
const QDawg& Global::addedDawg()
{
    return dawg("local");
}

/*!
  Returns the QDawg with the given \a name.
  This is an application-specific word list.

  \a name should not contain "/".
*/
const QDawg& Global::dawg(const QString& name)
{
    createDocDir();
    if ( !named_dawg )
    named_dawg = new QDict<QDawg>;
    QDawg* r = named_dawg->find(name);
    if ( !r ) {
    r = new QDawg;
    named_dawg->insert(name,r);
    QString dawgfilename = applicationFileName("Dictionary", name ) + ".dawg";
    QFile dawgfile(dawgfilename);
    if ( dawgfile.open(IO_ReadOnly) )
        r->readFile(dawgfilename);
    }
    return *r;
}

/*!
  \overload
  Adds \a wordlist to the addedDawg().

  Note that the addition of words persists between program executions
  (they are saved in the dictionary files), so you should confirm the
  words with the user before adding them.
*/
void Global::addWords(const QStringList& wordlist)
{
    addWords("local",wordlist);
}

/*!
  \overload
  Adds \a wordlist to the addedDawg().

  Note that the addition of words persists between program executions
  (they are saved in the dictionary files), so you should confirm the
  words with the user before adding them.
*/
void Global::addWords(const QString& dictname, const QStringList& wordlist)
{
    QDawg& d = (QDawg&)dawg(dictname);
    QStringList all = d.allWords() + wordlist;
    d.createFromWords(all);

    QString dawgfilename = applicationFileName("Dictionary", dictname) + ".dawg";
    QFile dawgfile(dawgfilename);
    if ( dawgfile.open(IO_WriteOnly) ) {
    d.write(&dawgfile);
    dawgfile.close();
    }

    // #### Re-read the dawg here if we use mmap().

    // #### Signal other processes to re-read.
}


/*!
  Returns the full path for the application called \a appname, with the
  given \a filename. Returns QString::null if there was a problem creating
  the directory tree for \a appname.
  If \a filename contains "/", it is the caller's responsibility to
  ensure that those directories exist.
*/
QString Global::applicationFileName(const QString& appname, const QString& filename)
{
    QDir d;
    QString r = getenv("HOME");
    r += "/Applications/";
    if ( !QFile::exists( r ) )
    if ( d.mkdir(r) == false )
        return QString::null;
    r += appname;
    if ( !QFile::exists( r ) )
    if ( d.mkdir(r) == false )
        return QString::null;
    r += "/"; r += filename;
    return r;
}

/*!
  \internal
*/
void Global::createDocDir()
{
    if ( !docDirCreated ) {
    docDirCreated = true;
    mkdir( QPEApplication::documentDir().latin1(), 0755 );
    }
}


/*!
  Displays a status \a message to the user. This usually appears
  in the taskbar for a short amount of time, then disappears.
*/
void Global::statusMessage(const QString& message)
{
#if !defined(QT_NO_COP)
    QCopEnvelope e( "QPE/TaskBar", "message(QString)" );
    e << message;
#endif
}

/*!
  \internal
*/
void Global::applyStyle()
{
#if !defined(QT_NO_COP)
    QCopChannel::send( "QPE/System", "applyStyle()" );
#else
    ((QPEApplication *)qApp)->applyStyle(); // apply without needing QCop for floppy version
#endif
}

/*!
  \internal
*/
QWidget *Global::shutdown( bool )
{
#if !defined(QT_NO_COP)
    QCopChannel::send( "QPE/System", "shutdown()" );
#endif
    return 0;
}

/*!
  \internal
*/
QWidget *Global::restart( bool )
{
#if !defined(QT_NO_COP)
    QCopChannel::send( "QPE/System", "restart()" );
#endif
    return 0;
}

/*!
  Explicitly show the current input method.

  Input methods are indicated in the taskbar by a small icon. If the
  input method is activated (shown) then it takes up some proportion
  of the bottom of the screen, to allow the user to interact (input
  characters) with it.

  \sa hideInputMethod()
*/
void Global::showInputMethod()
{
#if !defined(QT_NO_COP)
    QCopChannel::send( "QPE/TaskBar", "showInputMethod()" );
#endif
}

/*!
  Explicitly hide the current input method.

  The current input method is still indicated in the taskbar, but no
  longer takes up screen space, and can no longer be interacted with.

  \sa showInputMethod()
*/
void Global::hideInputMethod()
{
#if !defined(QT_NO_COP)
    QCopChannel::send( "QPE/TaskBar", "hideInputMethod()" );
#endif
}


/*!
  \internal
*/
bool Global::isBuiltinCommand( const QString &name )
{
    if(!builtin)
    return false; // yes, it can happen
    for (int i = 0; builtin[i].file; i++) {
    if ( builtin[i].file == name ) {
        return true;
    }
    }
    return false;
}

Global::Command* Global::builtin=0;
QGuardedPtr<QWidget> *Global::running=0;

/*!
  \class Global::Command
  \brief The Global::Command class is internal.
  \internal
*/

/*!
  \internal
*/
void Global::setBuiltinCommands( Command* list )
{
    if ( running )
    delete [] running;

    builtin = list;
    int count = 0;
    if (!builtin)
    return;
    while ( builtin[count].file )
    count++;

    running = new QGuardedPtr<QWidget> [ count ];
}

/*!
  \internal
*/
void Global::setDocument( QWidget* receiver, const QString& document )
{
    Emitter emitter(receiver,document);
}

/*!
  \internal
*/
bool Global::terminateBuiltin( const QString& n )
{
    if (!builtin)
    return false;
    for (int i = 0; builtin[i].file; i++) {
    if ( builtin[i].file == n ) {
        delete running[i];
        return true;
    }
    }
    return false;
}

/*!
  \internal
*/
void Global::terminate( const AppLnk* app )
{
    //if ( terminateBuiltin(app->exec()) ) return; // maybe? haven't tried this

#ifndef QT_NO_COP
    QCString channel = "QPE/Application/" + app->exec().utf8();
    if ( QCopChannel::isRegistered(channel) ) {
    QCopEnvelope e(channel, "quit()");
    }
#endif
}

/*!
  Low-level function to run command \a c.

  \warning Do not use this function. Use execute instead.

  \sa execute()
*/
void Global::invoke(const QString &c)
{
    // Convert the command line in to a list of arguments
    QStringList list = QStringList::split(QRegExp("  *"),c);

#if !defined(QT_NO_COP)
    QString ap=list[0];
    // see if the application is already running
    // XXX should lock file /tmp/qcop-msg-ap
    if ( QCopChannel::isRegistered( ("QPE/Application/" + ap).latin1() ) ) {
        // If the channel is already register, the app is already running, so show it.
        {
            QCopEnvelope env( ("QPE/Application/" + ap).latin1(), "raise()" );
	}

        //QCopEnvelope e("QPE/System", "notBusy(QString)" );
        //e << ap;
        return;
    }
    // XXX should unlock file /tmp/qcop-msg-ap
    //see if it is being started
    if ( StartingAppList::isStarting( ap ) ) {
        // FIXME take it out for now, since it leads to a much to short showing of wait if
        // some entry is clicked.
        // Real cause is that ::execute is called twice for document tab. But it would need some larger changes
        // to fix that, and with future syncs with qtopia 1.6 it will change anyway big time since somebody there
        // had the idea that an apploader belongs to the launcher ...
        //QCopEnvelope e("QPE/System", "notBusy(QString)" );
        //e << ap;
        return;
    }

#endif

#ifdef QT_NO_QWS_MULTIPROCESS
    QMessageBox::warning( 0, "Error", "Could not find the application " + c, "Ok", 0, 0, 0, 1 );
#else

    QStrList slist;
    unsigned int j;
    for ( j = 0; j < list.count(); j++ )
        slist.append( list[j].utf8() );

    const char **args = new const char *[slist.count() + 1];
    for ( j = 0; j < slist.count(); j++ )
        args[j] = slist.at(j);

    args[j] = NULL;

#if !defined(QT_NO_COP)
    // an attempt to show a wait...
    // more logic should be used, but this will be fine for the moment...
    QCopEnvelope ( "QPE/System", "busy()" );
#endif

#ifdef HAVE_QUICKEXEC
#ifdef Q_OS_MACX
    QString libexe = qpeDir()+"/binlib/lib"+args[0] + ".dylib";
#else
    QString libexe = qpeDir()+"/binlib/lib"+args[0] + ".so";
#endif
    qDebug("libfile = %s", libexe.latin1() );
    if ( QFile::exists( libexe ) ) {
        qDebug("calling quickexec %s", libexe.latin1() );
        quickexecv( libexe.utf8().data(), (const char **)args );
    } else
#endif
    {
        bool success = false;
        int pfd [2];
        if ( ::pipe ( pfd ) < 0 )
            pfd [0] = pfd [1] = -1;

        pid_t pid = ::fork ( );

        if ( pid == 0 ) { // child
            for ( int fd = 3; fd < 100; fd++ ) {
                if ( fd != pfd [1] )
                    ::close ( fd );
            }
            ::setpgid ( ::getpid ( ), ::getppid ( ));

            // Closing of fd[1] indicates that the execvp succeeded!
            if ( pfd [1] >= 0 )
                ::fcntl ( pfd [1], F_SETFD, FD_CLOEXEC );

            // Try bindir first, so that foo/bar works too
            ::execv ( qpeDir ( ) + "/bin/" + args [0], (char * const *) args );
            ::execvp ( args [0], (char * const *) args );

            char resultByte = 1;
            if ( pfd [1] >= 0 )
                ::write ( pfd [1], &resultByte, 1 );
            ::_exit ( -1 );
        }
        else if ( pid > 0 ) {
            success = true;

            if ( pfd [1] >= 0 )
                ::close ( pfd [1] );
            if ( pfd [0] >= 0 ) {
                while ( true ) {
                    char resultByte;
                    int n = ::read ( pfd [0], &resultByte, 1 );
                    if ( n == 1 ) {
                        success = false;
                        break;
                    }
                    if (( n == -1 ) && (( errno == ECHILD ) || ( errno == EINTR )))
                        continue;

                    break; // success
                }
                ::close ( pfd [0] );
            }
        }
        if ( success )
            StartingAppList::add( list[0] );
        else
            QMessageBox::warning( 0, "Error", "Could not start the application " + c, "Ok", 0, 0, 0, 1 );
    }
    delete [] args;
#endif //QT_NO_QWS_MULTIPROCESS
}


/*!
  Executes the application identfied by \a c, passing \a
  document if it isn't null.

  Note that a better approach might be to send a QCop message to the
  application's QPE/Application/\e{appname} channel.
*/
void Global::execute( const QString &c, const QString& document )
{
    // ask the server to do the work
#if !defined(QT_NO_COP)
    if ( document.isNull() ) {
        QCopEnvelope e( "QPE/System", "execute(QString)" );
        e << c;
    } else {
        QCopEnvelope e( "QPE/System", "execute(QString,QString)" );
        e << c << document;
    }
#endif
    return;
}

/*!
  Returns the string \a s with the characters '\', '"', and '$' quoted
  by a preceeding '\'.

  \sa stringQuote()
*/
QString Global::shellQuote(const QString& s)
{
    QString r="\"";
    for (int i=0; i<(int)s.length(); i++) {
    char c = s[i].latin1();
    switch (c) {
        case '\\': case '"': case '$':
        r+="\\";
    }
    r += s[i];
    }
    r += "\"";
    return r;
}

/*!
  Returns the string \a s with the characters '\' and '"' quoted by a
  preceeding '\'.

  \sa shellQuote()
*/
QString Global::stringQuote(const QString& s)
{
    QString r="\"";
    for (int i=0; i<(int)s.length(); i++) {
    char c = s[i].latin1();
    switch (c) {
        case '\\': case '"':
        r+="\\";
    }
    r += s[i];
    }
    r += "\"";
    return r;
}

/*!
  Finds all documents on the system's document directories which
  match the filter \a mimefilter, and appends the resulting DocLnk
  objects to \a folder.
*/
void Global::findDocuments(DocLnkSet* folder, const QString &mimefilter)
{
    QString homedocs = QString(getenv("HOME")) + "/Documents";
    DocLnkSet d(homedocs,mimefilter);
    folder->appendFrom(d);
    /** let's do intellegint way of searching these files
     *  a) the user don't want to check mediums global
     *  b) the user wants to check but use the global options for it
     *  c) the user wants to check it but not this medium
     *  d) the user wants to check and this medium as well
     *
     *  In all cases we need to apply a different mimefilter to
     *  the medium.
     *  a) mimefilter.isEmpty() we need to apply the responding filter
     *     either the global or the one on the medium
     *
     *  b) mimefilter is set to an application we need to find out if the
     *     mimetypes are included in the mime mask of the medium
     */
    StorageInfo storage;
    const QList<FileSystem> &fs = storage.fileSystems();
    QListIterator<FileSystem> it ( fs );
    for ( ; it.current(); ++it ) {
      if ( (*it)->isRemovable() ) { // let's find out  if we should search on it
        // this is a candidate look at the cf and see if we should search on it
        QString path = (*it)->path();
        Config conf((*it)->path() + "/.opiestorage.cf", Config::File );
        conf.setGroup("main");
        if (!conf.readBoolEntry("check",true)) {
            continue;
        }
        conf.setGroup("subdirs");
        if (conf.readBoolEntry("wholemedia",true)) {
            DocLnkSet ide( path,mimefilter);
            folder->appendFrom(ide);
        } else {
            QStringList subDirs = conf.readListEntry("subdirs",':');
            if (subDirs.isEmpty()) {
                subDirs.append("Documents");
            }
            for (unsigned c = 0; c < subDirs.count();++c) {
                DocLnkSet ide( path+"/"+subDirs[c], mimefilter );
                folder->appendFrom(ide);
            }
        }
      } else if ( (*it)->disk() == "/dev/mtdblock6" || (*it)->disk() == "tmpfs" ) {
    QString path = (*it)->path() + "/Documents";
    DocLnkSet ide( path, mimefilter );
    folder->appendFrom(ide);
      }
    }
}

QStringList Global::languageList()
{
    QString lang = getenv("LANG");
    QStringList langs;
    langs.append(lang);
    int i  = lang.find(".");
    if ( i > 0 )
    lang = lang.left( i );
    i = lang.find( "_" );
    if ( i > 0 )
    langs.append(lang.left(i));
    return langs;
}

QStringList Global::helpPath()
{
    QString qpeDir = QPEApplication::qpeDir();
    QStringList path;
    QStringList langs = Global::languageList();
    for (QStringList::ConstIterator it = langs.fromLast(); it!=langs.end(); --it) {
    QString lang = *it;
    if ( !lang.isEmpty() )
        path += qpeDir + "/help/" + lang + "/html";
    }
    path += qpeDir + "/pics";
    path += qpeDir + "/help/html";
    /* we even put english into the en dir so try it as fallback as well for opie */
    path += qpeDir + "/help/en/html";
    path += qpeDir + "/docs";


    return path;
}

/*!
  \internal
  Truncate file to size specified
  \a f must be an open file
  \a size must be a positive value
 */
bool Global::truncateFile(QFile &f, int size){
    if (!f.isOpen())
      return false;

    int fileHandle = f.handle();
    if (fileHandle == -1)
	return false;

    return ::ftruncate(fileHandle, size) != -1;
}




// #if defined(Q_OS_UNIX) && defined(Q_WS_QWS)
// extern int qws_display_id;
// #endif

/*!
  /internal
  Returns the default system path for storing temporary files.
  Note: This does not it ensure that the provided directory exists
*/
QString Global::tempDir()
{
    QString result;
#ifdef Q_OS_UNIX
#ifdef Q_WS_QWS
    result = QString("/tmp/qtopia-%1/").arg(QString::number(qws_display_id));
#else
    result="/tmp/";
#endif
#else
    if (getenv("TEMP"))
	result = getenv("TEMP");
    else
	result = getenv("TMP");

    if (result[(int)result.length() - 1] != QDir::separator())
	result.append(QDir::separator());
#endif

    return result;
}

//#endif


#include "global.moc"
