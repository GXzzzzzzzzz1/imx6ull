# opencv_qt

```
服务端实现摄像头数据采集，人脸检测，人脸识别，用户注册人脸，数据库用于绑定faceid和名称，接收客户端数据，数据反馈给客户端还未实现
客户端实现摄像头数据采集，发送摄像头数据给服务端，接收服务端反馈还未实现
```



### 环境搭建

```
#opencv下载地址
[https://github.com/opencv/opencv/tree/3.4.1]
#opencv_contrib下载地址
[https://github.com/opencv/opencv_contrib/tree/3.4.1]

#opencv、opencv_contrib编译教程
[https://blog.csdn.net/qq_26943851/article/details/104434130]
[https://blog.csdn.net/ezhchai/article/details/80557936]

#seetaface
[https://github.com/seetafaceengine/SeetaFace2]

#注意说明
我是根据正点原子的教程来编译的，后面发现需要添加opencv_contrib库，所以在cmake-gui中额外添加了OPENCV_EXTRA_MODULES_PATH指定opencv_contrib源码中module的地址，重新编译
最终使用opencv检测人脸，seetaface2来识别人脸，没有使用到opencv_contrib

#编译出现的问题
cuba.hpp头文件丢失
[https://blog.csdn.net/u012321968/article/details/106654012]
boostdesc_bgm.i文件缺失
[https://blog.csdn.net/u011736771/article/details/85960300]

#qt环境
直接安装qt即可。
安装位于/opt/Qt5.12.8/5.12.8/gcc_64/bin/qmake
```



### 首次编译说明

```
client和server目录下，各有env_setting和make.sh
首次编译执行make.sh即可，编译前先查看env_setting
client目录中的env_setting是设置交叉编译工具，用于开发板运行
server目录中的env_setting是设置服务端在ubuntu上的运行环境
编译无法通过请查看qmake的目录是否正确，修改make.sh中的qmake目录，可写成绝对路径
```

