QT       += core gui widgets printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = image_to_pdf
TEMPLATE = app

# The following define makes sure your project is compiled
# against the Qt 5 framework.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also use the following to disable deprecations
# DEFINES += QT_NO_DEPRECATED

SOURCES += \
        htmlprocessor.cpp \
        main.cpp \
        mainwindow.cpp \


HEADERS += \
        htmlprocessor.h \
        mainwindow.h \

