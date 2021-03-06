/**********************************************************************
 ** Copyright (C) 2000 Trolltech AS.  All rights reserved.
 **
 ** This file is part of Qtopia Environment.
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
// additions made by L.J. Potter Sun 02-17-2002 22:27:46

#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#define QTEXTEDIT_OPEN_API

//#include "fileBrowser.h"
//#include "fileSaver.h"

#include <qpe/filemanager.h>
#include <qpe/qcopenvelope_qws.h>

//#include <opie2/ofileselector.h>

#include <qmainwindow.h>
#include <qmultilineedit.h>
#include <qlist.h>
#include <qmap.h>

class QAction;
class QWidgetStack;
class QToolButton;
class QPopupMenu;
class QToolBar;
class QLineEdit;
class QAction;
class FileSelector;
class QpeEditor;
class QPopupMenu;
class QTimer;

class TextEdit : public QMainWindow
{
    Q_OBJECT

public:
    static QString appName() { return QString::fromLatin1("textedit"); }
    TextEdit( QWidget *parent = 0, const char *name = 0, WFlags f = 0 );
    ~TextEdit();

protected:
    QPopupMenu *font;
    QAction *desktopAction, *filePermAction, *nAutoSave, *undoAction, *redoAction, *deleteAction;
    bool edited;
    void openFile( const QString & );
    QCopChannel * channel;

    bool featureAutoSave;
    void closeEvent( QCloseEvent *e );
    bool savePrompt();
    void setTimer();
private slots:
    void editorChanged();
    void receive(const QCString&, const QByteArray&);
    void timerCrank();
    void doTimer(bool);
    void editPasteTimeDate();
    void doDesktop(bool);
    void doFilePerms(bool);
    void setDocument(const QString&);
    void changeFont();
    void fileNew();
    void fileOpen();
    bool save();
    bool saveAs();
    void cleanUp();

    void gotoLine();
    void doGoto();

    void editCut();
    void editCopy();
    void editPaste();
    void editFind();
    void fileDelete();

    void findNext();
    void findClose();

    void search();

    void newFile();
    void openFile( const DocLnk & );
    void showEditTools();
    void resetEditStatus();
    void checkEnableDelete();

    void zoomIn();
    void zoomOut();
    void setWordWrap(bool y);

private:
    void openDotFile(const QString &);
    void colorChanged( const QColor &c );
    void clear();
    void updateCaption();
    void setFontSize(int sz, bool round_down_not_up);
private:
    QpeEditor* editor;
    QToolBar *menu, *editBar, *searchBar;
    QPopupMenu *advancedMenu;
    QLineEdit *gotoEdit;
    QLineEdit *searchEdit;
    DocLnk *doc;
    bool openDesktop, filePerms;
    QAction *zin, *zout;
    QString currentFileName;
};

#endif
