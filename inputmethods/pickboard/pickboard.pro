TEMPLATE	= lib
CONFIG		+= qt warn_on release
HEADERS	=   pickboard.h pickboardcfg.h pickboardimpl.h pickboardpicks.h
SOURCES	=   pickboard.cpp pickboardcfg.cpp pickboardimpl.cpp pickboardpicks.cpp
TARGET		= qpickboard
DESTDIR		= ../../plugins/inputmethods
INCLUDEPATH += $(OPIEDIR)/include
DEPENDPATH      += ../$(OPIEDIR)/include ../../launcher
LIBS            += -lqpe
VERSION		= 1.0.0



TRANSLATIONS = ../../i18n/de/libqpickboard.ts
TRANSLATIONS += ../../i18n/en/libqpickboard.ts
TRANSLATIONS += ../../i18n/es/libqpickboard.ts
TRANSLATIONS += ../../i18n/fr/libqpickboard.ts
TRANSLATIONS += ../../i18n/hu/libqpickboard.ts
TRANSLATIONS += ../../i18n/ja/libqpickboard.ts
TRANSLATIONS += ../../i18n/ko/libqpickboard.ts
TRANSLATIONS += ../../i18n/no/libqpickboard.ts
TRANSLATIONS += ../../i18n/pl/libqpickboard.ts
TRANSLATIONS += ../../i18n/pt/libqpickboard.ts
TRANSLATIONS += ../../i18n/pt_BR/libqpickboard.ts
TRANSLATIONS += ../../i18n/sl/libqpickboard.ts
TRANSLATIONS += ../../i18n/zh_CN/libqpickboard.ts
TRANSLATIONS += ../../i18n/zh_TW/libqpickboard.ts

