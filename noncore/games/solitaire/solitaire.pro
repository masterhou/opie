TEMPLATE	= app

CONFIG		+= qt warn_on release
DESTDIR		= $(OPIEDIR)/bin

HEADERS		= canvascard.h   canvasshapes.h   cardgame.h   cardgamelayout.h   cardpile.h   card.h   carddeck.h   canvascardgame.h   freecellcardgame.h   chicanecardgame.h   harpcardgame.h   teeclubcardgame.h   patiencecardgame.h   canvascardwindow.h

SOURCES		= canvascard.cpp canvasshapes.cpp cardgame.cpp cardgamelayout.cpp cardpile.cpp card.cpp carddeck.cpp canvascardgame.cpp freecellcardgame.cpp chicanecardgame.cpp harpcardgame.cpp teeclubcardgame.cpp   patiencecardgame.cpp canvascardwindow.cpp main.cpp

TARGET		= patience

INCLUDEPATH += $(OPIEDIR)/include
DEPENDPATH	+= $(OPIEDIR)/include
LIBS            += -lqpe

REQUIRES	= patience

TRANSLATIONS = ../i18n/de/patience.ts
TRANSLATIONS += ../i18n/es/patience.ts
TRANSLATIONS += ../i18n/pt/patience.ts
TRANSLATIONS += ../i18n/pt_BR/patience.ts
TRANSLATIONS   += ../i18n/en/patience.ts
TRANSLATIONS   += ../i18n/hu/patience.ts
TRANSLATIONS   += ../i18n/ja/patience.ts
TRANSLATIONS   += ../i18n/sl/patience.ts
TRANSLATIONS   += ../i18n/ko/patience.ts
TRANSLATIONS   += ../i18n/pl/patience.ts
TRANSLATIONS   += ../i18n/no/patience.ts
TRANSLATIONS   += ../i18n/fr/patience.ts
TRANSLATIONS   += ../i18n/zh_CN/patience.ts
TRANSLATIONS   += ../i18n/zh_TW/patience.ts
