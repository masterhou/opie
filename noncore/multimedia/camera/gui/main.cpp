/**********************************************************************
** Copyright (C) 2003 Michael 'Mickey' Lauer.  All rights reserved.
**
** This file is part of Opie Environment.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
**********************************************************************/


#include "mainwindow.h"
#include <opie2/oapplication.h>

using namespace Opie::Core;
int main( int argc, char **argv )
{
    OApplication a( argc, argv, "Opie-Camera" );
    CameraMainWindow* w = new CameraMainWindow();
    a.showMainWidget( w );
    a.exec();
    delete w;
    return 0;
}

