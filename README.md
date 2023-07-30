# v4l2.homework

使用C++实现从相机读取图片到PC，使用v4l2的函数实现，显示图片在一个窗口上。

## 设计思路

实现上述功能，一般分以下三步：

- 通过v4l2 API完成摄像头初始化
- 取出一帧YUV影像，并将其转换为RGB格式
- 将RGB数据交由UI框架GTK完成绘制及显示

## 构建指导

### 环境需求

- Ubuntu 20.04 LTS
- GCC / CMake
- gtkmm-3.0(libgtkmm-3.0-dev)
- USB Camera

其中，USB摄像头需支持640*480分辨率下的YUV 4:2:2 (V4L2_PIX_FMT_YUYV)格式输出。

### 编译准备

安装基础软件包：

```bash
sudo apt install build-essential cmake libgtkmm-3.0-dev 
```

若使用`git clone`获取代码，则需安装git，此处不再赘述。

### 构建

配置与构建，请在项目根目录下执行如下命令：

```bash
cmake -S . -B build && cmake --build build
```

### 使用方式

构建完毕的程序将生成在`bin/`目录下。通过传递参数方式使用，使用方式有两种：

#### 1 扫描设备

使用`-s`参数，程序将会列出所有可用相机设备：

```bash
./bin/grabber -s
```

输出示例：

```
[XiaoMi USB 2.0 Webcam: XiaoMi U] at [usb-0000:00:14.0-7] is attached to:
	/dev/video0
	/dev/video1
```

#### 2 抓取特定设备影像

使用`-d [device path]`参数，程序将弹出图像抓取窗口：

```bash
./bin/grabber -d [your-camera-path]
```

点击抓取影像按钮，即可在新窗口中查看抓取的影像。

#### 注意事项

1. `-s`与`-d [device path]`参数不可同时使用。若同时使用，`-s`将会覆盖`-d [device path]`。
2. 无参数或者传入非法参数，将会显示用法，如下所示。

```bash
Usage:
    -s Scan all cameras
    -d [device path] Open camera and capture
```

## 常见问题

1. **多次抓取存在“延迟”，或第一次抓取无图像**

    经多次试验，v4l2视频采集队列“写满即停”，“延迟”的根源为抓取到了采集队列中的旧数据。为解决此问题，同时避免反复开关设备造成效率浪费，每次抓取图像前，程序都会刷新采集队列里的所有缓冲区。

    若第一次抓取无图像，请等待几秒钟后再次抓取。出现该现象的原因是摄像头启动较慢，需要一定时间图像才传输至系统。

    若还是存在问题，请检查摄像头链接。

2. **摄像头无法获取数据**

    摄像头正常链接但无法获取数据，一般出现在虚拟机链接宿主机摄像头的情况下。且此种情况尝试使用GNOME Cheese (a.k.a. Ubuntu “茄子”) 打开摄像头也会失败。请在虚拟机设置中调整USB控制器兼容性。若为2.0，请调整为3.0；若为3.0，请调整为2.0。

3. **短时间连续多次运行程序抓取影像，部分抓取正常，部分抓取出现select timeout**

    请优先按照问题2的方式进行调整，若无效，则问题可能与摄像头的硬件带宽有关。快速、多次开关设备可能会造成此问题，并且该问题容易在性能较低的摄像头设备上复现。如果需要连续多次抓取影像，请使用Control Window下的抓取按钮。
    
    以下两个链接可能会有帮助：

    1. [Error with USB webcam - v4l2: oops: select timeout](https://forums.raspberrypi.com/viewtopic.php?t=35184)
    2. [OpenCV v4l2 select timeout error SOLVED! High FPS!](https://forums.raspberrypi.com/viewtopic.php?t=35689)

## 参考文档

1. [Linux Media Infrastructure userspace API - Part I - Video for Linux API](https://linuxtv.org/downloads/v4l-dvb-apis-new/userspace-api/v4l/v4l2.html)
2. [gtkmm - C++ Interfaces for GTK and GNOME](https://www.gtkmm.org/en/index.html)
3. [Programming with gtkmm 3](https://developer-old.gnome.org/gtkmm-tutorial/3.24/index.html.en)
4. [Using CMake to build gtkmm programs](https://wiki.gnome.org/Projects/gtkmm/UsingCMake)
5. [Correct YUV422 to RGB conversion](https://stackoverflow.com/questions/8042563/correct-yuv422-to-rgb-conversion)