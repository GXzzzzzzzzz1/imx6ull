#include "tcpclient.h"
#include <QDataStream>
#include <QDebug>
#include <QImage>

tcpclient::tcpclient(QObject *parent) : QTcpSocket(parent)
{


}

void tcpclient::connectToServer(QString ip, unsigned short port)
{
    this->connectToHost(QHostAddress(ip),port);
}

void tcpclient::disconnectToServer()
{
    this->close();
}

void tcpclient::sendimageToServer(cv::Mat image)
{
    qDebug()<<"send image to server";
//    QByteArray byteArray;
//    QDataStream stream(&byteArray, QIODevice::WriteOnly);
//    QImage img(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
//    stream << img;
//    sentDataToServer(byteArray);//增加包头发送
//    emit notify_mainwindows_cmd_signals_tcpclient(0);//进行下一次发送

    //编码
    std::vector<uchar> buf;
    cv::imencode(".jpg",image,buf);
    QByteArray byte((const char*)buf.data(),buf.size());
    //准备发送
    QByteArray senddata;
    QDataStream stream(&senddata, QIODevice::WriteOnly);
    stream << byte;
    sentDataToServer(senddata);
    qDebug() << "前10个字节：" << senddata.left(10);

    emit notify_mainwindows_cmd_signals_tcpclient(0);//进行下一次发送
}

void tcpclient::sentDataToServer(const QByteArray &data)
{
//    //测试
//    QByteArray data1 = "Hello World";
//    int len = data1.size();
//    qDebug()<<"数据长度"<<len;
//    QByteArray msg((char *)&len,4);
//    msg.append(data1);
//    write(msg);

    //添加包头
    int len = data.size();
    qDebug()<<"数据长度"<<len;
    QByteArray msg((char *)&len,4);
    msg.append(data);
    write(msg);
}

