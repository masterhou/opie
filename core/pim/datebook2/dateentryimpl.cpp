/*
                             This file is part of the Opie Project

                             Copyright (C) Opie Team <opie-devel@handhelds.org>
              =.
            .=l.
           .>+-=
 _;:,     .>    :=|.         This program is free software; you can
.> <`_,   >  .   <=          redistribute it and/or  modify it under
:`=1 )Y*s>-.--   :           the terms of the GNU Library General Public
.="- .-=="i,     .._         License as published by the Free Software
 - .   .-<_>     .<>         Foundation; either version 2 of the License,
     ._= =}       :          or (at your option) any later version.
    .%`+i>       _;_.
    .i_,=:_.      -<s.       This program is distributed in the hope that
     +  .  -:.       =       it will be useful,  but WITHOUT ANY WARRANTY;
    : ..    .:,     . . .    without even the implied warranty of
    =_        +     =;=|`    MERCHANTABILITY or FITNESS FOR A
  _.=:.       :    :=>`:     PARTICULAR PURPOSE. See the GNU
..}^=.=       =       ;      Library General Public License for more
++=   -.     .`     .:       details.
:     =  ...= . :.=-
 -.   .:....=;==+<;          You should have received a copy of the GNU
  -_. . .   )=.  =           Library General Public License along with
    --        :-=`           this library; see the file COPYING.LIB.
                             If not, write to the Free Software Foundation,
                             Inc., 59 Temple Place - Suite 330,
                             Boston, MA 02111-1307, USA.
*/

#include "namespace_hack.h"
#include "dateentryimpl.h"
#include "repeatentry.h"

#include <opie2/odebug.h>
#include <opie2/otimepicker.h>

#include <qpe/qpeapplication.h>
#include <qpe/categoryselect.h>
#include <qpe/datebookmonth.h>
#include <qpe/tzselect.h>

#include <qlineedit.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qmessagebox.h>

#include "onoteedit.h"

#include <stdlib.h>
#include <stdio.h>

using namespace Opie;
using namespace Opie::Datebook;

/*
 *  Constructs a DateEntry which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */

DateEntry::DateEntry( bool startOnMonday, const QDateTime &start,
                      const QDateTime &end, bool whichClock, QWidget* parent,
                      const char* name )
    : DateEntryBase( parent, name ),
      ampm( whichClock ),
      startWeekOnMonday( startOnMonday ),
      m_showStart(true)
{
    init();
    setDates(start,end);
    setFocusProxy(comboDescription);
}

bool DateEntry::eventFilter(QObject *obj, QEvent *ev )
{
    if( ev->type() == QEvent::FocusIn ){
        if( obj == comboStart ){
            timePickerStart->setHour(startTime.hour());
            timePickerStart->setMinute(startTime.minute());
            TimePickerLabel->setText( tr("Start Time" ) );
            m_showStart = true;
        }else if( obj == comboEnd ){
            timePickerStart->setHour(endTime.hour());
            timePickerStart->setMinute(endTime.minute());
            TimePickerLabel->setText( tr("End Time") );
            m_showStart = false;
        }
    } else if( ev->type() == QEvent::FocusOut ){
//    if( obj == comboEnd ){
//      QString s;
//      s.sprintf("%.2d:%.2d",endTime.hour(), endTime.minute());
//      comboEnd->setText(s);
//    }
//    else if( obj == comboStart ){
//      QString s;
//      s.sprintf("%.2d:%.2d",startTime.hour(), startTime.minute());
//      comboStart->setText(s);
//    }
    }

    return false;
}

static void addOrPick( QComboBox* combo, const QString& t )
{
    // Pick an  item if one excists
    for (int i=0; i<combo->count(); i++) {
        if ( combo->text(i) == t ) {
            combo->setCurrentItem(i);
            return;
        }
    }

    // Else add one
    combo->insertItem(t);
    combo->setCurrentItem(combo->count()-1);
}

