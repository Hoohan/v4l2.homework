#include <fcntl.h>
#include <libv4l2.h>
#include <linux/videodev2.h>

#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    string devName = "/dev/video0";

    //open the device
    int fd = -1;
    fd = v4l2_open(devName.c_str(), O_RDWR | O_NONBLOCK, 0);
    if(fd< 0){
        perror("Can not open device");
    }

    //show capability
    struct v4l2_capability cap;
    int ret = v4l2_ioctl(fd, VIDIOC_QUERYCAP, &cap);
    if(ret < 0){
        perror("require capability failed");
        return -1;
    }
    printf("driver:\t\t%s\n",cap.driver);
    printf("card:\t\t%s\n",cap.card);
    printf("bus_info:\t%s\n",cap.bus_info);
    printf("version:\t%d\n",cap.version);
    printf("capabilities:\t%x\n",cap.capabilities);  
    if((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE)
    {
        printf("Device %s: supports capture.\n",devName.c_str());
    }
    if((cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING)
    {
        printf("Device %s: supports streaming.\n",devName.c_str());
    }

    //set format
    struct v4l2_format fmt;
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = 640;
    fmt.fmt.pix.height      = 480;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    ret = v4l2_ioctl(fd, VIDIOC_S_FMT, &fmt);
    if(ret < 0){
        perror("set format failed!");
        return -1;
    }
    if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_RGB24) {
                printf("Libv4l didn't accept RGB24 format. Can't proceed.\n");
                exit(EXIT_FAILURE);
    }
    if ((fmt.fmt.pix.width != 640) || (fmt.fmt.pix.height != 480)){
        printf("Warning: driver is sending image at %dx%d\n", fmt.fmt.pix.width, fmt.fmt.pix.height);
    }
    printf("fmt.type:\t\t%d\n",fmt.type);
    printf("pix.pixelformat:\t%c%c%c%c\n", \
            fmt.fmt.pix.pixelformat & 0xFF,\
            (fmt.fmt.pix.pixelformat >> 8) & 0xFF, \
            (fmt.fmt.pix.pixelformat >> 16) & 0xFF,\
            (fmt.fmt.pix.pixelformat >> 24) & 0xFF);
    printf("pix.width:\t\t%d\n",fmt.fmt.pix.width);
    printf("pix.height:\t\t%d\n",fmt.fmt.pix.height);
    printf("pix.field:\t\t%d\n",fmt.fmt.pix.field);

    // request frame buf
    struct v4l2_requestbuffers req;
    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    v4l2_ioctl(fd, VIDIOC_REQBUFS, &req);
    if(req.count < 2){
        perror("buffer memory is Insufficient! \n");
        return -1;
    }

    // close device
    v4l2_close(fd);
    return 0;
}
