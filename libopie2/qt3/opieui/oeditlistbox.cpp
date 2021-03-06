/* This file is part of the KDE libraries
   Copyright (C) 2000 David Faure <faure@kde.org>, Alexander Neundorf <neundorf@kde.org>
   2000, 2002 Carsten Pfeiffer <pfeiffer@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

/* QT */

#include <qstringlist.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qlistbox.h>
#include <qwhatsthis.h>
#include <qlabel.h>

/* OPIE */

#include <opie2/ocombobox.h>
#include <opie2/odialog.h>
#include <opie2/olineedit.h>
#include <opie2/oeditlistbox.h>

/* UNIX */

#include <assert.h>

/*======================================================================================
 * OEditListBoxPrivate
 *======================================================================================*/

class OEditListBoxPrivate
{
public:
    bool m_checkAtEntering;
    int buttons;
};

/*======================================================================================
 * OEditListBox
 *======================================================================================*/

OEditListBox::OEditListBox(QWidget *parent, const char *name,
			   bool checkAtEntering, int buttons )
    :QGroupBox(parent, name )
{
    init( checkAtEntering, buttons );
}

OEditListBox::OEditListBox(const QString& title, QWidget *parent,
			   const char *name, bool checkAtEntering, int buttons)
    :QGroupBox(title, parent, name )
{
    init( checkAtEntering, buttons );
}

OEditListBox::OEditListBox(const QString& title, const CustomEditor& custom,
                           QWidget *parent, const char *name,
                           bool checkAtEntering, int buttons)
    :QGroupBox(title, parent, name )
{
    m_lineEdit = custom.lineEdit();
    init( checkAtEntering, buttons, custom.representationWidget() );
}

OEditListBox::~OEditListBox()
{
    delete d;
    d=0;
}

void OEditListBox::init( bool checkAtEntering, int buttons,
                         QWidget *representationWidget )
{
    d=new OEditListBoxPrivate;
    d->m_checkAtEntering=checkAtEntering;
    d->buttons = buttons;

    int lostButtons = 0;
    if ( (buttons & Add) == 0 )
        lostButtons++;
    if ( (buttons & Remove) == 0 )
        lostButtons++;
    if ( (buttons & UpDown) == 0 )
        lostButtons += 2;


    servNewButton = servRemoveButton = servUpButton = servDownButton = 0L;
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::MinimumExpanding));

    QWidget * gb = this;
    QGridLayout * grid = new QGridLayout(gb, 7 - lostButtons, 2,
                                         ODialog::marginHint(),
                                         ODialog::spacingHint());
    grid->addRowSpacing(0, fontMetrics().lineSpacing());
    for ( int i = 1; i < 7 - lostButtons; i++ )
        grid->setRowStretch(i, 1);

    grid->setMargin(15);

    if ( representationWidget )
        representationWidget->reparent( gb, QPoint(0,0) );
    else
        m_lineEdit=new OLineEdit(gb);

    m_listBox = new QListBox(gb);

    QWidget *editingWidget = representationWidget ?
                             representationWidget : m_lineEdit;
    grid->addMultiCellWidget(editingWidget,1,1,0,1);
    grid->addMultiCellWidget(m_listBox, 2, 6 - lostButtons, 0, 0);
    int row = 2;
    if ( buttons & Add ) {
        servNewButton = new QPushButton(tr("&Add"), gb);
        servNewButton->setEnabled(false);
        connect(servNewButton, SIGNAL(clicked()), SLOT(addItem()));

        grid->addWidget(servNewButton, row++, 1);
    }

    if ( buttons & Remove ) {
        servRemoveButton = new QPushButton(tr("&Remove"), gb);
        servRemoveButton->setEnabled(false);
        connect(servRemoveButton, SIGNAL(clicked()), SLOT(removeItem()));

        grid->addWidget(servRemoveButton, row++, 1);
    }

    if ( buttons & UpDown ) {
        servUpButton = new QPushButton(tr("Move &Up"), gb);
        servUpButton->setEnabled(false);
        connect(servUpButton, SIGNAL(clicked()), SLOT(moveItemUp()));

        servDownButton = new QPushButton(tr("Move &Down"), gb);
        servDownButton->setEnabled(false);
        connect(servDownButton, SIGNAL(clicked()), SLOT(moveItemDown()));

        grid->addWidget(servUpButton, row++, 1);
        grid->addWidget(servDownButton, row++, 1);
    }

    connect(m_lineEdit,SIGNAL(textChanged(const QString&)),this,SLOT(typedSomething(const QString&)));
    m_lineEdit->setTrapReturnKey(true);
    connect(m_lineEdit,SIGNAL(returnPressed()),this,SLOT(addItem()));
    connect(m_listBox, SIGNAL(highlighted(int)), SLOT(enableMoveButtons(int)));

    // maybe supplied lineedit has some text already
    typedSomething( m_lineEdit->text() );
}

