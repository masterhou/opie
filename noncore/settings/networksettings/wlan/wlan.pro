TEMPLATE        = lib
CONFIG          += qt warn_on release
#CONFIG		+= qt warn_on debug
DESTDIR         = $(OPIEDIR)/plugins/networksettings
HEADERS		= wlanimp.h infoimp.h wlanmodule.h wextensions.h
SOURCES		= wlanimp.cpp infoimp.cpp wlanmodule.cpp wextensions.cpp
INCLUDEPATH	+= $(OPIEDIR)/include ../ ../interfaces/
DEPENDPATH	+= $(OPIEDIR)/include
LIBS            += -lqpe -L../interfaces/ -linterfaces
INTERFACES	= wlan.ui info.ui
TARGET		= wlanplugin
VERSION		= 1.0.0



include ( $(OPIEDIR)/include.pro )