DateEntry::DateEntry( bool startOnMonday, const OPimEvent &event, bool whichClock,
                      QWidget* parent,  const char* name )
    : DateEntryBase( parent, name ),
      ampm( whichClock ),
      startWeekOnMonday( startOnMonday ),
      m_showStart(true)

{
    init();
    setDates(event.startDateTime(), event.endDateTime());
    comboCategory->setCategories( event.categories(), "Calendar", tr("Calendar") );
    if(!event.description().isEmpty())
        addOrPick( comboDescription, event.description() );
    if(!event.location().isEmpty())
        addOrPick( comboLocation, event.location() );
    checkAlarm->setChecked( event.hasNotifiers() );
    checkAllDay->setChecked( event.isAllDay() );
    if(!event.note().isEmpty()) 
        noteStr=event.note();
    else 
        noteStr="";
//X    spinAlarm->setValue(event.alarmTime());
//X    if ( event.alarmSound() != OPimEvent::Silent )
//X        comboSound->setCurrentItem( 1 );
    if ( event.hasRecurrence() ) {
        rp = event.recurrence();
        cmdRepeat->setText( tr("Repeat...") );
    }
    setRepeatLabel();
//X    exceptions = event.exceptionsStr();
}

void DateEntry::setDates( const QDateTime& s, const QDateTime& e )
{
    startDate = s.date();
    endDate = e.date();
    startTime = s.time();
    endTime = e.time();

    startDateChanged( s.date().year(), s.date().month(), s.date().day() );
    endDateChanged( e.date().year(), e.date().month(), e.date().day() );

    updateTimeEdit(true,true);
}

void DateEntry::updateTimeEdit(bool s, bool e) {

    // Comboboxes
    QString strStart, strEnd;
    int shour, ehour;
    if ( ampm ) {
        shour = startTime.hour();
        ehour = endTime.hour();
        if ( shour >= 12 ) {
            if ( shour > 12 )
                shour -= 12;
            strStart.sprintf( "%d:%02d PM", shour, startTime.minute() );
        } else {
            if ( shour == 0 )
                shour = 12;
            strStart.sprintf( "%d:%02d AM", shour, startTime.minute() );
        }
        if ( ehour == 24 && endTime.minute() == 0 ) {
            strEnd  = "11:59 PM"; // or "midnight"
        } else if ( ehour >= 12 ) {
            if ( ehour > 12 )
                ehour -= 12;
            strEnd.sprintf( "%d:%02d PM", ehour, endTime.minute() );
        } else {
            if ( ehour == 0 )
                ehour = 12;
            strEnd.sprintf( "%d:%02d AM", ehour, endTime.minute() );
        }
    } else {
        strStart.sprintf( "%02d:%02d", startTime.hour(), startTime.minute() );
        strEnd.sprintf( "%02d:%02d", endTime.hour(), endTime.minute() );
    }

    if (s) comboStart->setText(strStart);
    if (e) comboEnd->setText(strEnd);
}

void DateEntry::init()
{
    comboDescription->setInsertionPolicy(QComboBox::AtCurrent);
    comboLocation->setInsertionPolicy(QComboBox::AtCurrent);

    initCombos();
    QPopupMenu *m1 = new QPopupMenu( this );
    startPicker = new DateBookMonth( m1, 0, TRUE );
    m1->insertItem( startPicker );
    buttonStart->setPopup( m1 );
    connect( startPicker, SIGNAL( dateClicked(int,int,int) ),
             this, SLOT( startDateChanged(int,int,int) ) );

    //Let start button change both start and end dates
    connect( startPicker, SIGNAL( dateClicked(int,int,int) ),
             this, SLOT( endDateChanged(int,int,int) ) );
    connect( qApp, SIGNAL( clockChanged(bool) ),
             this, SLOT( slotChangeClock(bool) ) );
    connect( qApp, SIGNAL(weekChanged(bool)),
             this, SLOT(slotChangeStartOfWeek(bool)) );

    connect( editNote, SIGNAL(clicked()),
             this, SLOT(slotEditNote()) );

    QPopupMenu *m2 = new QPopupMenu( this );
    endPicker = new DateBookMonth( m2, 0, TRUE );
    m2->insertItem( endPicker );
    buttonEnd->setPopup( m2 );
    connect( endPicker, SIGNAL( dateClicked(int,int,int) ),
             this, SLOT( endDateChanged(int,int,int) ) );

    connect(timePickerStart, SIGNAL( timeChanged(const QTime&) ),
            this, SLOT( startTimePicked(const QTime&) ));
    // install eventFilters
    comboEnd->installEventFilter( this );
    comboStart->installEventFilter( this );
}