void OEditListBox::typedSomething(const QString& text)
{
    if(currentItem() >= 0) {
        if(currentText() != m_lineEdit->text())
        {
            // IMHO changeItem() shouldn't do anything with the value
            // of currentItem() ... like changing it or emitting signals ...
            // but TT disagree with me on this one (it's been that way since ages ... grrr)
            bool block = m_listBox->signalsBlocked();
            m_listBox->blockSignals( true );
            m_listBox->changeItem(text, currentItem());
            m_listBox->blockSignals( block );
            emit changed();
        }
    }

    if ( !servNewButton )
        return;

    if (!d->m_checkAtEntering)
        servNewButton->setEnabled(!text.isEmpty());
    else
    {
        if (text.isEmpty())
        {
            servNewButton->setEnabled(false);
        }
        else
        {
            #if QT_VERSION >= 0x030000
            StringComparisonMode mode = (StringComparisonMode) (ExactMatch | CaseSensitive );
            bool enable = (m_listBox->findItem( text, mode ) == 0L);
            #else
            bool enable = (m_listBox->findItem( text ) == 0L);
            #endif
            servNewButton->setEnabled( enable );
        }
    }
}

void OEditListBox::moveItemUp()
{
    if (!m_listBox->isEnabled())
    {
        //ONotifyClient::beep();
        return;
    }

    unsigned int selIndex = m_listBox->currentItem();
    if (selIndex == 0)
    {
        //ONotifyClient::beep();
        return;
    }

    QListBoxItem *selItem = m_listBox->item(selIndex);
    m_listBox->takeItem(selItem);
    m_listBox->insertItem(selItem, selIndex-1);
    m_listBox->setCurrentItem(selIndex - 1);

    emit changed();
}

void OEditListBox::moveItemDown()
{
    if (!m_listBox->isEnabled())
    {
        //ONotifyClient::beep();
        return;
    }

    unsigned int selIndex = m_listBox->currentItem();
    if (selIndex == m_listBox->count() - 1)
    {
        //ONotifyClient::beep();
        return;
    }

    QListBoxItem *selItem = m_listBox->item(selIndex);
    m_listBox->takeItem(selItem);
    m_listBox->insertItem(selItem, selIndex+1);
    m_listBox->setCurrentItem(selIndex + 1);

    emit changed();
}

