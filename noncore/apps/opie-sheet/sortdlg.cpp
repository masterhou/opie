/*
               =.            This file is part of the Opie Project
             .=l.            Copyright (C) 2004 Opie Developer Team <opie-devel@lists.sourceforge.net>
           .>+-=
 _;:,     .>    :=|.         This program is free software; you can
.> <`_,   >  .   <=          redistribute it and/or  modify it under
:`=1 )Y*s>-.--   :           the terms of the GNU General Public
.="- .-=="i,     .._         License as published by the Free Software
 - .   .-<_>     .<>         Foundation; either version 2 of the License,
     ._= =}       :          or (at your option) any later version.
    .%`+i>       _;_.
    .i_,=:_.      -<s.       This program is distributed in the hope that
     +  .  -:.       =       it will be useful,  but WITHOUT ANY WARRANTY;
    : ..    .:,     . . .    without even the implied warranty of
    =_        +     =;=|`    MERCHANTABILITY or FITNESS FOR A
  _.=:.       :    :=>`:     PARTICULAR PURPOSE. See the GNU
..}^=.=       =       ;      Library General Public License for more
++=   -.     .`     .:       details.
 :     =  ...= . :.=-
 -.   .:....=;==+<;          You should have received a copy of the GNU
  -_. . .   )=.  =           Library General Public License along with
    --        :-=`           this library; see the file COPYING.LIB.
                             If not, write to the Free Software Foundation,
                             Inc., 59 Temple Place - Suite 330,
                             Boston, MA 02111-1307, USA.

*/

/*
 * Opie Sheet (formerly Sheet/Qt)
 * by Serdar Ozler <sozler@sitebest.com>
 */

#include "sortdlg.h"

/* QT */
#include <qlabel.h>
#include <qradiobutton.h>
#include <qmessagebox.h>

SortDialog::SortDialog(QWidget *parent)
        :QDialog(parent, 0, TRUE)
{
    // Main widget
    tabs=new QTabWidget(this);
    widgetSort=new QWidget(tabs);
    widgetOptions=new QWidget(tabs);
    tabs->addTab(widgetSort, tr("&Sort"));
    tabs->addTab(widgetOptions, tr("&Options"));

    // Sort tab
    comboFieldA=createFieldCombo(tr("&Sort by"), 10);
    groupOrderA=createOrderButtons(10);
    comboFieldB=createFieldCombo(tr("&Then by"), 90);
    groupOrderB=createOrderButtons(90);
    comboFieldC=createFieldCombo(tr("Then &by"), 170);
    groupOrderC=createOrderButtons(170);

    // Options tab
    checkCase=new QCheckBox(tr("&Case Sensitive"), widgetOptions);
    checkCase->setGeometry(10, 10, 215, 20);
    checkCase->setChecked(TRUE);

    groupDirection=new QVButtonGroup(tr("&Direction"), widgetOptions);
    groupDirection->setGeometry(10, 40, 215, 70);
    new QRadioButton(tr("&Top to bottom (rows)"), groupDirection);
    new QRadioButton(tr("&Left to right (columns)"), groupDirection);
    groupDirection->setButton(0);
    connect(groupDirection, SIGNAL(clicked(int)), this, SLOT(directionChanged(int)));

    // Main widget
    box=new QVBoxLayout(this);
    box->addWidget(tabs);

    setCaption(tr("Sort"));
}

SortDialog::~SortDialog()
{}

QComboBox *SortDialog::createFieldCombo(const QString &caption, int y)
{
    QLabel *label=new QLabel(caption, widgetSort);
    label->setGeometry(10, y+5, 215, 20);
    QComboBox *combo=new QComboBox(FALSE, widgetSort);
    combo->setGeometry(10, y+35, 105, 20);
    label->setBuddy(combo);
    return combo;
}

QVButtonGroup *SortDialog::createOrderButtons(int y)
{
    QVButtonGroup *group=new QVButtonGroup(widgetSort);
    group->setGeometry(125, y, 100, 60);
    new QRadioButton(tr("&Ascending"), group);
    new QRadioButton(tr("&Descending"), group);
    group->setButton(0);
    return group;
}

