DESTDIR   = $(OPIEDIR)/bin
TEMPLATE  = app
#CONFIG   = qt warn_on debug
CONFIG    = qt warn_on release
HEADERS   = mainwindowimp.h addconnectionimp.h defaultmodule.h  module.h
SOURCES   = main.cpp mainwindowimp.cpp addconnectionimp.cpp
INCLUDEPATH += $(OPIEDIR)/include interfaces/
DEPENDPATH  += $(OPIEDIR)/include interfaces/ wlan/
LIBS        += -lqpe -L$(OPIEDIR)/plugins/networksettings -Linterfaces/ -linterfaces -lopie
INTERFACES = mainwindow.ui addconnection.ui
TARGET    = networksettings



include ( $(OPIEDIR)/include.pro )
