# v4l2.homework

使用C++实现从相机读取图片到PC，使用v4l2的函数实现，显示图片在一个窗口上。

## 环境需求(todo)

- Ubuntu / Arch Linux
- GCC / CMake
- libv4l2
- USB Camera

## 使用说明

请确保运行环境已安装v4l-utils/gtkmm-4.0

Arch Linux执行如下命令：

```bash
sudo pacman -S gtkmm-4.0 libv4l
```

配置与构建：

```bash
cmake -S . -B build
cmake --build build
```

运行：

```bash
./bin/grabber
```

## 其他

本项目开发时使用clangd作为LSP，请使用如下命令生成clangd可直接读取的构建配置，否则clangd语法分析将出错。

```
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1
```