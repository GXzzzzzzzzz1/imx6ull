#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>

class tcpclient : public QObject
{
    Q_OBJECT
public:
    explicit tcpclient(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TCPCLIENT_H
