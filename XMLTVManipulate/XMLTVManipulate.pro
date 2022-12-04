TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt
#QT += testlib

QT += testlib
QT -= gui
QT += xml
QT += xmlpatterns
QT += core

LIBS += -lxml2 -lpthread

SOURCES += main.cpp \
    ../Rutiinit/natlan.cpp \
    ../Rutiinit/datetime.cpp \
    ../Rutiinit/stringrutins.cpp \
    ../XMLTVp/names.cpp \
    ../XMLTVp/episodenum.cpp \
    #../XMLTVp/episodeparser.cpp \
    ../XMLTVp/parse-episode.cpp \
    ../XMLTVp/access.cpp \
    ../XMLTVp/programme.cpp \
    #../XMLTVp/tuotantotiedot.cpp \
    ../XMLTVp/parse-specs.cpp \
    argparse.cpp \
    manipulate.cpp \
    translate.cpp \
    wikimediap/parsewikipedia.cpp \
    wikimediap/wikinames.cpp \
    descparser.cpp

HEADERS += \
    ../Rutiinit/natlan.h \
    ../Rutiinit/datetime.h \
    ../Rutiinit/stringrutins.h \
    ../XMLTVp/names.h \
    ../XMLTVp/episodenum.h \
    ../XMLTVp/parse-episode.h \
    #../XMLTVp/episodeparser.h \
    ../XMLTVp/parse-specs.h \
    ../XMLTVp/access.h \
    ../XMLTVp/programme.h \
    ../XMLTVp/tuotantotiedot.h \
    manipulate.h \
    translate.h \
    descparser.h \
    wikimediap/parsewikipedia.h \
    wikimediap/wikinames.h \

INCLUDEPATH += /usr/include/libxml2
#INCLUDEPATH += ../XMLTVp
INCLUDEPATH += wikimediap

DISTFILES +=
