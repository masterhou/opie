/*
                             This file is part of the Opie Project
                             Copyright (C) Stefan Eilers (Eilers.Stefan@epost.de)
              =.             Copyright (C) The Opie Team <opie-devel@lists.sourceforge.net>
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
/*
 * SQL Backend for the OPIE-Contact Database.
 */

#ifndef _OPimContactAccessBackend_SQL_
#define _OPimContactAccessBackend_SQL_

#include <opie2/ocontactaccessbackend.h>
#include <opie2/ocontactaccess.h>
#include <opie2/opimchangelog_sql.h>

#include <qlist.h>
#include <qdict.h>

/* aren't in namespace Opie yet - alwin */
namespace Opie {
namespace DB   {
class OSQLDriver;
class OSQLResult;
class OSQLResultItem;
}
}

namespace Opie {

/* the default xml implementation */
/**
 * This class is the SQL implementation of a Contact backend
 * it does implement everything available for OPimContact.
 * @see OPimAccessBackend for more information of available methods
 */
class OPimContactAccessBackend_SQL : public OPimContactAccessBackend {
public:
    OPimContactAccessBackend_SQL( const QString& appname, const QString& filename = QString::null );

    ~OPimContactAccessBackend_SQL();

    bool save();

    bool load();

    void clear();

    bool wasChangedExternally();
    bool dataSourceExists() const;

    UIDArray allRecords() const;

    OPimContact find( int uid ) const;
    OPimContact find( int uid, const UIDArray& items, uint cur, Frontend::CacheDirection ) const;

    UIDArray queryByExample( const UIDArray& uidlist, const OPimContact &query, int settings,
                             const QDateTime& d ) const;

    UIDArray matchRegexp(  const QRegExp &r ) const;

    uint querySettings() const;

    bool hasQuerySettings (uint querySettings) const;

    UIDArray sorted( const UIDArray& ar, bool asc, int sortOrder,
                     int filter, const QArray<int>& categories)const;

    bool add( const OPimContact &newcontact );

    bool replace( const OPimContact &contact );

    bool remove( int uid );
    bool reload();

    OPimChangeLog *changeLog() const;

private:
    UIDArray extractUids( Opie::DB::OSQLResult& res ) const;
    QMap<int, QString>  requestNonCustom( int uid ) const;
    QMap<QString, QString>  requestCustom( int uid ) const;
    QMap<int, QString> fillNonCustomMap( const Opie::DB::OSQLResultItem& resultItem ) const;
    OPimContact requestContactsAndCache( int uid, const QArray<int>& cachelist ) const;
    void update();

protected:
    bool m_changed;
    QString m_fileName;
    UIDArray m_uids;

    Opie::DB::OSQLDriver* m_driver;
    OPimChangeLog_SQL *m_changeLog;
};

}

#endif
