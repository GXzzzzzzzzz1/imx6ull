#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <QTimer>

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

    //初始化人脸检测模块
    facedetector_working = false;
    facedetector_ptr = new face_detector();
    facedetector_thread = new QThread();
    facedetector_ptr->moveToThread(facedetector_thread);
    facedetector_thread->start();
    connect(this,&MainWindow::launchfacedetector_signals_mainwindows,facedetector_ptr,&face_detector::facedector_working);
    connect(facedetector_ptr,&face_detector::check_result_signals_face_detector,this,&MainWindow::get_facedetector_data);

    //初始化人脸识别模块
    faceidentify_ptr = new face_identify();
    faceidentify_thread = new QThread();
    faceidentify_ptr->moveToThread(faceidentify_thread);
    faceidentify_thread->start();
    connect(this,&MainWindow::launchfaceidentify_register_signals_mainwindows,faceidentify_ptr,&face_identify::face_register);
    connect(this,&MainWindow::launchfaceidentify_signals_mainwindows,faceidentify_ptr,&face_identify::face_query);
    connect(faceidentify_ptr,&face_identify::registerfaceid_signals_faceidentify,this,&MainWindow::get_faceidentify_register_faceid_data);
    connect(faceidentify_ptr,&face_identify::identifyfaceid_signals_faceidentify,this,&MainWindow::get_faceidentify_faceid_data);
    //画面刷新
    refreash_ui_timer = new QTimer();
    refreash_ui_timer->setInterval(1);
    refreash_ui_timer->setSingleShot(true);
    refreash_ui_timer->start(16);
    connect(refreash_ui_timer,&QTimer::timeout,this,&MainWindow::refreash_ui_timeout);

    //网络模块
    tcpserver_ptr = new tcp_server();
    tcpserver_ptr->listen(QHostAddress("192.168.5.11"),8899);
    connect(tcpserver_ptr,&tcp_server::cameradata_signals_camera,this,&MainWindow::get_camera_data);
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
        refreash_ui_timer->start(16);
        ui->opencamera_btn->setText("停止摄像机");
        ui->opencamera_btn->setEnabled(true);
    }
    else if(result == "相机启动失败"|| result == "相机运行结束" || result == "相机运行异常")
    {
        //refreash_ui_timer->stop();
        ui->opencamera_btn->setText("启动摄像机");
        ui->opencamera_btn->setEnabled(true);
    }else{
        qDebug()<<"主线程接收到未知结果，来自相机模块。结果："<<result;
    }
}

void MainWindow::get_camera_data(cv::Mat image)
{
    bgr_image = image.clone();

    //人脸检测模块未运行，可发送
    if(facedetector_working == false)
    {
        facedetector_working = true;
        emit launchfacedetector_signals_mainwindows(bgr_image);
    }

    //人脸识别模块未运行，可发送
    if(faceidentify_working == false)
    {
        faceidentify_working = true;
        emit launchfaceidentify_signals_mainwindows(bgr_image);
    }
}

void MainWindow::get_facedetector_data(const std::vector<cv::Rect> &faces)
{
    //qDebug()<<"人脸数量:"<<faces.size();
    if (!faces.empty())
    {
        // 获取第一个检测到的人脸
        const cv::Rect& face = faces[0];
//        qDebug() << "第一个人脸:";
//        qDebug() << "左上角坐标: (" << face.x << "," << face.y << ")";
//        qDebug() << "宽度: " << face.width;
//        qDebug() << "高度: " << face.height;
//        qDebug() << "右下角坐标: (" << face.br().x << "," << face.br().y << ")";
//        qDebug() << "面积: " << face.area();
//        qDebug() << "中心坐标: (" << face.x + face.width / 2 << "," << face.y + face.height / 2 << ")";
//        qDebug() << "长宽比: " << static_cast<double>(face.width) / face.height;
        facedector_x = face.x;
        facedector_y = face.y;
        facedector_width = face.width;
        facedector_height = face.height;
    }else{
        facedector_x = 0;
        facedector_y = 0;
        facedector_width = 0;
        facedector_height = 0;
    }
    facedetector_working = false;
}

