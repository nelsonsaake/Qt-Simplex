#-------------------------------------------------
#
# Project created by QtCreator 2020-01-26T17:03:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simplex
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    simplexMethod/adjust.cpp \
    simplexMethod/augmentedlpmodel.cpp \
    simplexMethod/globals.cpp \
    simplexMethod/lpmodel.cpp \
    simplexMethod/matrix.cpp \
    simplexMethod/outputcastingformarting.cpp \
    simplexMethod/simplextableau.cpp \
    simplexMethod/sm1_simplex_approach.cpp \
    simplexMethod/util.cpp \
    ui/mainwindow.cpp \
    ui/messagedialog.cpp

HEADERS += \
    simplexMethod/adjust.h \
    simplexMethod/augmentedlpmodel.h \
    simplexMethod/globals.h \
    simplexMethod/lpmodel.h \
    simplexMethod/matrix.h \
    simplexMethod/outputcastingformarting.h \
    simplexMethod/simplextableau.h \
    simplexMethod/sm1_simplex_approach.h \
    simplexMethod/util.h \
    ui/mainwindow.h \
    ui/messagedialog.h

FORMS += \
    ui/mainwindow.ui \
    ui/messagedialog.ui

RESOURCES += \
    ui/img.qrc

DISTFILES += \
    ui/img/background.PNG