/*
 *  Destroys the object and frees any allocated resources
 */
DateEntry::~DateEntry()
{
    // no need to delete child widgets, Qt does it all for us
        //cout << "Del: " << comboStart->currentText() << endl;
}

/*
 * public slot
 */

void DateEntry::slotEditNote() {
    QString s;
    s = "<B>" + TimeString::longDateString( startDate ) + "</B>";
//    s.sprintf("<B>%d/%d</B> ", startDate.day(), startDate.month());
    NoteEntry noteDlg(s+comboDescription->currentText(), noteStr,
                    this,0,TRUE);

    if ( QPEApplication::execDialog( &noteDlg ) ) {
        noteStr=noteDlg.note->text();
    }
}

void DateEntry::endDateChanged( int y, int m, int d )
{
    endDate.setYMD( y, m, d );
    if ( endDate < startDate ) {
        endDate = startDate;
    }

    buttonEnd->setText( TimeString::shortDate( endDate ) );

    endPicker->setDate( endDate.year(), endDate.month(), endDate.day() );
}

static QTime parseTime( const QString& s, bool ampm )
{
    QTime tmpTime;
    QStringList l = QStringList::split( ':', s );
    int hour = l[0].toInt();
    if ( ampm ) {
        int i=0;
        while (i<int(l[1].length()) && l[1][i]>='0' && l[1][i]<='9')
            i++;
        QString digits = l[1].left(i);
        if ( l[1].contains( "PM", FALSE ) ) {
            if ( hour != 12 )
                hour += 12;
        } else {
            if ( hour == 12 )
                hour = 0;
        }
        l[1] = digits;
    }
    int minute = l[1].toInt();
    if ( minute > 59 )
        minute = 59;
    else if ( minute < 0 )
        minute = 0;
    if ( hour > 23 ) {
        hour = 23;
        minute = 59;
    } else if ( hour < 0 )
        hour = 0;
    tmpTime.setHMS( hour, minute, 0 );
    return tmpTime;
}

/*
 * public slot
 */
void DateEntry::endTimeChanged( const QString &s )
{
  endTimeChanged( parseTime(s,ampm) );
}

void DateEntry::endTimeChanged( const QTime &t ) {
    if ( endDate > startDate || t >= startTime ) {
        endTime = t;
    } else {
        endTime = startTime;
        //comboEnd->setCurrentItem( comboStart->currentItem() );
    }
    timePickerStart->setHour(endTime.hour());
    timePickerStart->setMinute(endTime.minute());
}

/*
 * public slot
 */
void DateEntry::startDateChanged( int y, int m, int d )
{
    QDate prev = startDate;
    startDate.setYMD( y, m, d );
    if ( rp.type() == OPimRecurrence::Weekly &&
         startDate.dayOfWeek() != prev.dayOfWeek() ) {
        // if we change the start of a weekly repeating event
        // set the repeating day appropriately
        char mask = 1 << (prev.dayOfWeek()-1);
        int days = rp.days();
        days &= (~mask);
        days |= 1 << (startDate.dayOfWeek()-1);
        rp.setDays(days);
    }

    buttonStart->setText( TimeString::shortDate( startDate ) );

    // our pickers must be reset...
    startPicker->setDate( y, m, d );
    endPicker->setDate( y, m, d );
}

