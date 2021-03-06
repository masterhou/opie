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
#ifndef OPIE_DATE_BOOK_ACCESS_BACKEND_H
#define OPIE_DATE_BOOK_ACCESS_BACKEND_H

#include <qarray.h>

#include <opie2/opimaccessbackend.h>
#include <opie2/opimevent.h>

namespace Opie {
/**
 * This class is the interface to the storage of Events.
 * @see OPimAccessBackend
 *
 */
class ODateBookAccessBackend : public OPimAccessBackend<OPimEvent> {
public:
    /**
     * c'tor without parameter
     */
    ODateBookAccessBackend();
    ~ODateBookAccessBackend();

    /**
     * This method should return a list of UIDs containing
     * all repeating events. No filter should be applied
     * @return list of repeating events
     */
    virtual UIDArray rawRepeats()const = 0;

    /**
     * This mthod should return a list of UIDs containing all non
     * repeating events. No filter should be applied
     * @return list of nonrepeating events
     */
    virtual UIDArray nonRepeats() const = 0;

    /**
     * If you do not want to implement the effectiveEvents methods below
     * you need to supply it with directNonRepeats.
     * This method can return empty lists if effectiveEvents is implememted
     */
    virtual OPimEvent::ValueList directNonRepeats()const = 0;

    /**
     * Same as above but return raw repeats!
     */
    virtual OPimEvent::ValueList directRawRepeats()const = 0;

    /* is implemented by default but you can reimplement it*/
    /**
     * Effective Events are special event occuring during a time frame. This method does calcualte
     * EffectiveEvents bases on the directNonRepeats and directRawRepeats. You may implement this method
     * yourself
     */
    virtual OPimBackendOccurrence::List effectiveNonRepeatingEvents( const QDate& from, const QDate& to )const;

    /**
     * this is an overloaded member function
     * @see effectiveNonRepeatingEvents( const QDate& from, const QDate& to )
     */
    virtual OPimBackendOccurrence::List effectiveNonRepeatingEvents( const QDateTime& start )const;

    /**
     * Common and probably inefficent implementation
     * for queryByExample, sorted
     * and occurrences
     */
//@{
    /**
     *  Return all possible settings.
     *  @return All settings provided by the current backend
     * (i.e.: query_WildCards & query_IgnoreCase)
     */
    uint querySettings() const;

    /**
     * Check whether settings are correct.
     * @return <i>true</i> if the given settings are correct and possible.
     */
    bool hasQuerySettings (uint querySettings) const;

    UIDArray queryByExample( const UIDArray& uidlist, const OPimEvent&, int settings, const QDateTime& d = QDateTime() )const;
    UIDArray sorted( const UIDArray&, bool asc, int, int, const QArray<int>& )const;
    OPimBackendOccurrence::List occurrences( const QDate&, const QDate& end )const;
    OPimBackendOccurrence::List occurrences( const QDateTime& )const;
//@}

protected:
    static OPimBackendOccurrence::List filterOccurrences(const OPimBackendOccurrence::List,
                                                         const QDateTime& time );
private:
    class Private;
    Private *d;

};

}

#endif
