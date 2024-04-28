#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化照相机模块
    camera_ptr = new camera();
    camera_thread = new QThread();
    camera_ptr->moveToThread(camera_thread);
    camera_thread->start();
    connect(camera_ptr,&camera::cameranumber_signals_camera,this,&MainWindow::updatecameralist);
    connect(this,&MainWindow::getcameralist_signals_mainwindows,camera_ptr,&camera::getcameranumber);
    connect(this,&MainWindow::launchcamera_signals_mainwindows,camera_ptr,&camera::cameraworking);
    connect(camera_ptr,&camera::results_signals_camera,this,&MainWindow::camera_result);
    connect(camera_ptr,&camera::cameradata_signals_camera,this,&MainWindow::get_camera_data);

    //画面刷新
    refreash_ui_timer = new QTimer();
    refreash_ui_timer->setInterval(1);
    refreash_ui_timer->setSingleShot(true);
    refreash_ui_timer->stop();
    connect(refreash_ui_timer,&QTimer::timeout,this,&MainWindow::refreash_ui_timeout);


    //网络模块
    client_ptr = new tcpclient();
    client_thread = new QThread();
    client_ptr->moveToThread(client_thread);
    client_thread->start();
    ui->ip->setText("192.168.5.11");
    ui->port->setText("8899");
    ui->disconnect->setDisabled(true);
    connect(this,&MainWindow::connectToServer,client_ptr,&tcpclient::connectToServer);//建立连接
    connect(this,&MainWindow::disconnectToServer,client_ptr,&tcpclient::disconnectToServer);//断开连接
    connect(this,&MainWindow::sendimageToServer,client_ptr,&tcpclient::sendimageToServer);
    connect(client_ptr,&tcpclient::notify_mainwindows_cmd_signals_tcpclient,this,&MainWindow::tcpclient_Result);
    //连接成功提示
    connect(client_ptr,&QTcpSocket::connected,this,[=](){
        ui->tips_label->setText("连接上服务器");
        ui->disconnect->setDisabled(false);
        ui->connect->setDisabled(true);
    });
    //断开连接提示
    connect(client_ptr,&QTcpSocket::disconnected,this,[=](){
        ui->tips_label->setText("服务器断开连接");
        ui->disconnect->setDisabled(true);
        ui->connect->setDisabled(false);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updatecameralist(int number)
{
    ui->cameralist_cbox->clear();
    ui->tips_label->setText(QString("摄像头数量:%1").arg(number));
    if(number == 0)
    {
        return;
    }

    for(int i=0;i<number;i++)
    {
        ui->cameralist_cbox->addItem(QString("%1").arg(i));
    }

}


void MainWindow::on_opencamera_btn_clicked()
{
    ui->opencamera_btn->setEnabled(false);
    if(ui->opencamera_btn->text() == "启动摄像机")
    {
        QString str = ui->cameralist_cbox->currentText();
        if(str.isEmpty())
        {
            ui->tips_label->setText(QString("当前摄像头数量为空，请刷新摄像机！"));
            return;
        }
        emit launchcamera_signals_mainwindows(str.toInt());
    }
    else if(ui->opencamera_btn->text() == "停止摄像机")
    {
        camera_ptr->forcestop = true;//注意，此处直接跨线程访问
    }
    else
    {
        ui->tips_label->setText("启动/停止摄像机按钮未知状态");
    }

}


void MainWindow::on_updatecameralist_btn_clicked()
{
    if(ui->opencamera_btn->isEnabled()&&ui->opencamera_btn->text() == "启动摄像机")
    {
        ui->tips_label->setText(QString("正在刷新摄像机个数..."));
        emit getcameralist_signals_mainwindows();
    }
    else
    {
        ui->tips_label->setText(QString("相机目前在运行，请不要点击刷新按钮"));
    }
}

void MainWindow::camera_result(QString result)
{
    ui->tips_label->setText(result);
    if(result.isEmpty())
        return;

    if(result == "相机启动成功")
    {
        refreash_ui_timer->start(10);
        ui->opencamera_btn->setText("停止摄像机");
        ui->opencamera_btn->setEnabled(true);
    }
    else if(result == "相机启动失败"|| result == "相机运行结束" || result == "相机运行异常")
    {
        refreash_ui_timer->stop();
        ui->opencamera_btn->setText("启动摄像机");
        ui->opencamera_btn->setEnabled(true);
    }else{
        qDebug()<<"主线程接收到未知结果，来自相机模块。结果："<<result;
    }
}

void MainWindow::get_camera_data(cv::Mat image)
{
    bgr_image = image.clone();
    if(bgr_image.empty())
        return;

    //发送给服务器
    if(tcp_working == false)
    {
        tcp_working = true;
        emit sendimageToServer(bgr_image);
    }
}


void MainWindow::refreash_ui_timeout()
{
    refreash_ui_timer->stop();
    if(bgr_image.empty())
    {
        refreash_ui_timer->start(5);
        return;
    }
    //mat -> qimage
    cv::Mat rgbImage;
    cv::cvtColor(bgr_image,rgbImage,cv::COLOR_BGR2RGB);
    QImage Qimage(rgbImage.data,rgbImage.cols,rgbImage.rows,rgbImage.step1(),QImage::Format_RGB888);

    //框出人脸
//    QPainter painter(&Qimage);
//    painter.setPen(QPen(Qt::red, 2));  // 设置画笔颜色和宽度
//    if(facedector_width != 0 || facedector_height != 0)
//    painter.drawRect(facedector_x, facedector_y, facedector_width, facedector_height);

    //显示识别出名称
//    if(!faceidentify_name.isEmpty())
//    {
//        QFont font("Arial", 24, QFont::Bold); // 创建一个字体对象,设置字体为Arial,大小为16,加粗
//        painter.setFont(font); // 设置QPainter使用该字体
//        painter.setPen(QPen(Qt::white, 1)); // 设置文本颜色为白色,宽度为2像素
//        QRect nameRect(facedector_x, facedector_y - 20, facedector_width, 20); // 名称区域的坐标和大小
//        painter.setPen(QPen(Qt::white, 1)); // 设置文本颜色和宽度
//        painter.drawText(nameRect, Qt::AlignCenter, faceidentify_name); // 在名称区域内绘制文本
//    }
    //显示照片
    QPixmap pixmap = QPixmap::fromImage(Qimage);
    ui->camera_label->setPixmap(pixmap);
    refreash_ui_timer->start(5);
}

void MainWindow::on_connect_clicked()
{
    QString ip = ui->ip->text();
    unsigned short port = ui->port->text().toUShort();
    emit connectToServer(ip,port);
}

void MainWindow::on_disconnect_clicked()
{
    emit disconnectToServer();
}

void MainWindow::tcpclient_Result(int cmd)
{
    if(cmd == 0)//发送结束
    {
        tcp_working = false;
    }
}
