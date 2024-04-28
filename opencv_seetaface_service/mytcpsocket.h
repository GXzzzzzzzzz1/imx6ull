#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpSocket(qintptr socketDescriptor,QObject *parent = nullptr);

signals:
    void readUserData(const int socketID, const QString &ip, const quint16 port, const QByteArray &data);//发送获得用户发过来的数据
    void sockDisConnect(const int socketID,const QString &ip,const quint16 port);//断开连接的用户信息

public slots:
    void readUData();
    void sentDataSlot(const QByteArray & ,const int);//发送信号的槽

private:
    qintptr socketID;//保存id，== this->socketDescriptor()；但是this->socketDescriptor()客户端断开会被释放，
                     //断开信号用this->socketDescriptor()得不到正确值
    unsigned int totalBytes = 0;
    unsigned int recvBytes = 0;
    QByteArray block;
    QByteArray pack_head;
    int i = 0;

};

#endif // MYTCPSOCKET_H
