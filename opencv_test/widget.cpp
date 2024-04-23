#include "widget.h"
#include "ui_widget.h"


#include <QPixmap>
#include <QImage>
#include <Qt>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    cap = new VideoCapture(0,CAP_V4L);
    classifier = new CascadeClassifier("haarcascade_frontalface_alt2.xml");


    if(!cap->isOpened())
    {
        qDebug()<<"Camera open failed\n";
    }
    else
    {
        qDebug()<<"Camera open success\n";
    }

    cap->set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    //cap->set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('Y', 'U', 'Y', 'V'));
    // 获取并打印摄像头的颜色格式
    double fourcc = cap->get(cv::CAP_PROP_FOURCC);

    if (fourcc == cv::VideoWriter::fourcc('M', 'J', 'P', 'G')) {
        qDebug() << "Camera is set to MJPEG format\n";
    } else if (fourcc == cv::VideoWriter::fourcc('Y', 'U', 'Y', 'V')) {
        qDebug() << "Camera is set to YUV422 format\n";
    } else {
        qDebug() << "Unknown camera format (FourCC code: " << fourcc << ")\n";
    }


    timer = new QTimer();
    connect(timer,&QTimer::timeout,this,&Widget::timeout);
    timer->setInterval(1);
    timer->setSingleShot(true);
    timer->start();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::timeout()
{
    cv::Mat frame;
    cv::Mat GrayFrame;

    *cap >> frame;

    if (!frame.empty()) {

        cv::cvtColor(frame, GrayFrame, cv::COLOR_BGR2GRAY);
        //cv::equalizeHist(GrayFrame,GrayFrame);
        classifier->detectMultiScale(GrayFrame,AllFace,1.3,0,0,cv::Size(200, 200));
        qDebug()<<AllFace.size();
        if(AllFace.size())
            cv::rectangle(GrayFrame,AllFace[0],cv::Scalar(0,255,255));

        QImage qImage(GrayFrame.data, GrayFrame.cols, GrayFrame.rows, GrayFrame.step, QImage::Format_Grayscale8);//Format_RGB888 Format_Grayscale8
        QPixmap pixmap = QPixmap::fromImage(qImage);
        ui->label->setPixmap(pixmap);
    }
    timer->start();
}

