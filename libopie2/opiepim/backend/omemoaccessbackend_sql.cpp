/*
                             This file is part of the Opie Project
                             Copyright (C) Stefan Eilers <eilers.stefan@epost.de>
              =.             Copyright (C) 2009 The Opie Team <opie-devel@lists.sourceforge.net>
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
 * SQL Backend for the OPIE-Notes Database.
 */

#include "omemoaccessbackend_sql.h"

/* OPIE */
#include <opie2/opimmemo.h>
#include <opie2/opimdateconversion.h>
#include <opie2/osqldriver.h>
#include <opie2/osqlresult.h>
#include <opie2/osqlmanager.h>
#include <opie2/osqlquery.h>
#include <opie2/opimsql.h>
#include <opie2/odebug.h>

#include <qpe/global.h>
#include <qpe/recordfields.h>

/* QT */
#include <qarray.h>
#include <qdatetime.h>
#include <qstringlist.h>
#include <qfile.h>


using namespace Opie;
using namespace Opie::DB;


/*
 * Implementation of used query types * CREATE query
 * LOAD query
 * INSERT
 * REMOVE
 * CLEAR
 */
namespace {
    /**
     * CreateQuery for the Todolist Table
     */
    class CreateQuery : public OSQLQuery {
    public:
        CreateQuery();
        ~CreateQuery();
        QString query()const;
    };

    /**
     * Clears (delete) a Table
     */
    class ClearQuery : public OSQLQuery {
    public:
        ClearQuery();
        ~ClearQuery();
        QString query()const;

    };


    /**
     * inserts/adds a OPimMemo to the table
     */
    class InsertQuery : public OSQLQuery {
    public:
        InsertQuery(const OPimMemo& );
        ~InsertQuery();
        QString query()const;
    private:
        OPimMemo m_memo;
    };


    // We use a single "notes" table to store all data
    CreateQuery::CreateQuery() : OSQLQuery() {}
    CreateQuery::~CreateQuery() {}
    QString CreateQuery::query()const {
        QString qu;

        qu += "create table if not exists notes( uid PRIMARY KEY ";
        qu += ",categories VARCHAR(10)";
        qu += ",text VARCHAR(10)";
        qu += " );";

        return qu;
    }

    ClearQuery::ClearQuery()
        : OSQLQuery() {}
    ClearQuery::~ClearQuery() {}
    QString ClearQuery::query()const {
        QString qu = "drop table notes;";
        return qu;
    }



    InsertQuery::InsertQuery( const OPimMemo& memo )
        : OSQLQuery(), m_memo( memo ) {
    }

    InsertQuery::~InsertQuery() {
    }

    /*
     * converts from a OPimMemo to a query
     */
    QString InsertQuery::query()const{

        QString qu;
        qu  += "insert into notes VALUES( " +
            QString::number( m_memo.uid() );

        qu += QString( ",\"%1\"" ).arg( OPimMemo::idsToString( m_memo.categories() ) );
        qu += QString( ",\"%1\"" ).arg( m_memo.text() ); // FIXME escape quotes
        qu += " );";

        
        // qu  += "commit;";
        odebug << "add " << qu << "" << oendl;
        return qu;
    }


};


/* --------------------------------------------------------------------------- */

