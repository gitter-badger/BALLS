#-------------------------------------------------
#
# Project created by QtCreator 2015-07-22T19:04:56
#
#-------------------------------------------------

TARGET = QSciGlslLexer
TEMPLATE = lib
CONFIG += staticlib

SOURCES += QSciGlslLexer.cpp

HEADERS += QSciGlslLexer.hpp
unix {
    target.path = /usr/lib
    INSTALLS += target
}
