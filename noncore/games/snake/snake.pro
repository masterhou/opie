TEMPLATE	= app
CONFIG		+= qt warn_on release
DESTDIR		= $(OPIEDIR)/bin
HEADERS		= snake.h target.h obstacle.h interface.h codes.h
SOURCES		= snake.cpp target.cpp obstacle.cpp interface.cpp main.cpp
TARGET		= snake
INCLUDEPATH += $(OPIEDIR)/include
DEPENDPATH	+= $(OPIEDIR)/include
LIBS            += -lqpe



TRANSLATIONS = ../../../i18n/de/snake.ts
TRANSLATIONS += ../../../i18n/en/snake.ts
TRANSLATIONS += ../../../i18n/es/snake.ts
TRANSLATIONS += ../../../i18n/fr/snake.ts
TRANSLATIONS += ../../../i18n/hu/snake.ts
TRANSLATIONS += ../../../i18n/ja/snake.ts
TRANSLATIONS += ../../../i18n/ko/snake.ts
TRANSLATIONS += ../../../i18n/no/snake.ts
TRANSLATIONS += ../../../i18n/pl/snake.ts
TRANSLATIONS += ../../../i18n/pt/snake.ts
TRANSLATIONS += ../../../i18n/pt_BR/snake.ts
TRANSLATIONS += ../../../i18n/sl/snake.ts
TRANSLATIONS += ../../../i18n/zh_CN/snake.ts
TRANSLATIONS += ../../../i18n/zh_TW/snake.ts

