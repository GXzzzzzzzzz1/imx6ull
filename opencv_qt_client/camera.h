#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.h>

using namespace cv;

class camera : public QObject
{
    Q_OBJECT
public:
    explicit camera(QObject *parent = nullptr);

    //强制停止
    bool forcestop = false;
signals:
    //通知主线程摄像头数量
    void cameranumber_signals_camera(int number);
    //通知主线程tips
    void results_signals_camera(QString results);
    //发送摄像头数据给主线程
    void cameradata_signals_camera(cv::Mat image);
public slots:
    //检测摄像头是否存在
    void getcameranumber(void);
    //启动摄像头
    void cameraworking(int camera_index);

private:


};

#endif // CAMERA_H
