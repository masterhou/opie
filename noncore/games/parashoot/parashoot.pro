TEMPLATE	= app
CONFIG		+= qt warn_on release
DESTDIR		= $(OPIEDIR)/bin
HEADERS		= interface.h man.h cannon.h base.h bullet.h helicopter.h
SOURCES		= main.cpp interface.cpp man.cpp cannon.cpp base.cpp bullet.cpp helicopter.cpp
TARGET		= parashoot 
INCLUDEPATH += $(OPIEDIR)/include
DEPENDPATH	+= $(OPIEDIR)/include
LIBS            += -lqpe



TRANSLATIONS = ../../../i18n/de/parashoot.ts
TRANSLATIONS += ../../../i18n/en/parashoot.ts
TRANSLATIONS += ../../../i18n/es/parashoot.ts
TRANSLATIONS += ../../../i18n/fr/parashoot.ts
TRANSLATIONS += ../../../i18n/hu/parashoot.ts
TRANSLATIONS += ../../../i18n/ja/parashoot.ts
TRANSLATIONS += ../../../i18n/ko/parashoot.ts
TRANSLATIONS += ../../../i18n/no/parashoot.ts
TRANSLATIONS += ../../../i18n/pl/parashoot.ts
TRANSLATIONS += ../../../i18n/pt/parashoot.ts
TRANSLATIONS += ../../../i18n/pt_BR/parashoot.ts
TRANSLATIONS += ../../../i18n/sl/parashoot.ts
TRANSLATIONS += ../../../i18n/zh_CN/parashoot.ts
TRANSLATIONS += ../../../i18n/zh_TW/parashoot.ts

