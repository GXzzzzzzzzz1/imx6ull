#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.h>


#include "vector"
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QTimer>
using namespace cv;
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void timeout();
private:
    Ui::Widget *ui;
    VideoCapture *cap;
    CascadeClassifier *classifier;
    std::vector<cv::Rect> AllFace;
    QTimer *timer;
};
#endif // WIDGET_H
