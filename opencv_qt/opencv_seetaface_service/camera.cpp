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
    //获取摄像头参数
#if 0
    int nFrameWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    int nFrameHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    double fFrameRate = capture.get(CV_CAP_PROP_FPS);
    int nBrightness = capture.get(CV_CAP_PROP_BRIGHTNESS);
    int nContrast = capture.get(CV_CAP_PROP_CONTRAST);
    int nSaturation = capture.get(CV_CAP_PROP_SATURATION);
    int nHue = capture.get(CV_CAP_PROP_HUE);
    int nExposure = capture.get(CV_CAP_PROP_EXPOSURE);
    double fFrameCount = capture.get(CV_CAP_PROP_FRAME_COUNT);//总帧数
    double fFramePos = capture.get(CV_CAP_PROP_POS_FRAMES);//视频帧号，0-based index of the frame to be decoded/captured next.
    double fTimestamp = capture.get(CV_CAP_PROP_POS_MSEC);//时间戳(毫秒)，position of the video file in milliseconds or video capture timestamp.
#endif
    //设置摄像机格式并且读出格式
    capture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    //cap->set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('Y', 'U', 'Y', 'V'));
    // 获取并打印摄像头的颜色格式
    double fourcc = capture.get(cv::CAP_PROP_FOURCC);
    if (fourcc == cv::VideoWriter::fourcc('M', 'J', 'P', 'G')) {
        qDebug() << "Camera is set to MJPEG format\n";
    } else if (fourcc == cv::VideoWriter::fourcc('Y', 'U', 'Y', 'V')) {
        qDebug() << "Camera is set to YUV422 format\n";
    } else {
        qDebug() << "Unknown camera format (FourCC code: " << fourcc << ")\n";
    }

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
