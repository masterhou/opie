#if defined __cplusplus

#include "core/ocontactaccess.h"
#include "core/odatebookaccess.h"
#include "core/opimaccessfactory.h"
#include "core/opimaccesstemplate.h"
#include "core/opimcache.h"
#include "core/opimcontactfields.h"
#include "core/opimcontact.h"
#include "core/opimdateconversion.h"
#include "core/opimevent.h"
#include "core/opimglobal.h"
#include "core/opimmaintainer.h"
#include "core/opimnotify.h"
#include "core/opimnotifymanager.h"
#include "core/opimoccurrence.h"
#include "core/opimrecord.h"
#include "core/opimrecordlist.h"
#include "core/opimrecurrence.h"
#include "core/opimresolver.h"
#include "core/opimstate.h"
#include "core/opimtemplatebase.h"
#include "core/opimtimezone.h"
#include "core/opimtodo.h"
#include "core/opimxref.h"
#include "core/opimxrefmanager.h"
#include "core/opimxrefpartner.h"
#include "core/otodoaccess.h"
#include "backend/obackendfactory.h"
#include "backend/ocontactaccessbackend.h"
#include "backend/ocontactaccessbackend_vcard.h"
#include "backend/ocontactaccessbackend_xml.h"
#include "backend/odatebookaccessbackend.h"
#include "backend/odatebookaccessbackend_xml.h"
#include "backend/opimaccessbackend.h"
#include "backend/opimbackendoccurrence.h"
#include "backend/otodoaccessbackend.h"
#include "backend/otodoaccessvcal.h"
#include "backend/otodoaccessxml.h"
#include "backend/otodoaccesssql.h"
#include "backend/ocontactaccessbackend_sql.h"
#include "backend/odatebookaccessbackend_sql.h"
#include "ui/opimmainwindow.h"
#include "ui/opimrecurrencewidget.h"
#include "ui/todayconfigwidget.h"
#include "ui/todayplugininterface.h"
#include "private/vobject_p.h"
#include "private/opimoccurrence_p.h"
#include "private/opimsortvector.h"
#include "private/opimtodosortvector.h"


#endif
