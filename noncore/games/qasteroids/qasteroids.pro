TEMPLATE	= app
CONFIG		+= qt warn_on release
DESTDIR		= $(OPIEDIR)/bin
HEADERS		= ledmeter.h  sprites.h  toplevel.h  view.h
SOURCES		= ledmeter.cpp  toplevel.cpp  view.cpp main.cpp
TARGET		= qasteroids
INCLUDEPATH += $(OPIEDIR)/include
DEPENDPATH	+= $(OPIEDIR)/include
LIBS            += -lqpe



TRANSLATIONS = ../../../i18n/de/qasteroids.ts
TRANSLATIONS += ../../../i18n/en/qasteroids.ts
TRANSLATIONS += ../../../i18n/es/qasteroids.ts
TRANSLATIONS += ../../../i18n/fr/qasteroids.ts
TRANSLATIONS += ../../../i18n/hu/qasteroids.ts
TRANSLATIONS += ../../../i18n/ja/qasteroids.ts
TRANSLATIONS += ../../../i18n/ko/qasteroids.ts
TRANSLATIONS += ../../../i18n/no/qasteroids.ts
TRANSLATIONS += ../../../i18n/pl/qasteroids.ts
TRANSLATIONS += ../../../i18n/pt/qasteroids.ts
TRANSLATIONS += ../../../i18n/pt_BR/qasteroids.ts
TRANSLATIONS += ../../../i18n/sl/qasteroids.ts
TRANSLATIONS += ../../../i18n/zh_CN/qasteroids.ts
TRANSLATIONS += ../../../i18n/zh_TW/qasteroids.ts

