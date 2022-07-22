QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    basewidget.cpp \
    briefwidget.cpp \
    friendwidget.cpp \
    main.cpp \
    msgwidget.cpp \
    protocol.cpp \
    qclient.cpp

HEADERS += \
    basewidget.h \
    briefwidget.h \
    friendwidget.h \
    msgwidget.h \
    protocol.h \
    qclient.h

FORMS += \
    basewidget.ui \
    briefwidget.ui \
    friendwidget.ui \
    msgwidget.ui \
    qclient.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    config.qrc
