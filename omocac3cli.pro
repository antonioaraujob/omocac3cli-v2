#-------------------------------------------------
#
# Project created by QtCreator 2015-01-18T17:47:44
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = omocac3cli
CONFIG   += console
CONFIG   -= app_bundle

# para eliminar la aparicion de los qDebug descomentar la siguiente linea
DEFINES += QT_NO_DEBUG_OUTPUT


TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

LIBS += -lsqlite3

SOURCES += main.cpp \
    mainwindow.cpp \
    simulation.cpp \
    individual.cpp \
    normativephenotypicpart.cpp \
    gridsubinterval.cpp \
    normativegrid.cpp \
    externalfile.cpp \
    selection.cpp \
    mutation.cpp \
    cell.cpp \
    gen.cpp \
    ctable.cpp \
    ctablegen.cpp \
    emulator.cpp

HEADERS += \
    mainwindow.h \
    simulation.h \
    individual.h \
    normativephenotypicpart.h \
    gridsubinterval.h \
    normativegrid.h \
    externalfile.h \
    selection.h \
    mutation.h \
    cell.h \
    gen.h \
    ctable.h \
    ctablegen.h \
    emulator.h
