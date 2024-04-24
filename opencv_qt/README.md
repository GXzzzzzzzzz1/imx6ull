# opencv_qt

基于opencv和qt实现用户登录系统。



### 环境搭建

```
#opencv下载地址
[https://github.com/opencv/opencv/tree/3.4.1]
#opencv_contrib下载地址
[https://github.com/opencv/opencv_contrib/tree/3.4.1]

#opencv、opencv_contrib编译教程
[https://blog.csdn.net/qq_26943851/article/details/104434130]
[https://blog.csdn.net/ezhchai/article/details/80557936]

#注意说明
我是根据正点原子的教程来编译的，后面发现需要添加opencv_contrib库，所以在cmake-gui中额外添加了OPENCV_EXTRA_MODULES_PATH指定opencv_contrib源码中module的地址，重新编译

#编译出现的问题
cuba.hpp头文件丢失
[https://blog.csdn.net/u012321968/article/details/106654012]
boostdesc_bgm.i文件缺失
[https://blog.csdn.net/u011736771/article/details/85960300]

#qt环境
直接安装qt即可。
安装位于/opt/Qt5.12.8/5.12.8/gcc_64/bin/qmake
```