/*
 * public slot
 */
void DateEntry::startTimeEdited( const QString &s )
{
    startTimeChanged(parseTime(s,ampm));
    updateTimeEdit(false,true);
    timePickerStart->setHour(startTime.hour());
    timePickerStart->setMinute(startTime.minute());
}

void DateEntry::startTimeChanged( const QTime &t )
{
    int duration = startTime.secsTo(endTime);
    startTime = t;
    endTime = t.addSecs(duration);
}

void DateEntry::startTimePicked( const QTime &t ) {
    if(m_showStart) {
        startTimeChanged(t);
        updateTimeEdit(true,true);
    } 
    else {
        endTimeChanged(t);
        updateTimeEdit(false, true );
    }
}

/*
 * public slot
 */
void DateEntry::typeChanged( const QString &s )
{
    bool b = s != "All Day";
    buttonStart->setEnabled( b );
    comboStart->setEnabled( b );
    comboEnd->setEnabled( b );
}

void DateEntry::slotRepeat()
{
    // Work around for compiler Bug..
    RepeatEntry *e;

    // it is better in my opinion to just grab this from the mother,
    // since, this dialog doesn't need to keep track of it...
    if ( rp.type() != OPimRecurrence::NoRepeat )
        e = new RepeatEntry( startWeekOnMonday, rp, startDate, exceptions, this);
    else
        e = new RepeatEntry( startWeekOnMonday, startDate, this );

    if ( QPEApplication::execDialog( e ) ) {
        rp = e->recurrence();
        setRepeatLabel();
        exceptions = e->getExceptions();
    }
    // deleting sounds like a nice idea...
    delete e;
}

void DateEntry::slotChangeStartOfWeek( bool onMonday )
{
    startWeekOnMonday = onMonday;
}

OPimEvent DateEntry::event()
{
    OPimEvent ev;
//X    OPimEvent::SoundTypeChoice st;
    ev.setDescription( comboDescription->currentText() );
    ev.setLocation( comboLocation->currentText() );
    ev.setCategories( comboCategory->currentCategories() );
    if ( startDate > endDate ) {
        QDate tmp = endDate;
        endDate = startDate;
        startDate = tmp;
    }

    // This is now done in the changed slots
    //    startTime = parseTime( comboStart->text(), ampm );
    //endTime = parseTime( comboEnd->text(), ampm );

    if ( startTime > endTime && endDate == startDate ) {
        QTime tmp = endTime;
        endTime = startTime;
        startTime = tmp;
    }
    // don't set the time if theres no need too
    if ( ev.isAllDay() ) {
        startTime.setHMS( 0, 0, 0 );
        endTime.setHMS( 23, 59, 59 );
    }

    // adjust start and end times based on timezone
    QDateTime start( startDate, startTime );
    QDateTime end( endDate, endTime );
    time_t start_utc, end_utc;

//        odebug << "tz: " << timezone->currentZone() << oendl;

    // get real timezone
    QString realTZ;
    realTZ = QString::fromLocal8Bit( getenv("TZ") );

    // set timezone
    if ( setenv( "TZ", timezone->currentZone(), true ) != 0 )
        owarn << "There was a problem setting the timezone." << oendl;

    // convert to UTC based on selected TZ (calling tzset internally)
    start_utc = TimeConversion::toUTC( start );
    end_utc = TimeConversion::toUTC( end );

    // done playing around... put it all back
    unsetenv( "TZ" );
    if ( !realTZ.isNull() )
        if ( setenv( "TZ", realTZ, true ) != 0 )
            owarn << "There was a problem setting the timezone." << oendl;

    // convert UTC to local time (calling tzset internally)
    ev.setStartDateTime( TimeConversion::fromUTC( start_utc ) );
    ev.setEndDateTime( TimeConversion::fromUTC( end_utc ) );

    if(checkAllDay->isChecked())
        ev.setAllDay(true);

    // we only have one type of sound at the moment... LOUD!!!
//X    if ( comboSound->currentItem() != 0 )
//X        st = OPimEvent::Loud;
//X    else
//X        st = OPimEvent::Silent;
//X    ev.setAlarm( checkAlarm->isChecked(), spinAlarm->value(), st );
    if ( rp.type() != OPimRecurrence::NoRepeat )
        ev.setRecurrence( rp );
    ev.setNote( noteStr );

                //cout << "Start: " << comboStart->currentText() << endl;

//X    ev.setExceptionsStr(exceptions);

    return ev;
}

