#include "mytcpsocket.h"
#include <QThread>
#include <QHostAddress>

MyTcpSocket::MyTcpSocket(qintptr socketDescriptor,QObject *parent) : QTcpSocket(parent),socketID(socketDescriptor)
{
    this->setSocketDescriptor(socketDescriptor);
    block.clear();
    pack_head.clear();
    pack_head.resize(sizeof(int));
    connect(this,&MyTcpSocket::readyRead,this,&MyTcpSocket::readUData);

    connect(this,&MyTcpSocket::disconnected, //断开连接的信号转换
                [this](){
                    emit sockDisConnect(socketID,this->peerAddress().toString(),this->peerPort());//发送断开连接的用户信息
    });
}


void MyTcpSocket::readUData()
{
    if(this->bytesAvailable() == 0)
    {
        return;
    }

    //读包头
    if(totalBytes == 0 && pack_head.size() == 4)
    {
        pack_head.clear();
        if(this->bytesAvailable() >= sizeof(int))
        {
            pack_head.append(this->read(4));
            totalBytes = *(int *)pack_head.data();

        }
        else
        {
            pack_head.append(this->read(this->bytesAvailable()));
            return;
        }
    }
    else if(totalBytes == 0 && pack_head.size() < 4)
    {
        if(this->bytesAvailable() >= (4 - pack_head.size()))
        {
            pack_head.append(this->read(4 - pack_head.size()));
            totalBytes = *(int *)pack_head.data();
        }
    }



    while(totalBytes - recvBytes >0 && this->bytesAvailable())
    {
        block.append(this->read(totalBytes - recvBytes));
        recvBytes = block.size();
    }

    if(totalBytes == recvBytes)
    {
        emit readUserData(socketID,this->peerAddress().toString(),this->peerPort() ,block);//发送用户发过来的数据
        block.clear();
        totalBytes = 0;
        recvBytes = 0;
    }

    if(this->bytesAvailable() > 0 && (totalBytes - recvBytes) == 0)
    {
        readUData();
    }
}

void MyTcpSocket::sentDataSlot(const QByteArray &data, const int id)
{
    //如果是服务器判断好，直接调用write会出现跨线程的现象，所以服务器用广播，每个连接判断是否是自己要发送的信息。
    if(id == socketID)//判断是否是此socket的信息
    {
        //添加包头
        int len = data.size();
        QByteArray msg((char *)&len,4);
        msg.append(data);
        write(msg);
    }
}
