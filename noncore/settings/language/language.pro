TEMPLATE	= app
CONFIG		+= qt warn_on release
DESTDIR		= $(OPIEDIR)/bin
HEADERS		= settings.h
SOURCES		= language.cpp main.cpp
INTERFACES	= languagesettingsbase.ui
INCLUDEPATH += $(OPIEDIR)/include
DEPENDPATH	+= ../$(OPIEDIR)/include
LIBS            += -lqpe
TARGET		= language



TRANSLATIONS = ../../../i18n/de/language.ts
TRANSLATIONS += ../../../i18n/en/language.ts
TRANSLATIONS += ../../../i18n/es/language.ts
TRANSLATIONS += ../../../i18n/fr/language.ts
TRANSLATIONS += ../../../i18n/hu/language.ts
TRANSLATIONS += ../../../i18n/ja/language.ts
TRANSLATIONS += ../../../i18n/ko/language.ts
TRANSLATIONS += ../../../i18n/no/language.ts
TRANSLATIONS += ../../../i18n/pl/language.ts
TRANSLATIONS += ../../../i18n/pt/language.ts
TRANSLATIONS += ../../../i18n/pt_BR/language.ts
TRANSLATIONS += ../../../i18n/sl/language.ts
TRANSLATIONS += ../../../i18n/zh_CN/language.ts
TRANSLATIONS += ../../../i18n/zh_TW/language.ts

