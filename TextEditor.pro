TEMPLATE = app

QT += qml quick
CONFIG += c++11


SOURCES += main.cpp \
    options.cpp \
    stack.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    options.h \
    stack.h

DISTFILES += \

CONFIG += qtquickcompiler