void DateEntry::setRepeatLabel()
{
    switch( rp.type() ) {
        case OPimRecurrence::Daily:
            cmdRepeat->setText( tr("Daily...") );
            break;
        case OPimRecurrence::Weekly:
            cmdRepeat->setText( tr("Weekly...") );
            break;
        case OPimRecurrence::MonthlyDay:
        case OPimRecurrence::MonthlyDate:
            cmdRepeat->setText( tr("Monthly...") );
            break;
        case OPimRecurrence::Yearly:
            cmdRepeat->setText( tr("Yearly...") );
            break;
        default:
            cmdRepeat->setText( tr("No Repeat...") );
    }
}

void DateEntry::setAlarmEnabled( bool alarmPreset, int presetTime/*X, OPimEvent::SoundTypeChoice sound */)
{
    checkAlarm->setChecked( alarmPreset );
    spinAlarm->setValue( presetTime );
/*    if ( sound != OPimEvent::Silent )
        comboSound->setCurrentItem( 1 );
    else
        comboSound->setCurrentItem( 0 );*/
}

void DateEntry::initCombos()
{
    /*
    comboStart->clear();
    comboEnd->clear();
    if ( ampm ) {
        for ( int i = 0; i < 24; i++ ) {
            if ( i == 0 ) {
                comboStart->insertItem( "12:00 AM" );
                comboStart->insertItem( "12:30 AM" );
                comboEnd->insertItem( "12:00 AM" );
                comboEnd->insertItem( "12:30 AM" );
            } else if ( i == 12 ) {
                comboStart->insertItem( "12:00 PM" );
                comboStart->insertItem( "12:30 PM" );
                comboEnd->insertItem( "12:00 PM" );
                comboEnd->insertItem( "12:30 PM" );
            } else if ( i > 12 ) {
                comboStart->insertItem( QString::number( i - 12 ) + ":00 PM" );
                comboStart->insertItem( QString::number( i - 12 ) + ":30 PM" );
                comboEnd->insertItem( QString::number( i - 12 ) + ":00 PM" );
                comboEnd->insertItem( QString::number( i - 12 ) + ":30 PM" );
            } else {
                comboStart->insertItem( QString::number( i) + ":00 AM" );
                comboStart->insertItem( QString::number( i ) + ":30 AM" );
                comboEnd->insertItem( QString::number( i ) + ":00 AM" );
                comboEnd->insertItem( QString::number( i ) + ":30 AM" );
            }
        }
    } else {
        for ( int i = 0; i < 24; i++ ) {
            if ( i < 10 ) {
                comboStart->insertItem( QString("0")
                                        + QString::number(i) + ":00" );
                comboStart->insertItem( QString("0")
                                        + QString::number(i) + ":30" );
                comboEnd->insertItem( QString("0")
                                      + QString::number(i) + ":00" );
                comboEnd->insertItem( QString("0")
                                      + QString::number(i) + ":30" );
            } else {
                comboStart->insertItem( QString::number(i) + ":00" );
                comboStart->insertItem( QString::number(i) + ":30" );
                comboEnd->insertItem( QString::number(i) + ":00" );
                comboEnd->insertItem( QString::number(i) + ":30" );
            }
        }
    }
    */
}

