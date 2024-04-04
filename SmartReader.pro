QT       += core gui network
CONFIG += c++11
LIBS += -lpoppler-qt5 -lasound
QMAKE_CXXFLAGS += -g -O0

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bookwidget.cpp \
    camerawidget.cpp \
    filehandle.cpp \
    main.cpp \
    myrecordc.c \
    readwidget.cpp \
    startwindow.cpp \
    v4l2api.cpp

HEADERS += \
    bookwidget.h \
    camerawidget.h \
    filehandle.h \
    myrecordc.h \
    readwidget.h \
    startwindow.h \
    v4l2api.h

FORMS += \
    bookwidget.ui \
    camerawidget.ui \
    readwidget.ui \
    startwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
