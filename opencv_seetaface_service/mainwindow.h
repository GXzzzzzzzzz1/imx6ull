#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "face_sqlite.h"
#include "camera.h"
#include "face_detector.h"
#include "face_identify.h"
#include "tcp_server.h"

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
    //通知人脸检测进行检测人脸
    void launchfacedetector_signals_mainwindows(cv::Mat faceimage);
    //通知人脸识别进行注册人脸
    void launchfaceidentify_register_signals_mainwindows(cv::Mat faceimage);
    //通知人脸识别进行人脸识别
    void launchfaceidentify_signals_mainwindows(cv::Mat faceimage);
private slots:
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
    //处理人脸检测返回结果
    void get_facedetector_data(const std::vector<cv::Rect>& faces);
    //录入人脸按钮槽函数
    void on_register_btn_clicked();
    //处理人脸识别注册返回的人脸id
    void get_faceidentify_register_faceid_data(int faceid);
    //处理人脸识别返回的人脸id
    void get_faceidentify_faceid_data(int faceid);
    //刷新界面
    void refreash_ui_timeout();
    //拍照
    void on_take_photograph_btn_clicked();

private:
    Ui::MainWindow *ui;

    //相机模块
    cv::Mat bgr_image;
    cv::Mat photograph_image;
    camera *camera_ptr;
    QThread* camera_thread;
    //人脸检测模块
    bool facedetector_working = false;
    int facedector_x,facedector_y,facedector_width,facedector_height = 0;
    face_detector *facedetector_ptr;
    QThread* facedetector_thread;
    //人脸识别模块
    QString faceidentify_name;
    bool faceidentify_working = false;
    face_identify *faceidentify_ptr;
    QThread *faceidentify_thread;
    //画面刷新
    QTimer *refreash_ui_timer;
    //网络模块
    tcp_server *tcpserver_ptr;
    //数据库
    face_sqlite face_sqlite_db;
};
#endif // MAINWINDOW_H