void DateEntry::slotChangeClock( bool whichClock )
{
    ampm = whichClock;
    initCombos();
    setDates( QDateTime( startDate, startTime ), QDateTime( endDate, endTime ) );
}

////////  DateEntryEditor ////////

DateEntryEditor::DateEntryEditor( MainWindow* mw, QWidget* parent )
    : Editor(mw, parent)
{
    m_parent = parent;
}

bool DateEntryEditor::showDialog( QString caption, OPimEvent& event ) {
    QDialog newDlg( m_parent, 0, TRUE );
    newDlg.setCaption( caption );
    QVBoxLayout *vb = new QVBoxLayout( &newDlg );
    QScrollView *sv = new QScrollView( &newDlg );
    sv->setResizePolicy( QScrollView::AutoOneFit );
    sv->setFrameStyle( QFrame::NoFrame );
    sv->setHScrollBarMode( QScrollView::AlwaysOff );
    vb->addWidget( sv );

    DateEntry *de = new DateEntry( weekStartOnMonday(), event, isAP(), &newDlg );
    sv->addChild( de );
    while ( QPEApplication::execDialog( &newDlg ) ) {
        event = de->event();
        QString error = checkEvent( event );
        if ( !error.isNull() ) {
            if ( QMessageBox::warning( m_parent, QObject::tr("Error!"), error, QObject::tr("Fix it"), QObject::tr("Continue"), 0, 0, 1 ) == 0 )
                continue;
        }
        return true;
    }
    return false;
}

bool DateEntryEditor::newEvent( const QDate& date ) {
    OPimEvent ev;
/*X    ev.setDescription(  str );
    // When the new gui comes in, change this...
    if(location==0) {
        if(defaultLocation.isEmpty()) {
            ev.setLocation(tr("(Unknown)"));
        } else {
            ev.setLocation( defaultLocation );
        }
    } 
    else
        ev.setLocation(location);
    ev.setCategories(defaultCategories); */
    QDateTime start = date;
    QDateTime end = date;
    start.setTime( QTime( 10, 0 ) );
    end.setTime( QTime( 12, 0 ) );
    ev.setStartDateTime( start );
    ev.setEndDateTime( end );

    if( showDialog( DateEntryBase::tr("New Event"), ev ) ) {
        ev.assignUid();
        m_event = ev;
        return true;
    }
    else
        return false;


//X    e->setAlarmEnabled( aPreset, presetTime, Event::Loud );
}

bool DateEntryEditor::newEvent( const QDateTime& start, const QDateTime& end ) {
    OPimEvent ev;
    ev.setStartDateTime( start );
    ev.setEndDateTime( end );

    if( showDialog( DateEntryBase::tr("New Event"), ev ) ) {
        ev.assignUid();
        m_event = ev;
        return true;
    }
    else
        return false;
}

bool DateEntryEditor::edit( const OPimEvent& event, bool showRec) {
    OPimEvent ev(event);
    if( showDialog( DateEntryBase::tr("Edit Event"), ev ) ) {
        m_event = ev;
        m_event.setUid( event.uid() );
        return true;
    }
    else
        return false;
}

OPimEvent DateEntryEditor::event() const {
    return m_event;
}

QString DateEntryEditor::checkEvent(const OPimEvent &e)
{
    // check if overlaps with itself
    bool checkFailed = FALSE;

    // check the next 12 repeats. should catch most problems
    QDate current_date = e.startDateTime().date();
    OPimRecurrence rp = e.recurrence();
    for(int i = 0; i < 12; i++)
    {
        QDate next;
        if (!rp.nextOcurrence(current_date.addDays(1), next)) {
            break;  // no more repeats
        }
        if(next < e.endDateTime().date()) {
            checkFailed = TRUE;
            break;
        }
        current_date = next;
    }

    if(checkFailed)
        return QObject::tr("Event duration is potentially longer\n"
            "than interval between repeats.");
    else
        return QString::null;
}