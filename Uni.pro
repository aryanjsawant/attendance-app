QT       += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addbox.cpp \
    attendance.cpp \
    findbox.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    subject.cpp \
    updatebox.cpp

HEADERS += \
    addbox.h \
    attendance.h \
    findbox.h \
    login.h \
    mainwindow.h \
    subject.h \
    updatebox.h

FORMS += \
    addbox.ui \
    attendance.ui \
    findbox.ui \
    login.ui \
    mainwindow.ui \
    subject.ui \
    updatebox.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
