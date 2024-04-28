#include "mytcpsocket.h"
#include "tcp_server.h"
#include <QThread>
#include <QDataStream>

tcp_server::tcp_server(QObject *parent) : QTcpServer(parent)
{

}



void tcp_server::incomingConnection(qintptr handle)
{
    MyTcpSocket * tcpTemp = new MyTcpSocket(handle);
    QThread * thread = new QThread(tcpTemp);

    connect(tcpTemp,&MyTcpSocket::readUserData,this,&tcp_server::readDataSlot);//接受到数据
    connect(tcpTemp,&MyTcpSocket::sockDisConnect,this,&tcp_server::sockDisConnectSlot);//断开连接的处理，从列表移除，并释放断开的Tcpsocket
    //connect(this,&tcp_server::sentData,tcpTemp,&MyTcpSocket::sentDataSlot);//发送数据
    connect(tcpTemp,&MyTcpSocket::disconnected,thread,&QThread::quit);//断开连接时线程退出

    tcpTemp->moveToThread(thread);
    thread->start();

    tcpClient.insert(handle,tcpTemp);
}

void tcp_server::readDataSlot(const int socketID, const QString &ip, const quint16 port, const QByteArray &data)
{
    Q_UNUSED(ip)
    Q_UNUSED(port)
    Q_UNUSED(data)


//    qDebug()<<"服务器接收到数据长度:"<<data.size();
//    qDebug() << "前10个字节：" << data.left(10);
    QDataStream stream(data);
    QByteArray recvdata;
    stream >> recvdata;

    std::vector<uchar> imageData(recvdata.begin(), recvdata.end());
    cv::Mat recv_image = cv::imdecode(imageData, cv::IMREAD_COLOR);

    // 检查是否成功解码
    if (!recv_image.empty()) {
        emit cameradata_signals_camera(recv_image);
    } else {
        qDebug() << "解码失败!";
    }

}

void tcp_server::sockDisConnectSlot(int socketID, QString ip, quint16 port)
{
    Q_UNUSED(ip)
    Q_UNUSED(port)
    tcpClient[socketID]->deleteLater();
    tcpClient.remove(socketID);
}
