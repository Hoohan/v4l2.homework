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

### 编译准备

安装基础软件包：

```bash
sudo apt install build-essential cmake libgtkmm-3.0-dev 
```

若使用`git clone`获取代码，则需安装git，此处不再赘述。

### 构建与运行

配置与构建，请在项目根目录下执行如下命令：

```bash
cmake -S . -B build && cmake --build build
```

构建完毕的程序将生成在`bin/`目录下，使用以下命令运行，查看效果：

```bash
./bin/grabber
```

## 常见问题

1. 摄像头无法获取数据

    摄像头正常链接但无法获取数据，一般出现在虚拟机链接宿主机摄像头的情况下。且此种情况尝试使用GNOME Cheese (a.k.a. Ubuntu “茄子”) 打开摄像头也会失败。请在虚拟机设置中调整USB控制器版本。若为2.0，请调整为3.0；若为3.0，请调整为2.0。

2. 短时间连续多次运行程序抓取影像，大多工作正常，但偶见select timeout

    请优先按照问题1的方式进行调整，若无效，则问题可能与摄像头的硬件带宽有关，请更换摄像头。

## 参考文档

1. [Linux Media Infrastructure userspace API - Part I - Video for Linux API](https://linuxtv.org/downloads/v4l-dvb-apis-new/userspace-api/v4l/v4l2.html)
2. [gtkmm - C++ Interfaces for GTK and GNOME](https://www.gtkmm.org/en/index.html)
3. [Programming with gtkmm 3](https://developer-old.gnome.org/gtkmm-tutorial/3.24/index.html.en)
4. [Using CMake to build gtkmm programs](https://wiki.gnome.org/Projects/gtkmm/UsingCMake)
