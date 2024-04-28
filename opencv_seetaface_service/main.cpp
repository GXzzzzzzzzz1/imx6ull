#include "mainwindow.h"

#include <QApplication>
#include <QMetaType>

#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.h>

int main(int argc, char *argv[])
{
    qRegisterMetaType< cv::Mat >("cv::Mat");
    qRegisterMetaType< std::vector<cv::Rect> >("std::vector<cv::Rect>");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
