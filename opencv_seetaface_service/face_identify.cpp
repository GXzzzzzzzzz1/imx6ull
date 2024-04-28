#include "face_identify.h"
#include <QDebug>

face_identify::face_identify(QObject *parent) : QObject(parent)
{
    seeta::ModelSetting FDmode("./seetaface/fd_2_00.dat",seeta::ModelSetting::CPU,0);
    seeta::ModelSetting PDmode("./seetaface/pd_2_00_pts5.dat",seeta::ModelSetting::CPU,0);
    seeta::ModelSetting FRmode("./seetaface/fr_2_10.dat",seeta::ModelSetting::CPU,0);
    fengineptr = new seeta::FaceEngine(FDmode,PDmode,FRmode);
    fengineptr->Load("./db/seetaface.db");
}

void face_identify::face_register(Mat faceimage)
{
    cv::Mat image = faceimage.clone();
    if(image.empty())
    {
        qDebug()<<"注册失败，人脸数据为空";
        return ;
    }

    //mat -> seetaimage
    SeetaImageData simage;
    simage.data = image.data;
    simage.width = image.cols;
    simage.height = image.rows;
    simage.channels = image.channels();
    float similarity = 0.0;
    int64_t faceid = fengineptr->Query(simage,&similarity);
    if(faceid >= 0 && similarity > 0.8)
    {
        qDebug()<<"注册失败，已经存在相同人脸faceid:"<<faceid<<"可信度:"<<similarity;
        emit registerfaceid_signals_faceidentify(-1);
        return;
    }else{
        qDebug()<<"不存在相同人脸，开始注册";
    }
    faceid = fengineptr->Register(simage);//return face id
    if(faceid >= 0)
    {
        qDebug()<<"注册成功，人脸id:"<<faceid;
        fengineptr->Save("./db/seetaface.db");
        emit registerfaceid_signals_faceidentify(faceid);
        return;
    }
    else
    {
        qDebug()<<"注册失败faceid < 0";
    }
    emit registerfaceid_signals_faceidentify(-1);
}

void face_identify::face_query(Mat faceimage)
{
    cv::Mat image = faceimage.clone();
    SeetaImageData simage;
    simage.data = image.data;
    simage.width = image.cols;
    simage.height = image.rows;
    simage.channels = image.channels();
    float similarity = 0;
    int64_t faceid = fengineptr->Query(simage,&similarity);
    if(faceid >= 0 && similarity > 0.8)
        emit identifyfaceid_signals_faceidentify(faceid);
    else
        emit identifyfaceid_signals_faceidentify(-1);

    return ;
}
