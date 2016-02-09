#-------------------------------------------------
#
# Project created by QtCreator 2011-01-08T11:02:03
#
#-------------------------------------------------

QT       += core gui phonon

TARGET = yoshitimer
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    xmlreader.cpp \
    xmlwriter.cpp

HEADERS  += mainwindow.h \
    xmlreader.h \
    xmlwriter.h

FORMS    += \
    mainwindow.ui

OTHER_FILES += sounds/getready.wav \
    sounds/phaser.wav \
    sounds/pleaserest.wav \
    sounds/setcompleted.wav \
    sounds/supersetcompleted.wav \
    sounds/work.wav \
    sounds/buzzer.wav \
    sounds/youaredoneeverything.wav
