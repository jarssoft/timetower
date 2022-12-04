#g++ `pkg-config --cflags --libs libxml-2.0` argparse.cpp datetime.cpp table.cpp xml_writer.hpp xmltv.cpp -o weektable
#g++ `xml2-config --cflags --libs` argparse.cpp datetime.cpp table.cpp xml_writer.hpp xmltv.cpp -o weektable

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

LIBS += -lxml2

QT -= gui
QT += xml
QT += xmlpatterns
QT += core
CONFIG += console

HEADERS += \
    ../Rutiinit/natlan.h \
    timetable.h \
    writer.h \
    writer_html.h \
    writer_svg.h \
    xml_writer.hpp \
    xmltv.h \
    sending.h \
    setup.h \
    sendinggroup.h \
    ../XMLTVp/names.h \
    ../XMLTVp/episodenum.h \
    ../XMLTVp/access.h \
    ../XMLTVp/tuotantotiedot.h \
    ../Rutiinit/datetime.h \
    ../Rutiinit/colors.h \
    ../Rutiinit/stringrutins.h \
    ../XMLTVGrid/gridp/parseprogramlist.h \
    ../XMLTVGrid/gridp/programmap.h \
    ../XMLTVGrid/gridp/programnames.h \

SOURCES += \
    ../Rutiinit/natlan.cpp \
    argparse.cpp \
    timetable.cpp \
    writer.cpp \
    writer_html.cpp \
    writer_svg.cpp \
    xmltv.cpp \
    sending.cpp \
    setup.cpp \
    sendinggroup.cpp \
    ../XMLTVp/names.cpp \
    ../XMLTVp/episodenum.cpp \
    ../XMLTVp/access.cpp \
    ../Rutiinit/datetime.cpp \
    ../Rutiinit/colors.cpp \
    ../Rutiinit/stringrutins.cpp \
    ../XMLTVGrid/gridp/parseprogramlist.cpp \
    ../XMLTVGrid/gridp/programmap.cpp \
    ../XMLTVGrid/gridp/programnames.cpp

INCLUDEPATH += /usr/include/libxml2

DISTFILES += \
    README.md
