SOURCES	+= chess.cpp main.cpp 
HEADERS	+= chess.h 
DESTDIR = $(OPIEDIR)/bin
TARGET = chess
DEPENDPATH	+= $(OPIEDIR)/include
INTERFACES = mainwindow.ui 
IMAGES	= images/new.png images/repeat.png images/txt.png images/back.png 
TEMPLATE	=app
CONFIG	+= qt warn_on release
INCLUDEPATH += $(OPIEDIR)/include
LIBS	+= -lqpe
DBFILE	= chess.db
LANGUAGE	= C++
CPP_ALWAYS_CREATE_SOURCE	= TRUE



TRANSLATIONS = ../../../i18n/de/chess.ts
TRANSLATIONS += ../../../i18n/en/chess.ts
TRANSLATIONS += ../../../i18n/es/chess.ts
TRANSLATIONS += ../../../i18n/fr/chess.ts
TRANSLATIONS += ../../../i18n/hu/chess.ts
TRANSLATIONS += ../../../i18n/ja/chess.ts
TRANSLATIONS += ../../../i18n/ko/chess.ts
TRANSLATIONS += ../../../i18n/no/chess.ts
TRANSLATIONS += ../../../i18n/pl/chess.ts
TRANSLATIONS += ../../../i18n/pt/chess.ts
TRANSLATIONS += ../../../i18n/pt_BR/chess.ts
TRANSLATIONS += ../../../i18n/sl/chess.ts
TRANSLATIONS += ../../../i18n/zh_CN/chess.ts
TRANSLATIONS += ../../../i18n/zh_TW/chess.ts

