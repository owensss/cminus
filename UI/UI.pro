#-------------------------------------------------
#
# Project created by QtCreator 2013-06-14T14:41:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    TextEditor.cpp \
    CMinusHighlighter.cpp

HEADERS  += mainwindow.hpp \
    TextEditor.hpp \
    CMinusHighlighter.hpp

FORMS    += mainwindow.ui \
    TextEditor.ui
