/*
                             This file is part of the Opie Project
                             Copyright (C) 2010 Paul Eggleton <bluelightning@bluelightning.org>
              =.             Copyright (C) 2010 The Opie Team <opie-devel@lists.sourceforge.net>
            .=l.
           .>+-=
 _;:,     .>    :=|.         This program is free software; you can
.> <`_,   >  .   <=          redistribute it and/or  modify it under
:`=1 )Y*s>-.--   :           the terms of the GNU Library General Public
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
#ifndef OPIE_PIM_AUTOCONVERT_H
#define OPIE_PIM_AUTOCONVERT_H

#include <opie2/opimglobal.h>
#include <opie2/opimtemplatebase.h>

#include <qdialog.h>

class QWidget;

namespace Opie {

class OPimAutoConverter {
protected:
    static OPimBase *createAccess( Pim::OPimGlobal::PimType type, Pim::OPimGlobal::DatabaseStyle dbtype );
public:
    static bool promptConvertData( Pim::OPimGlobal::PimType type, QWidget *parent, const QString &appTitle );
};
    
}

#endif
