#ifndef FACE_IDENTIFY_H
#define FACE_IDENTIFY_H

#include <QObject>
#include <seeta/FaceEngine.h>
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.h>
using namespace cv;

class face_identify : public QObject
{
    Q_OBJECT
public:
    explicit face_identify(QObject *parent = nullptr);

signals:
    //返回注册人脸id
    void registerfaceid_signals_faceidentify(int faceid);
    //返回识别出的人脸id
    void identifyfaceid_signals_faceidentify(int faceid);
public slots:
    //人脸注册
    void face_register(Mat faceimage);
    //人脸检测
    void face_query(Mat faceimage);

private:
    seeta::FaceEngine *fengineptr;
};

#endif // FACE_IDENTIFY_H
