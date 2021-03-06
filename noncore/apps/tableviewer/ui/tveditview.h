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

#ifndef Tveditview_H
#define Tveditview_H

#include <qwidget.h>
#include <qlist.h>
#include <qdialog.h>
#include <qmap.h>
#include "../db/common.h"

class QGrid;
class QSignalMapper;
class QCheckBox;

class TVEditView : public QDialog
{
	Q_OBJECT

public:
	TVEditView(TableState *s, DataElem *d, QWidget* parent = 0,
                const char* name = 0, WFlags fl = 0);
	~TVEditView();

    static bool openEditItemDialog(TableState *s, DataElem *d, QWidget *parent);

protected slots:
    void toggleEnabled(int);

protected:

    void setData(TableState *s, DataElem *d);

    QGrid *editDisplay;
    QList<QWidget> edits;
    QList<QCheckBox> buttons;
    QSignalMapper *toggles;

    QMap<int,int> keyIds;

	int num_edits;
};

#endif