void SortDialog::directionChanged(int id)
{
    direction=id;
    fillFieldCombo(comboFieldA);
    fillFieldCombo(comboFieldB);
    fillFieldCombo(comboFieldC);
}

void SortDialog::fillFieldCombo(QComboBox *combo)
{
    combo->clear();
    if (direction)
        for (int row=row1; row<=row2; ++row)
            combo->insertItem("Row "+QString::number(row+1));
    else
        for (int col=col1; col<=col2; ++col)
            combo->insertItem("Column "+Sheet::getHeaderString(col+1));
}

int SortDialog::exec(Sheet *s)
{
    sheet=s;
    sheet->getSelection(&row1, &col1, &row2, &col2);

    direction=0;
    fillFieldCombo(comboFieldA);
    fillFieldCombo(comboFieldB);
    fillFieldCombo(comboFieldC);

    if (row1==row2 && col1==col2)
    {
        QMessageBox::warning(this, tr("Error"), tr("One cell cannot be sorted!"));
        return QDialog::Rejected;
    }
    if (QDialog::exec()==QDialog::Accepted)
    {
        QString field1S=comboFieldA->currentText(), field2S=comboFieldB->currentText(), field3S=comboFieldC->currentText();
        field1S=field1S.mid(field1S.find(' ')+1);
        field2S=field2S.mid(field2S.find(' ')+1);
        field3S=field3S.mid(field3S.find(' ')+1);
        int field1, field2, field3;
        if (direction)
        {
            field1=field1S.toInt()-1;
            field2=field2S.toInt()-1;
            field3=field3S.toInt()-1;
            int i, j;
            for (i=col2; i>=col1; --i)
                for (j=col1+1; j<=i; ++j)
                {
                    bool swap=FALSE;
                    int compareResult=compareItems(s->item(field1, j-1), s->item(field1, j), groupOrderA->id(groupOrderA->selected()), checkCase->isChecked());
                    if (compareResult>0) swap=TRUE;
                    else if (compareResult==0)
                    {
                        compareResult=compareItems(s->item(field2, j-1), s->item(field2, j), groupOrderB->id(groupOrderB->selected()), checkCase->isChecked());
                        if (compareResult>0) swap=TRUE;
                        else if (compareResult==0)
                        {
                            compareResult=compareItems(s->item(field3, j-1), s->item(field3, j), groupOrderC->id(groupOrderC->selected()), checkCase->isChecked());
                            if (compareResult>0) swap=TRUE;
                        }
                    }
                    if (swap)
                        for (int row=row1; row<=row2; ++row)
                            s->swapCells(row, j-1, row, j);
                }
        }
        else
        {
            field1=Sheet::getHeaderColumn(field1S)-1;
            field2=Sheet::getHeaderColumn(field2S)-1;
            field3=Sheet::getHeaderColumn(field3S)-1;
            int i, j;
            for (i=row2; i>=row1; --i)
                for (j=row1+1; j<=i; ++j)
                {
                    bool swap=FALSE;
                    int compareResult=compareItems(s->item(j-1, field1), s->item(j, field1), groupOrderA->id(groupOrderA->selected()), checkCase->isChecked());
                    if (compareResult>0) swap=TRUE;
                    else if (compareResult==0)
                    {
                        compareResult=compareItems(s->item(j-1, field2), s->item(j, field2), groupOrderB->id(groupOrderB->selected()), checkCase->isChecked());
                        if (compareResult>0) swap=TRUE;
                        else if (compareResult==0)
                        {
                            compareResult=compareItems(s->item(j-1, field3), s->item(j, field3), groupOrderC->id(groupOrderC->selected()), checkCase->isChecked());
                            if (compareResult>0) swap=TRUE;
                        }
                    }
                    if (swap)
                        for (int col=col1; col<=col2; ++col)
                            s->swapCells(j-1, col, j, col);
                }
        }
        return QDialog::Accepted;
    }
    return QDialog::Rejected;
}

int SortDialog::compareItems(QTableItem *item1, QTableItem *item2,
                             int descending, bool caseSensitive)
{
    int result=0;
    if (item1)
    {
        if (item2) result=(caseSensitive ? item1->text().compare(item2->text()) : item1->text().upper().compare(item2->text().upper()));
        else result=-1;
    }
    else
        if (item2) result=1;
    return (descending ? -result : result);
}
