#-------------------------------------------------
#
# Project created by QtCreator 2015-07-22T19:04:56
#
#-------------------------------------------------

QT       += core gui widgets
TARGET = QSciGlslLexer
TEMPLATE = lib
CONFIG += qt warn_on staticlib c++11

SOURCES += QSciGlslLexer.cpp \
		Constants.cpp

HEADERS += QSciGlslLexer.hpp \
		Constants.hpp
unix {
		target.path = /usr/lib
		INSTALLS += target
}
