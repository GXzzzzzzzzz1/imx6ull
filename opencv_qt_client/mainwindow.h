#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "camera.h"
#include "tcpclient.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    //获得摄像头数量
    void getcameralist_signals_mainwindows();
    //通知摄像头模块启动摄像头
    void launchcamera_signals_mainwindows(int number);
    //通知连接服务器
    void connectToServer(QString ip,unsigned short port);
    //通知断开服务器
    void disconnectToServer();
    //发送图像到服务器
    void sendimageToServer(cv::Mat image);
public slots:
    //更新摄像头数量
    void updatecameralist(int number);
    //启动摄像头按钮槽函数
    void on_opencamera_btn_clicked();
    //刷新摄像头数量槽函数
    void on_updatecameralist_btn_clicked();
    //处理摄像头模块返回结果
    void camera_result(QString result);
    //接收摄像头数据原始图片BGR
    void get_camera_data(cv::Mat image);
    //刷新界面
    void refreash_ui_timeout();
    //连接服务器
    void on_connect_clicked();
    //断开服务器
    void on_disconnect_clicked();
    //处理网络模块发送结果
    void tcpclient_Result(int cmd);
private:
    Ui::MainWindow *ui;
    //相机模块
    cv::Mat bgr_image;
    camera *camera_ptr;
    QThread* camera_thread;

    //网络模块
    bool tcp_working = false;
    tcpclient *client_ptr;
    QThread* client_thread;

    //画面刷新
    QTimer *refreash_ui_timer;
};
#endif // MAINWINDOW_H
