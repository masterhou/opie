TEMPLATE	= lib
CONFIG		+= qt warn_on release
HEADERS	=   unikeyboard.h unikeyboardimpl.h
SOURCES	=   unikeyboard.cpp unikeyboardimpl.cpp
TARGET		= qunikeyboard
DESTDIR		= ../../plugins/inputmethods
INCLUDEPATH += $(OPIEDIR)/include
DEPENDPATH      += ../$(OPIEDIR)/include ../../launcher
LIBS            += -lqpe
VERSION		= 1.0.0



TRANSLATIONS = ../../i18n/de/libqunikeyboard.ts
TRANSLATIONS += ../../i18n/en/libqunikeyboard.ts
TRANSLATIONS += ../../i18n/es/libqunikeyboard.ts
TRANSLATIONS += ../../i18n/fr/libqunikeyboard.ts
TRANSLATIONS += ../../i18n/hu/libqunikeyboard.ts
TRANSLATIONS += ../../i18n/ja/libqunikeyboard.ts
TRANSLATIONS += ../../i18n/ko/libqunikeyboard.ts
TRANSLATIONS += ../../i18n/no/libqunikeyboard.ts
TRANSLATIONS += ../../i18n/pl/libqunikeyboard.ts
TRANSLATIONS += ../../i18n/pt/libqunikeyboard.ts
TRANSLATIONS += ../../i18n/pt_BR/libqunikeyboard.ts
TRANSLATIONS += ../../i18n/sl/libqunikeyboard.ts
TRANSLATIONS += ../../i18n/zh_CN/libqunikeyboard.ts
TRANSLATIONS += ../../i18n/zh_TW/libqunikeyboard.ts

