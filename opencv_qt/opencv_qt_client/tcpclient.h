#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.h>

class tcpclient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit tcpclient(QObject *parent = nullptr);

signals:
    //发送数据完成,通知主线程
    void notify_mainwindows_cmd_signals_tcpclient(int cmd);//0表示发送结束
public slots:
    void connectToServer(QString ip,unsigned short port);
    void disconnectToServer();
    void sendimageToServer(cv::Mat image);
    void sentDataToServer(const QByteArray &data);


private:


};

#endif // TCPCLIENT_H
