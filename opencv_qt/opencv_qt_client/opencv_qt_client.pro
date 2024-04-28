QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    camera.cpp \
    main.cpp \
    mainwindow.cpp \
    tcpclient.cpp

HEADERS += \
    camera.h \
    mainwindow.h \
    tcpclient.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += /home/book/opencv-3.4.9/install/include
LIBS += /home/book/opencv-3.4.9/install/lib/libopencv_core.so \
    /home/book/opencv-3.4.9/install/lib/libopencv_highgui.so \
    /home/book/opencv-3.4.9/install/lib/libopencv_imgproc.so \
    /home/book/opencv-3.4.9/install/lib/libopencv_videoio.so \
    /home/book/opencv-3.4.9/install/lib/libopencv_imgcodecs.so \
    /home/book/opencv-3.4.9/install/lib/libopencv_objdetect.so


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
