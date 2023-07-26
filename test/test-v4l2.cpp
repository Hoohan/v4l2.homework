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
    printf("driver:%s\n",cap.driver);
    printf("card:%s\n",cap.card);
    printf("bus_info:%s\n",cap.bus_info);
    printf("version:%d\n",cap.version);
    printf("capabilities:%x\n",cap.capabilities);  
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
    

    v4l2_close(fd);
    return 0;
}
