#include "face_detector.h"
#include <QDebug>
face_detector::face_detector(QObject *parent) : QObject(parent)
{
    classifier = new CascadeClassifier("haarcascade_frontalface_alt2.xml");
}

void face_detector::facedector_working(cv::Mat faceimage)
{
    cv::Mat image = faceimage.clone();
    cv::Mat GrayFrame; // 灰度图
    std::vector<cv::Rect> null;//返回一个空的结果
    null.clear();
    if(image.empty())
    {
        emit check_result_signals_face_detector(null);
        return;
    }


    //获取灰度图
    cv::cvtColor(image, GrayFrame, cv::COLOR_BGR2GRAY);
    classifier->detectMultiScale(GrayFrame,AllFace,1.3,0,0,cv::Size(200, 200));
    //qDebug()<<"人脸数量:"<<AllFace.size();
    if(AllFace.size())
    {
        emit check_result_signals_face_detector(AllFace);
    }
    else
    {
        emit check_result_signals_face_detector(null);
    }
}
