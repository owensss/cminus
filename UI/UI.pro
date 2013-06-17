#-------------------------------------------------
#
# Project created by QtCreator 2013-06-14T14:41:35
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++0x

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    TextEditor.cpp \
    CMinusHighlighter.cpp \
    CMinusFiles.cpp

HEADERS  += mainwindow.hpp \
    TextEditor.hpp \
    CMinusHighlighter.hpp \
    CMinusFiles.hpp

FORMS    += mainwindow.ui \
    TextEditor.ui
