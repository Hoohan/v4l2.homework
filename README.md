# v4l2.homework

使用C++实现从相机读取图片到PC，使用v4l2的函数实现，显示图片在一个窗口上。

## 编译指南(todo)

### 环境需求

- Ubuntu 20.04 LTS
- GCC / CMake
- gtkmm-4.0
- USB Camera

### 编译准备

安装gtkmm-4.0

### 构建与运行

配置与构建：

```bash
cmake -S . -B build
cmake --build build
```

运行：

```bash
./bin/grabber
```

### 常见问题

(todo)

## 参考文档

- [Linux Media Infrastructure userspace API - Part I - Video for Linux API](https://linuxtv.org/downloads/v4l-dvb-apis-new/userspace-api/v4l/v4l2.html)
- [gtkmm - C++ Interfaces for GTK and GNOME](https://www.gtkmm.org/en/index.html)
- [Using CMake to build gtkmm programs](https://wiki.gnome.org/Projects/gtkmm/UsingCMake)

## 备注

Arch Linux:

```bash
sudo pacman -S gtkmm-4.0
```
