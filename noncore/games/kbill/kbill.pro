TEMPLATE 	= app
CONFIG		= qt warn_on 
HEADERS 	= kbill.h field.h Bucket.h Cable.h Computer.h Game.h Horde.h \
Library.h MCursor.h Monster.h Network.h Picture.h Spark.h Strings.h \
UI.h  objects.h inputbox.h
SOURCES		= field.cpp Bucket.cc Cable.cc Computer.cc Game.cc Horde.cc \
Library.cc MCursor.cc Monster.cc  Network.cc Picture.cc \ 
Spark.cc UI.cpp inputbox.cpp kbill.cpp
INTERFACES = helpdialog.ui
TARGET 		= kbill
INCLUDEPATH	+= $(OPIEDIR)/include
DEPENDPATH	+= $(OPIEDIR)/include
LIBS		+= -lqpe -lopiecore2
DESTDIR 	=  $(OPIEDIR)/bin

include( $(OPIEDIR)/include.pro )
