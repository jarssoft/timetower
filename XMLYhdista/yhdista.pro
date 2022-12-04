QT -= gui
QT += xml
QT += xmlpatterns
QT += core
LIBS += -lxml2
CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ../Rutiinit/datetime.cpp \
        ../Rutiinit/stringrutins.cpp \
        ../XMLTVp/access.cpp \
        ../XMLTVp/compare.cpp \
        ../XMLTVp/episodenum.cpp \
        ../XMLTVp/filter.cpp \
        ../XMLTVp/grabber.cpp \
        ../XMLTVp/names.cpp \
        ../XMLTVp/parse-episode.cpp \
        ../XMLTVp/parse-specs.cpp \
        ../XMLTVp/parse-subtable.cpp \
        ../XMLTVp/programme.cpp \
        ../XMLTVp/vectors.cpp \
        ../XMLTVp/weektimes.cpp \
        ../XMLTVp/xmltvread.cpp \
        ../XMLTVp/xmltvvalidate.cpp \
        ../XMLTVp/xmltvwrite.cpp \
        console.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../Rutiinit/datetime.h \
    ../Rutiinit/stringrutins.h \
    ../XMLTVp/access.h \
    ../XMLTVp/compare.h \
    ../XMLTVp/episodenum.h \
    ../XMLTVp/filter.h \
    ../XMLTVp/grabber.h \
    ../XMLTVp/names.h \
    ../XMLTVp/parse-episode.h \
    ../XMLTVp/parse-specs.h \
    ../XMLTVp/parse-subtable.h \
    ../XMLTVp/programme.h \
    ../XMLTVp/vectors.h \
    ../XMLTVp/weektimes.h \
    ../XMLTVp/xmltvread.h \
    ../XMLTVp/xmltvvalidate.h \
    ../XMLTVp/xmltvwrite.h \
    console.hpp

INCLUDEPATH += /usr/include/libxml2

DISTFILES += \
    ../XMLTVp/xmltv.dtd
