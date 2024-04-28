QT       += core gui network
QT += sql #添加数据库模块
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
    face_detector.cpp \
    face_identify.cpp \
    face_sqlite.cpp \
    main.cpp \
    mainwindow.cpp \
    mytcpsocket.cpp \
    tcp_server.cpp

HEADERS += \
    camera.h \
    face_detector.h \
    face_identify.h \
    face_sqlite.h \
    mainwindow.h \
    mytcpsocket.h \
    tcp_server.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += /home/book/opencv-3.4.9-ubuntu/install/include
LIBS += /home/book/opencv-3.4.9-ubuntu/install/lib/libopencv_core.so \
    /home/book/opencv-3.4.9-ubuntu/install/lib/libopencv_highgui.so \
    /home/book/opencv-3.4.9-ubuntu/install/lib/libopencv_imgproc.so \
    /home/book/opencv-3.4.9-ubuntu/install/lib/libopencv_videoio.so \
    /home/book/opencv-3.4.9-ubuntu/install/lib/libopencv_imgcodecs.so \
    /home/book/opencv-3.4.9-ubuntu/install/lib/libopencv_objdetect.so

INCLUDEPATH += /home/book/SeetaFace2/install/include
LIBS += /home/book/SeetaFace2/install/lib/libSeetaFaceTracker.so \
    /home/book/SeetaFace2/install/lib/libSeetaFaceDetector.so \
    /home/book/SeetaFace2/install/lib/libSeetaNet.so \
    /home/book/SeetaFace2/install/lib/libSeetaFaceLandmarker.so \
    /home/book/SeetaFace2/install/lib/libSeetaQualityAssessor.so \
    /home/book/SeetaFace2/install/lib/libSeetaFaceRecognizer.so

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
