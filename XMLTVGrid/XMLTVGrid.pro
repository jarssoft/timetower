TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    weeklyrule.cpp \
    ../XMLTVp/names.cpp \
    ../XMLTVp/episodenum.cpp \
    ../XMLTVp/access.cpp \
    ../XMLTVtoHTML/datetime.cpp \
    ../XMLTVtoHTML/descparser.cpp \
    ../XMLTVtoHTML/stringrutins.cpp \
    suhde.cpp \
    neliosuhde.cpp \
    nsverkko.cpp \
    suhde2d.cpp \
    kuutiosuhde.cpp \
    ksverkko.cpp \
    kstaso.cpp \
    sverkko.cpp \
    ohjelmapaikka.cpp \
    ohjelmanimi.cpp \
    gridp/parseprogramlist.cpp \
    gridp/programnames.cpp \
    main-read.cpp \
    gridp/programmap.cpp


HEADERS += \
    weeklyrule.h \
    ../XMLTVp/names.h \
    ../XMLTVp/episodenum.h \
    ../XMLTVp/access.h \
    ../XMLTVtoHTML/datetime.h \
    ../XMLTVtoHTML/descparser.h \
    ../XMLTVtoHTML/stringrutins.h \
    ../XMLWriter/xml_writer.hpp \
    suhde.h \
    neliosuhde.h \
    nsverkko.h \
    suhde2d.h \
    kuutiosuhde.h \
    ksverkko.h \
    kstaso.h \
    vector3d.h \
    sverkko.h \
    ohjelmapaikka.h \
    ohjelmanimi.h \
    gridp/parseprogramlist.h \
    gridp/programnames.h \
    gridp/programmap.h


LIBS += -lxml2 -lpthread
INCLUDEPATH += /usr/include/libxml2
INCLUDEPATH += ../XMLTVp
INCLUDEPATH += ../vector2d

DISTFILES += \
    result.txt