void MainWindow::refreash_ui_timeout()
{
    refreash_ui_timer->stop();
    if(bgr_image.empty())
    {
        refreash_ui_timer->start(16);
        return;
    }

    //mat -> qimage
    cv::Mat rgbImage;
    cv::cvtColor(bgr_image,rgbImage,cv::COLOR_BGR2RGB);
    QImage Qimage(rgbImage.data,rgbImage.cols,rgbImage.rows,rgbImage.step1(),QImage::Format_RGB888);

    //框出人脸
    QPainter painter(&Qimage);
    painter.setPen(QPen(Qt::red, 2));  // 设置画笔颜色和宽度
    if(facedector_width != 0 || facedector_height != 0)
    painter.drawRect(facedector_x, facedector_y, facedector_width, facedector_height);

    //显示识别出名称
    if(!faceidentify_name.isEmpty())
    {
        QFont font("Arial", 24, QFont::Bold); // 创建一个字体对象,设置字体为Arial,大小为16,加粗
        painter.setFont(font); // 设置QPainter使用该字体
        painter.setPen(QPen(Qt::white, 1)); // 设置文本颜色为白色,宽度为2像素
        QRect nameRect(facedector_x, facedector_y - 20, facedector_width, 20); // 名称区域的坐标和大小
        painter.setPen(QPen(Qt::white, 1)); // 设置文本颜色和宽度
        painter.drawText(nameRect, Qt::AlignCenter, faceidentify_name); // 在名称区域内绘制文本
    }
    //显示照片
    QPixmap pixmap = QPixmap::fromImage(Qimage);
    ui->camera_label->setPixmap(pixmap);
    refreash_ui_timer->start(16);
}

void MainWindow::on_take_photograph_btn_clicked()
{
    photograph_image = bgr_image.clone();
    if(photograph_image.empty())
    {
        ui->tips_label->setText(QString("相机无法拍照"));
        return;
    }
    cv::Mat rgbImage;
    cv::cvtColor(photograph_image,rgbImage,cv::COLOR_BGR2RGB);
    QImage Qimage(rgbImage.data,rgbImage.cols,rgbImage.rows,rgbImage.step1(),QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(Qimage);
    ui->photograph_label->setPixmap(pixmap);
}

void MainWindow::on_register_btn_clicked()
{
    //判断名称是否为空，数据库中有无冲突
    QString name = ui->name_lineEdit->text();
    if(name.isEmpty())
    {
        ui->tips_label->setText(QString("请输入名称"));
        return;
    }

    if(face_sqlite_db.check_name_exist(name) == true)
    {
        ui->tips_label->setText(QString("数据库已存在:" + name));
        return;
    }

    //检查是否拍照
    if(photograph_image.empty())
    {
        ui->tips_label->setText(QString("请先拍照"));
        return;
    }

    ui->name_lineEdit->setEnabled(false);
    ui->tips_label->setText(QString("正在识别人脸..."));
    emit launchfaceidentify_register_signals_mainwindows(photograph_image);

}

void MainWindow::get_faceidentify_register_faceid_data(int faceid)
{
    if(faceid < 0)
    {
       ui->tips_label->setText("注册人脸失败");
       ui->name_lineEdit->setEnabled(true);
       return;
    }

    //判断名称是否为空，数据库中有无冲突
    QString name = ui->name_lineEdit->text();
    if(!face_sqlite_db.insert_name_faceid(name,faceid))
    {
        ui->tips_label->setText("seetaface注册人脸成功，数据库插入失败");
        ui->name_lineEdit->setEnabled(true);
        return;
    }

    ui->tips_label->setText(QString("用户:%1注册成功faceid:%2").arg(name).arg(faceid));
    ui->name_lineEdit->setEnabled(true);
}

void MainWindow::get_faceidentify_faceid_data(int faceid)
{
    if(faceid < 0)
    {
        //识别不出
        faceidentify_working = false;
        faceidentify_name.clear();
        return;
    }

    //检查faceid是否存在于数据库中
    if(!face_sqlite_db.check_faceid_exist(faceid))
    {
        qDebug()<<"识别出人脸但是不存在于数据库中？是否需要重新注册？";
        faceidentify_working = false;
        faceidentify_name.clear();
        return;
    }

    //通过faceid找到name
    faceidentify_name = face_sqlite_db.get_name_by_faceid(faceid);
    faceidentify_working = false;
}
