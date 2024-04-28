#include "camera.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <QDebug>


camera::camera(QObject *parent) : QObject(parent)
{

}

void camera::getcameranumber()
{
    int camera_number_max = 0;
    while(1)
    {
        std::string devicePath = "/dev/video" + std::to_string(camera_number_max);
        int fd = open(devicePath.c_str(), O_RDWR);
        if (fd != -1) {
          // 设备成功打开
          close(fd);
          camera_number_max++;
          continue;
        } else {
          // 设备无法打开
          break;
        }
    }
    emit cameranumber_signals_camera(camera_number_max);
}

void camera::cameraworking(int camera_index)
{
    //初始化数据
    forcestop = false;
    //打开摄像头
    VideoCapture capture = VideoCapture(camera_index);
    if(!capture.isOpened())
    {
        qDebug()<<"Camera open failed,camera index:"<<camera_index;
        emit results_signals_camera("相机启动失败");
        return;
    }

    //设置摄像头参数
    capture.set(CAP_PROP_FRAME_WIDTH, 640);
    capture.set(CAP_PROP_FRAME_HEIGHT, 480);
    capture.set(CAP_PROP_FPS, 30);
    capture.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));


    //读取数据
    emit results_signals_camera("相机启动成功");
    cv::Mat image;
    while(!forcestop)
    {
        capture >> image;
        if (!image.empty()) {
            emit cameradata_signals_camera(image);
        }else{
            capture.release();
            emit results_signals_camera("相机运行异常");
            return;
        }
        usleep(16000);
    }
    capture.release();
    emit results_signals_camera("相机运行结束");
}
