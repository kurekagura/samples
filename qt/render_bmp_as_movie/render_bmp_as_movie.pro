QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

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
    captureqthread.cpp \
    capturestdthread.cpp \
    imagecapturethread.cpp \
    imageprocessorthread.cpp \
    main.cpp \
    mywindow1.cpp \
    mywindow2.cpp \
    mywindow3.cpp

HEADERS += \
    captureqthread.h \
    capturestdthread.h \
    imagecapturethread.h \
    imageprocessorthread.h \
    mywindow1.h \
    mywindow2.h \
    mywindow3.h \
    queuechannel.h

FORMS += \
    mywindow1.ui \
    mywindow2.ui \
    mywindow3.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += "C:/lib/opencv-4.5.4/include"

LIBS += -L"C:/lib/opencv-4.5.4/x64/vc16/lib"
LIBS += -lopencv_world454d

DISTFILES += \
    readme.txt