void OEditListBox::addItem()
{
    // when m_checkAtEntering is true, the add-button is disabled, but this
    // slot can still be called through Key_Return/Key_Enter. So we guard
    // against this.
    if ( !servNewButton || !servNewButton->isEnabled() )
        return;

    const QString& currentTextLE=m_lineEdit->text();
    bool alreadyInList(false);
    //if we didn't check for dupes at the inserting we have to do it now
    if (!d->m_checkAtEntering)
    {
        // first check current item instead of dumb iterating the entire list
        if ( m_listBox->currentText() == currentTextLE )
            alreadyInList = true;
        else
        {
            #if QT_VERSION >= 0x030000
            StringComparisonMode mode = (StringComparisonMode) (ExactMatch | CaseSensitive );
            alreadyInList =(m_listBox->findItem(currentTextLE, mode) != 0);
            #else
            alreadyInList =(m_listBox->findItem(currentTextLE) != 0);
            #endif
        }
    }

    if ( servNewButton )
        servNewButton->setEnabled(false);

    bool block = m_lineEdit->signalsBlocked();
    m_lineEdit->blockSignals(true);
    m_lineEdit->clear();
    m_lineEdit->blockSignals(block);

    m_listBox->setSelected(currentItem(), false);

    if (!alreadyInList)
    {
        block = m_listBox->signalsBlocked();
        m_listBox->blockSignals( true );
        m_listBox->insertItem(currentTextLE);
        m_listBox->blockSignals( block );
        emit changed();
	emit added( currentTextLE );
    }
}

int OEditListBox::currentItem() const
{
    int nr = m_listBox->currentItem();
    #if QT_VERSION >= 0x030000
    if(nr >= 0 && !m_listBox->item(nr)->isSelected()) return -1;
    #else
    if(nr >= 0 && !m_listBox->isSelected(m_listBox->item(nr))) return -1;
    #endif
    return nr;
}

void OEditListBox::removeItem()
{
    int selected = m_listBox->currentItem();

    if ( selected >= 0 )
    {
	QString removedText = m_listBox->currentText();

        m_listBox->removeItem( selected );
        if ( count() > 0 )
            m_listBox->setSelected( QMIN( selected, count() - 1 ), true );

        emit changed();
	emit removed( removedText );
    }

    if ( servRemoveButton && m_listBox->currentItem() == -1 )
        servRemoveButton->setEnabled(false);
}

void OEditListBox::enableMoveButtons(int index)
{
    // Update the lineEdit when we select a different line.
    if(currentText() != m_lineEdit->text())
        m_lineEdit->setText(currentText());

    bool moveEnabled = servUpButton && servDownButton;

    if (moveEnabled )
    {
        if (m_listBox->count() <= 1)
        {
            servUpButton->setEnabled(false);
            servDownButton->setEnabled(false);
        }
        else if ((uint) index == (m_listBox->count() - 1))
        {
            servUpButton->setEnabled(true);
            servDownButton->setEnabled(false);
        }
        else if (index == 0)
        {
            servUpButton->setEnabled(false);
            servDownButton->setEnabled(true);
        }
        else
        {
            servUpButton->setEnabled(true);
            servDownButton->setEnabled(true);
        }
    }

    if ( servRemoveButton )
        servRemoveButton->setEnabled(true);
}

void OEditListBox::clear()
{
    m_lineEdit->clear();
    m_listBox->clear();
    emit changed();
}

void OEditListBox::insertStringList(const QStringList& list, int index)
{
    m_listBox->insertStringList(list,index);
}

void OEditListBox::insertStrList(const QStrList* list, int index)
{
    m_listBox->insertStrList(list,index);
}

void OEditListBox::insertStrList(const QStrList& list, int index)
{
    m_listBox->insertStrList(list,index);
}

void OEditListBox::insertStrList(const char ** list, int numStrings, int index)
{
    m_listBox->insertStrList(list,numStrings,index);
}

QStringList OEditListBox::items() const
{
    QStringList list;
    for ( uint i = 0; i < m_listBox->count(); i++ )
	list.append( m_listBox->text( i ));

    return list;
}

void OEditListBox::virtual_hook( int, void* )
{ /*BASE::virtual_hook( id, data );*/ }


/*======================================================================================
 * CustomEditor
 *======================================================================================*/

OEditListBox::CustomEditor::CustomEditor( OComboBox *combo )
{
    m_representationWidget = combo;
    assert( combo->lineEdit()->inherits( "OLineEdit" ) );
    m_lineEdit = static_cast<OLineEdit*>( combo->lineEdit() );

}
