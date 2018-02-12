#g++ `pkg-config --cflags --libs libxml-2.0` argparse.cpp datetime.cpp table.cpp xml_writer.hpp xmltv.cpp -o weektable
#g++ `xml2-config --cflags --libs` argparse.cpp datetime.cpp table.cpp xml_writer.hpp xmltv.cpp -o weektable

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    datetime.cpp \
    argparse.cpp \
    table.cpp \
    xmltv.cpp \
    colors.cpp \
    descparser.cpp \
    stringrutins.cpp \
    html_table_writer.cpp \
    svg_writer.cpp \
    sending.cpp \
    table_writer.cpp \
    setup.cpp

LIBS += -lxml2

HEADERS += \
    datetime.h \
    xml_writer.hpp \
    table.h \
    xmltv.h \
    colors.h \
    descparser.h \
    stringrutins.h \
    html_table_writer.h \
    svg_writer.h \
    sending.h \
    table_writer.h \
    setup.h

INCLUDEPATH += /usr/include/libxml2
