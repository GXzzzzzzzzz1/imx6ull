#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "mytcpsocket.h"

#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.h>

#include <QPixmap>
#include <QTcpServer>
#include <QTcpSocket>
#include <qimage.h>
class tcp_server : public QTcpServer
{
    Q_OBJECT
public:
    explicit tcp_server(QObject *parent = nullptr);
protected:
    virtual void incomingConnection(qintptr handle) override;

private:
    QMap<qintptr,MyTcpSocket *> tcpClient;//socketID对应socket
signals:
    void cameradata_signals_camera(cv::Mat image);
public slots:
    void readDataSlot(const int socketID, const QString &ip, const quint16 port, const QByteArray &data);//处理获得用户发过来的数据
    void sockDisConnectSlot(int socketID, QString ip, quint16 port);//断开连接的用户信息

};

#endif // TCP_SERVER_H
