#ifndef OPIE_PIM_RECORD_H
#define OPIE_PIM_RECORD_H

#include <qmap.h>
#include <qstring.h>
#include <qstringlist.h>

#include <qpe/palmtoprecord.h>


/**
 * This is the base class for 
 * all PIM Records
 *
 */
class OPimRecord : public Qtopia::Record {
public:
    /**
     * c'tor
     * uid of 0 isEmpty
     * uid of 1 will be assigned a new one
     */
    OPimRecord(int uid = 0);
    ~OPimRecord();

    /**
     * copy c'tor
     */
    OPimRecord( const OPimRecord& rec );

    /**
     * copy operator
     */
    OPimRecord &operator=( const OPimRecord& );

    /**
     * category names resolved
     */
    QStringList categoryNames()const;

    /**
     * set category names they will be resolved
     */
    void setCategoryNames( const QStringList& );

    /**
     * addCategoryName adds a name
     * to the internal category list
     */
    void addCategoryName( const QString& );

    /**
     * if a Record isEmpty
     * it's empty if it's 0
     */
    virtual bool isEmpty()const;

    /**
     * toRichText summary
     */
    virtual QString toRichText()const = 0;

    /**
     * a small one line summary
     */
    virtual QString toShortText()const = 0;

    /**
     * the name of the Record
     */
    virtual QString type()const = 0;

    /**
     * converts the internal structure to a map
     */
    virtual QMap<int, QString> toMap()const = 0;

    /**
     * key value representation of extra items
     */
    virtual QMap<QString, QString> toExtraMap()const = 0;

    /**
     * the name for a recordField
     */
    virtual QString recordField(int)const = 0;

    /**
     * the related apps names
     */
    QStringList relatedApps()const;

    /**
     * the realtions between an app
     */
    QArray<int> relations( const QString& app )const;

    /**
     * clear the relations for all relations
     * with app
     */
    void clearRelation( const QString& app );

    /**
     * add a relation
     */
    void addRelation( const QString& app,  int id );

    /**
     * set the relations for an app
     */
    void setRelations( const QString&, QArray<int> ids );
    
    /**
     * set the uid
     */
    virtual void setUid( int uid );

protected:
    Qtopia::UidGen &uidGen();
    QString crossToString()const;

private:
    class OPimRecordPrivate;
    OPimRecordPrivate *d;
    QMap<QString, QArray<int> > m_relations;
    static Qtopia::UidGen m_uidGen;

};



#endif
