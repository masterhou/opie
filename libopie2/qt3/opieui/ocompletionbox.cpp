/*
  This file                  Copyright (C) 2003 Michael 'Mickey' Lauer <mickey@tm.informatik.uni-frankfurt.de>
    is part of the           Copyright (C) 2000,2001 Carsten Pfeiffer <pfeiffer@kde.org>
       Opie Project          Copyright (C) 2000 Stefan Schimanski <1Stein@gmx.de>
                             Copyright (C) 2000,2001 Dawit Alemayehu <adawit@kde.org>
              =.
            .=l.             Originally part of the KDE Project
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

#include <qapplication.h>
#include <qevent.h>
#include <qstyle.h>

#include <opie2/ocompletionbox.h>

#define OListBox QListBox

class OCompletionBox::OCompletionBoxPrivate
{
public:
    QWidget *m_parent; // necessary to set the focus back
    QString cancelText;
    bool tabHandling;
    bool down_workaround;
};

OCompletionBox::OCompletionBox( QWidget *parent, const char *name )
    :OListBox( parent, name, WType_Popup )
{
    d = new OCompletionBoxPrivate;
    d->m_parent        = parent;
    d->tabHandling     = true;
    d->down_workaround = false;

    setColumnMode( 1 );
    setLineWidth( 1 );
    setFrameStyle( QFrame::Box | QFrame::Plain );

    if ( parent )
        setFocusProxy( parent );
    else
        setFocusPolicy( NoFocus );

    setVScrollBarMode( Auto );
    setHScrollBarMode( AlwaysOff );

    connect( this, SIGNAL( doubleClicked(QListBoxItem*)),
             SLOT( slotActivated(QListBoxItem*)) );

    // grmbl, just QListBox workarounds :[ Thanks Volker.
    connect( this, SIGNAL( currentChanged(QListBoxItem*)),
             SLOT( slotCurrentChanged() ));
    connect( this, SIGNAL( clicked(QListBoxItem*)),
             SLOT( slotItemClicked(QListBoxItem*)) );
}

OCompletionBox::~OCompletionBox()
{
    d->m_parent = 0L;
    delete d;
}

QStringList OCompletionBox::items() const
{
    QStringList list;
    for ( uint i = 0; i < count(); i++ ) {
        list.append( text( i ) );
    }
    return list;
}

void OCompletionBox::slotActivated( QListBoxItem *item )
{
    if ( !item )
        return;

    hide();
    emit activated( item->text() );
}

bool OCompletionBox::eventFilter( QObject *o, QEvent *e )
{
    int type = e->type();

    if ( o == d->m_parent ) {
        if ( isVisible() ) {
            if ( type == QEvent::KeyPress ) {
                QKeyEvent *ev = static_cast<QKeyEvent *>( e );
                switch ( ev->key() ) {
                    case Key_BackTab:
                        if ( d->tabHandling ) {
                            up();
                            ev->accept();
                            return true;
                        }
                        break;
                    case Key_Tab:
                        if ( d->tabHandling ) {
                            down(); // Only on TAB!!
                            ev->accept();
                            return true;
                        }
                        break;
                    case Key_Down:
                        down();
                        ev->accept();
                        return true;
                    case Key_Up:
                        up();
                        ev->accept();
                        return true;
                    case Key_Prior:
                        pageUp();
                        ev->accept();
                        return true;
                    case Key_Next:
                        pageDown();
                        ev->accept();
                        return true;
                    case Key_Escape:
                        cancelled();
                        ev->accept();
                        return true;
                    case Key_Enter:
                    case Key_Return:
                        if ( ev->state() & ShiftButton ) {
                            hide();
                            ev->accept();  // Consume the Enter event
                            return true;
                        }
                        break;
                    default:
                        break;
                }
            }
            else if ( type == QEvent::AccelOverride ) {
                // Override any acceleartors that match
                // the key sequences we use here...
                QKeyEvent *ev = static_cast<QKeyEvent *>( e );
                switch ( ev->key() ) {
                    case Key_Tab:
                    case Key_BackTab:
                    case Key_Down:
                    case Key_Up:
                    case Key_Prior:
                    case Key_Next:
                    case Key_Escape:
                    case Key_Enter:
                    case Key_Return:
                      ev->accept();
                      return true;
                      break;
                    default:
                        break;
                }
            }

            // parent loses focus or gets a click -> we hide
            else if ( type == QEvent::FocusOut || type == QEvent::Resize ||
                      type == QEvent::Close || type == QEvent::Hide ||
                      type == QEvent::Move ) {
                hide();
            }
            else if ( type == QEvent::Move )
                move( d->m_parent->mapToGlobal(QPoint(0, d->m_parent->height())));
            else if ( type == QEvent::Resize )
                resize( sizeHint() );
        }
    }

    // any mouse-click on something else than "this" makes us hide
    else if ( type == QEvent::MouseButtonPress ) {
        QMouseEvent *ev = static_cast<QMouseEvent *>( e );
        if ( !rect().contains( ev->pos() )) // this widget
            hide();
    }

    return OListBox::eventFilter( o, e );
}


void OCompletionBox::popup()
{
    if ( count() == 0 )
        hide();
    else {
        ensureCurrentVisible();
        bool block = signalsBlocked();
        blockSignals( true );
        setCurrentItem( 0 );
        blockSignals( block );
        clearSelection();
        if ( !isVisible() )
            show();
        else if ( size().height() < sizeHint().height() )
            resize( sizeHint() );
    }
}

void OCompletionBox::show()
{
    resize( sizeHint() );

    if ( d->m_parent )
    {
        //QDesktopWidget *screen = QApplication::desktop();
        QWidget *screen = QApplication::desktop();

        QPoint orig = d->m_parent->mapToGlobal( QPoint(0, d->m_parent->height()) );
            int x = orig.x();
        int y = orig.y();

        if ( x + width() > screen->width() )
            x = screen->width() - width();
        if (y + height() > screen->height() )
            y = y - height() - d->m_parent->height();

            move( x, y);
            qApp->installEventFilter( this );
    }

    // ### we shouldn't need to call this, but without this, the scrollbars
    // are pretty b0rked.
    //triggerUpdate( true );

    OListBox::show();
}

void OCompletionBox::hide()
{
    if ( d->m_parent )
        qApp->removeEventFilter( this );
    d->cancelText = QString::null;
    OListBox::hide();
}

QSize OCompletionBox::sizeHint() const
{
    int ih = itemHeight();
    int h = QMIN( 15 * ih, (int) count() * ih ) +1;
    h = QMAX( h, OListBox::minimumSizeHint().height() );

    int w = (d->m_parent) ? d->m_parent->width() : OListBox::minimumSizeHint().width();
    w = QMAX( OListBox::minimumSizeHint().width(), w );
    return QSize( w, h );
}

void OCompletionBox::down()
{
    int i = currentItem();

    if ( i == 0 && d->down_workaround ) {
        d->down_workaround = false;
        setCurrentItem( 0 );
        setSelected( 0, true );
        emit highlighted( currentText() );
    }

    else if ( i < (int) count() - 1 )
        setCurrentItem( i + 1 );
}

void OCompletionBox::up()
{
    if ( currentItem() > 0 )
        setCurrentItem( currentItem() - 1 );
}

void OCompletionBox::pageDown()
{
    int i = currentItem() + numItemsVisible();
    i = i > (int)count() - 1 ? (int)count() - 1 : i;
    setCurrentItem( i );
}

void OCompletionBox::pageUp()
{
    int i = currentItem() - numItemsVisible();
    i = i < 0 ? 0 : i;
    setCurrentItem( i );
}

void OCompletionBox::home()
{
    setCurrentItem( 0 );
}

void OCompletionBox::end()
{
    setCurrentItem( count() -1 );
}

void OCompletionBox::setTabHandling( bool enable )
{
    d->tabHandling = enable;
}

bool OCompletionBox::isTabHandling() const
{
    return d->tabHandling;
}

void OCompletionBox::setCancelledText( const QString& text )
{
    d->cancelText = text;
}

QString OCompletionBox::cancelledText() const
{
    return d->cancelText;
}

void OCompletionBox::cancelled()
{
    if ( !d->cancelText.isNull() )
        emit userCancelled( d->cancelText );
    if ( isVisible() )
        hide();
}

class OCompletionBoxItem : public QListBoxItem
{
public:
    void reuse( const QString &text ) { setText( text ); }
};


void OCompletionBox::insertItems( const QStringList& items, int index )
{
    bool block = signalsBlocked();
    blockSignals( true );
    insertStringList( items, index );
    blockSignals( block );
    d->down_workaround = true;
}

void OCompletionBox::setItems( const QStringList& items )
{
    bool block = signalsBlocked();
    blockSignals( true );

    QListBoxItem* item = firstItem();
    if ( !item ) {
        insertStringList( items );
    }
    else {
        for ( QStringList::ConstIterator it = items.begin(); it != items.end(); it++) {
            if ( item ) {
                ((OCompletionBoxItem*)item)->reuse( *it );
                item = item->next();
            }
            else {
                insertItem( new QListBoxText( *it ) );
            }
        }
        QListBoxItem* tmp = item;
        while ( (item = tmp ) ) {
            tmp = item->next();
            delete item;
        }
        triggerUpdate( false );
    }

    blockSignals( block );
    d->down_workaround = true;
}

void OCompletionBox::slotCurrentChanged()
{
    d->down_workaround = false;
}

void OCompletionBox::slotItemClicked( QListBoxItem *item )
{
    if ( item )
    {
        if ( d->down_workaround ) {
            d->down_workaround = false;
            emit highlighted( item->text() );
        }

        hide();
        emit activated( item->text() );
    }
}