namespace Opie {

OPimMemoAccessBackend_SQL::OPimMemoAccessBackend_SQL ( const QString& /* appname */,
                               const QString& filename ):
    OPimMemoAccessBackend(), m_changed(false), m_driver( NULL )
{
    odebug << "C'tor OPimMemoAccessBackend_SQL starts" << oendl;
    QTime t;
    t.start();

    /* Expecting to access the default filename if nothing else is set */
    if ( filename.isEmpty() ){
        m_fileName = Global::applicationFileName( "notes","notes.db" );
    } else
        m_fileName = filename;

    // Get the standart sql-driver from the OSQLManager..
    OSQLManager man;
    m_driver = man.standard();
    m_driver->setUrl( m_fileName );

    m_changeLog = new OPimChangeLog_SQL( m_driver, "changelog", "peers" );

    odebug << "C'tor OPimMemoAccessBackend_SQL ends: " << t.elapsed() << " ms" << oendl;
}

OPimMemoAccessBackend_SQL::~OPimMemoAccessBackend_SQL ()
{
    if( m_changeLog )
        delete m_changeLog;
    if( m_driver )
        delete m_driver;
}

bool OPimMemoAccessBackend_SQL::load ()
{
    if (!m_driver->open() )
        return false;

    // Don't expect that the database exists.
    // It is save here to create the table, even if it
    // do exist. ( Is that correct for all databases ?? )
    CreateQuery creat;
    OSQLResult res = m_driver->query( &creat );

    update();

    m_changeLog->init();
    
    return true;

}

bool OPimMemoAccessBackend_SQL::reload()
{
    return load();
}

bool OPimMemoAccessBackend_SQL::save()
{
    return m_driver->sync();
}


void OPimMemoAccessBackend_SQL::clear ()
{
    ClearQuery cle;
    OSQLResult res = m_driver->query( &cle );

    reload();
}

OPimChangeLog *OPimMemoAccessBackend_SQL::changeLog() const
{
    return m_changeLog;
}

bool OPimMemoAccessBackend_SQL::wasChangedExternally()
{
    return false;
}

bool OPimMemoAccessBackend_SQL::dataSourceExists() const {
    return QFile::exists( m_fileName );
}

UIDArray OPimMemoAccessBackend_SQL::allRecords() const
{

    // FIXME: Think about cute handling of changed tables..
    // Thus, we don't have to call update here...
    if ( m_changed )
        ((OPimMemoAccessBackend_SQL*)this)->update();

    return m_uids;
}

bool OPimMemoAccessBackend_SQL::add ( const OPimMemo &newmemo )
{
    odebug << "add in memo SQL-Backend" << oendl;
    InsertQuery ins( newmemo );
    OSQLResult res = m_driver->query( &ins );

    if ( res.state() == OSQLResult::Failure )
        return false;

    // Add changelog entry
    int uid = newmemo.uid();
    m_changeLog->addAddEntry( uid );

    int c = m_uids.count();
    m_uids.resize( c+1 );
    m_uids[c] = uid;

    return true;
}


bool OPimMemoAccessBackend_SQL::remove ( int uid )
{
    OPimSQLRemoveQuery rem( "notes", uid );
    OSQLResult res = m_driver->query(&rem );

    if ( res.state() == OSQLResult::Failure )
        return false;

    // Add changelog entry
    m_changeLog->addDeleteEntry( uid );

    m_changed = true;

    return true;
}

bool OPimMemoAccessBackend_SQL::replace ( const OPimMemo &memo )
{
    // Disable the changelog (since we don't want the delete and add entries)
    m_changeLog->setEnabled( false );

    // Delete the old record
    bool result = remove( memo.uid() );

    if( result ) {
        // Add the new version back in
        result = add( memo );
    }
        
    m_changeLog->setEnabled( true );
    if( result ) {
        // Add changelog entry
        m_changeLog->addUpdateEntry( memo.uid() );
    }

    return result;
}


OPimMemo OPimMemoAccessBackend_SQL::find ( int uid ) const
{
    odebug << "OPimMemoAccessBackend_SQL::find(" << uid << ")" << oendl;
    QTime t;
    t.start();

    OPimSQLFindQuery query( "notes", m_changeLog, uid );
    OSQLResult result = m_driver->query( &query );

    OSQLResultItem resItem = result.first();

    OPimMemo retMemo = readMemo( resItem );

    odebug << "OPimMemoAccessBackend_SQL::find() needed: " << t.elapsed() << " ms" << oendl;
    return retMemo;
}

OPimMemo OPimMemoAccessBackend_SQL::find( int uid, const UIDArray& queryUids, uint current, Frontend::CacheDirection direction ) const
{
    odebug << "find( " << uid << ", " << queryUids.count() << ", " << current << ", " << direction << " )" << oendl;
    odebug << "OPimMemoAccessBackend_SQL::find( ..multi.. )" << oendl;
    odebug << "searching for " << uid << "" << oendl;

    QTime t;
    t.start();

    uint numReadAhead = readAhead();
    QArray<int> searchList( numReadAhead );

    uint size =0;

    // Build an array with all elements which should be requested and cached
    // We will just request "numReadAhead" elements, starting from "current" position in
    // the list of many uids !
    switch( direction ) {
        /* forward */
    case Frontend::Forward:
        for ( uint i = current; i < queryUids.count() && size < numReadAhead; i++ ) {
            searchList[size] = queryUids[i];
            size++;
        }
        break;
        /* reverse */
    case Frontend::Reverse:
        for ( uint i = current; i != 0 && size <  numReadAhead; i-- ) {
            searchList[size] = queryUids[i];
            size++;
        }
        break;
    }

    //Shrink to real size..
    searchList.resize( size );

    OPimMemo retMemo( requestMemosAndCache( uid, searchList ) );

    odebug << "OPimMemoAccessBackend_SQL::find( ..multi.. ) needed: " << t.elapsed() << " ms" << oendl;
    return retMemo;
}


UIDArray OPimMemoAccessBackend_SQL::matchRegexp( const QRegExp &r ) const
{
    QString qu = "SELECT uid FROM notes WHERE (";
    QString searchlist;

    searchlist += " rlike(\""+ r.pattern() + "\",\"text\") ";

    qu = qu + searchlist + ")";

    odebug << "query: " << qu << "" << oendl;

    OSQLRawQuery raw( qu );
    OSQLResult res = m_driver->query( &raw );

    return extractUids( res );
}

uint OPimMemoAccessBackend_SQL::querySettings() const
{
    return OPimMemoAccess::IgnoreCase
        | OPimMemoAccess::WildCards
        ;
}

bool OPimMemoAccessBackend_SQL::hasQuerySettings (uint querySettings) const
{
    /* OPimMemoAccess::IgnoreCase may be added with any of the other settings. 
     * IgnoreCase should never used alone.
     * Wildcards, RegExp, ExactMatch should never used at the same time...
     */

    // Step 1: Check whether the given settings are supported by this backend
    if ( ( querySettings & (
                OPimMemoAccess::IgnoreCase
                | OPimMemoAccess::WildCards
//              | OPimMemoAccess::RegExp
//              | OPimMemoAccess::ExactMatch
                   ) ) != querySettings )
        return false;

    // Step 2: Check whether the given combinations are ok..

    // IngoreCase alone is invalid
    if ( querySettings == OPimMemoAccess::IgnoreCase )
        return false;

    // WildCards, RegExp and ExactMatch should never used at the same time
    switch ( querySettings & ~( OPimMemoAccess::IgnoreCase
                    )
         ){
    case OPimMemoAccess::RegExp:
        return ( true );
    case OPimMemoAccess::WildCards:
        return ( true );
    case OPimMemoAccess::ExactMatch:
        return ( true );
    case 0: // one of the upper removed bits were set..
        return ( true );
    default:
        return ( false );
    }

}

UIDArray OPimMemoAccessBackend_SQL::sorted( const UIDArray& ar, bool asc, int sortOrder,
                                  int filter, const QArray<int>& categories )const
{
    QTime t;
    t.start();

    QString query = "SELECT uid FROM notes";

    query += " WHERE (";
    for ( uint i = 0; i < ar.count(); i++ ) {
        query += " uid = " + QString::number( ar[i] ) + " OR";
    }
    query.remove( query.length()-2, 2 ); // Hmmmm..
    query += ")";


    if ( filter != OPimBase::FilterOff ){
        if ( filter & OPimBase::FilterCategory ){
            QString cat_query = "";
            for ( uint cat_nu = 0; cat_nu < categories.count(); ++cat_nu ) {
                int cat = categories[cat_nu];
                if( cat == -1 ) {
                    cat_query += " categories = '' OR";
                    break;
                }
                else if( cat != 0 ) {
                    cat_query += " categories LIKE '%" + QString::number( cat ) + "%' OR";
                    break;
                }
            }
            if( !cat_query.isEmpty() ) {
                cat_query.remove( cat_query.length()-2, 2 );
                query += "AND (" + cat_query + ")";
            }
        }
    }

    query += " ORDER BY";
    query += " \"Text\"";

    if ( !asc )
        query += " DESC";


    odebug << "sorted query is: " << query << "" << oendl;

    OSQLRawQuery raw( query );
    OSQLResult res = m_driver->query( &raw );
    if ( res.state() != OSQLResult::Success ){
        UIDArray empty;
        return empty;
    }

    UIDArray list = extractUids( res );

    odebug << "sorted needed " << t.elapsed() << " ms!" << oendl;
    return list;
}


void OPimMemoAccessBackend_SQL::update()
{
    odebug << "Update starts" << oendl;
    QTime t;
    t.start();

    // Now load the database set and extract the uid's
    // which will be held locally

    OPimSQLLoadQuery lo( "notes", m_changeLog );
    OSQLResult res = m_driver->query(&lo);
    if ( res.state() != OSQLResult::Success )
        return;

    m_uids = extractUids( res );

    m_changed = false;

    odebug << "Update ends " << t.elapsed() << " ms" << oendl;
}

UIDArray OPimMemoAccessBackend_SQL::extractUids( OSQLResult& res ) const
{
    odebug << "extractUids" << oendl;
    QTime t;
    t.start();
    OSQLResultItem::ValueList list = res.results();
    OSQLResultItem::ValueList::Iterator it;
    UIDArray ints(list.count() );
    odebug << " count = " << list.count() << "" << oendl;

    int i = 0;
    for (it = list.begin(); it != list.end(); ++it ) {
        ints[i] =  (*it).data("uid").toInt();
        i++;
    }
    odebug << "extractUids ready: count2 = " << i << " needs " << t.elapsed() << " ms" << oendl;

    return ints;

}


/* Returns memo requested by uid and fills cache with memos requested by uids in the cachelist */
OPimMemo OPimMemoAccessBackend_SQL::requestMemosAndCache( int uid, const UIDArray& uidlist )const
{
    // We want to get all memos with one query.
    // We don't have to add the given uid to the uidlist, it is expected to be there already (see opimrecordlist.h).
    // All memos will be stored in the cache, afterwards the memo with the user id "uid" will be returned
    // by using the cache..
    UIDArray cachelist = uidlist;
    OPimMemo retMemo;

    odebug << "Reqest and cache" << cachelist.size() << "elements !" << oendl;

    QTime t;
    t.start();

    int t2needed = 0;
    int t3needed = 0;
    QTime t2;
    t2.start();
    OPimSQLFindQuery query( "notes", m_changeLog, cachelist );
    OSQLResult result = m_driver->query( &query );
    t2needed = t2.elapsed();

    QTime t3;
    t3.start();
    OSQLResultItem resItem = result.first();
    do {
        OPimMemo memo = readMemo( resItem );
        odebug << "Caching uid: " << memo.uid() << oendl;
        cache( memo );
        if ( memo.uid() == uid )
            retMemo = memo;
        resItem = result.next();
    } while ( ! result.atEnd() ); //atEnd() is true if we are past(!) the list !!
    t3needed = t3.elapsed();


    // odebug << "Adding UID: " << resItem.data( "uid" ) << "" << oendl;
    odebug << "RequestMemosAndCache needed: insg.:" << t.elapsed() << " ms, query: " << t2needed
           << " ms, mapping: " << t3needed << " ms" << oendl;

    return retMemo;
}

OPimMemo OPimMemoAccessBackend_SQL::readMemo( const OSQLResultItem& resultItem ) const
{
    OPimMemo memo;

    memo.setUid( resultItem.data( "uid" ).toInt() );
    memo.setText( resultItem.data( "text" ) );
    QString categories = resultItem.data( "categories" );
    memo.setCategories( OPimMemo::idsFromString( categories ) );

    if( !m_changeLog->slowSync() ) {
        QString chgtype = resultItem.data( "chgtype" );
        if( chgtype == "D" )
            memo.setAction( OPimRecord::ACTION_REMOVE );
        else if( chgtype == "U" )
            memo.setAction( OPimRecord::ACTION_REPLACE );
    }

    return memo;
}


}
