TEMPLATE  = lib
CONFIG    += qt warn_on release
HEADERS =   irda.h irdaappletimpl.h
SOURCES =   irda.cpp irdaappletimpl.cpp
TARGET    = irdaapplet
DESTDIR   = $(OPIEDIR)/plugins/applets
INCLUDEPATH += $(OPIEDIR)/include
DEPENDPATH      += ../$(OPIEDIR)/include
LIBS            += -lqpe
VERSION   = 1.0.0



TRANSLATIONS = ../../../i18n/de/libirdaapplet.ts
TRANSLATIONS += ../../../i18n/en/libirdaapplet.ts
TRANSLATIONS += ../../../i18n/es/libirdaapplet.ts
TRANSLATIONS += ../../../i18n/fr/libirdaapplet.ts
TRANSLATIONS += ../../../i18n/hu/libirdaapplet.ts
TRANSLATIONS += ../../../i18n/ja/libirdaapplet.ts
TRANSLATIONS += ../../../i18n/ko/libirdaapplet.ts
TRANSLATIONS += ../../../i18n/no/libirdaapplet.ts
TRANSLATIONS += ../../../i18n/pl/libirdaapplet.ts
TRANSLATIONS += ../../../i18n/pt/libirdaapplet.ts
TRANSLATIONS += ../../../i18n/pt_BR/libirdaapplet.ts
TRANSLATIONS += ../../../i18n/sl/libirdaapplet.ts
TRANSLATIONS += ../../../i18n/zh_CN/libirdaapplet.ts
TRANSLATIONS += ../../../i18n/zh_TW/libirdaapplet.ts

