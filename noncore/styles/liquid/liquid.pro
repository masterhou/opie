TEMPLATE    =  lib
CONFIG       = qt embedded release warn_on
SOURCES     =  liquid.cpp \
               effects.cpp \
               liquidset.cpp \
               plugin.cpp
               
HEADERS     =  liquid.h \
               effects.h \
               liquidset.h \
               plugin.h
               
LIBS       += -lqpe -lopie
INCLUDEPATH += $(OPIEDIR)/include
DESTDIR      = $(OPIEDIR)/plugins/styles
TARGET       = liquid
VERSION      = 1.0.0

include ( $(OPIEDIR)/include.pro )
