#ifndef FACE_DETECTOR_H
#define FACE_DETECTOR_H

#include <QObject>
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.h>

using namespace cv;

class face_detector : public QObject
{
    Q_OBJECT
public:
    explicit face_detector(QObject *parent = nullptr);

signals:
    //返回人脸检测结果
    void check_result_signals_face_detector(const std::vector<cv::Rect>& faces);
public slots:
    //进行人脸检测
    void facedector_working(cv::Mat faceimage);
private:

    CascadeClassifier *classifier;
    std::vector<cv::Rect> AllFace;
};

#endif // FACE_DETECTOR_H
