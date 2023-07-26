#include <errno.h>
#include <fcntl.h>
#include <libv4l2.h>
#include <linux/v4l2-controls.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <sys/select.h>

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

using namespace std;

struct buffer {
    void* start;
    size_t length;
};

inline void v4l2_ioctl_safe(int fd, int request, void* arg) {
    int ret;

    do {
        ret = v4l2_ioctl(fd, request, arg);
    } while (ret == -1 && ((errno == EINTR) || (errno == EAGAIN)));

    if (ret == -1) {
        fprintf(stderr, "error %d, %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv) {
    string devName = "/dev/video0";

    // open the device
    int fd = -1;
    fd = v4l2_open(devName.c_str(), O_RDWR | O_NONBLOCK, 0);
    if (fd < 0) {
        perror("Can not open device");
    }

    cout << "fd: " << fd << endl;

    // show capability
    struct v4l2_capability cap;
    v4l2_ioctl_safe(fd, VIDIOC_QUERYCAP, &cap);

    printf("driver:\t\t%s\n", cap.driver);
    printf("card:\t\t%s\n", cap.card);
    printf("bus_info:\t%s\n", cap.bus_info);
    printf("version:\t%d\n", cap.version);
    printf("capabilities:\t%x\n", cap.capabilities);
    if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE) {
        printf("Device %s: supports capture.\n", devName.c_str());
    }
    if ((cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING) {
        printf("Device %s: supports streaming.\n", devName.c_str());
    }

    // set format
    struct v4l2_format fmt;
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = 640;
    fmt.fmt.pix.height = 480;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
    v4l2_ioctl_safe(fd, VIDIOC_S_FMT, &fmt);
    if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_RGB24) {
        printf("Libv4l didn't accept RGB24 format. Can't proceed.\n");
        exit(EXIT_FAILURE);
    }
    if ((fmt.fmt.pix.width != 640) || (fmt.fmt.pix.height != 480)) {
        printf("Warning: driver is sending image at %dx%d\n", fmt.fmt.pix.width,
               fmt.fmt.pix.height);
    }
    printf("fmt.type:\t\t%d\n", fmt.type);
    printf("pix.pixelformat:\t%c%c%c%c\n", fmt.fmt.pix.pixelformat & 0xFF,
           (fmt.fmt.pix.pixelformat >> 8) & 0xFF,
           (fmt.fmt.pix.pixelformat >> 16) & 0xFF,
           (fmt.fmt.pix.pixelformat >> 24) & 0xFF);
    printf("pix.width:\t\t%d\n", fmt.fmt.pix.width);
    printf("pix.height:\t\t%d\n", fmt.fmt.pix.height);
    printf("pix.field:\t\t%d\n", fmt.fmt.pix.field);

    // request frame buf
    struct v4l2_requestbuffers req;
    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    v4l2_ioctl_safe(fd, VIDIOC_REQBUFS, &req);
    if (req.count < 2) {
        perror("buffer memory is Insufficient! \n");
        return -1;
    }

    // map to user mode
    struct buffer* buffers;
    buffers = static_cast<buffer*>(calloc(req.count, sizeof(buffer)));
    int bufferNum;
    for (bufferNum = 0; bufferNum < req.count; ++bufferNum) {
        struct v4l2_buffer buf;
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = bufferNum;
        v4l2_ioctl_safe(fd, VIDIOC_QUERYBUF, &buf);
        buffers[bufferNum].length = buf.length;
        buffers[bufferNum].start =
            v4l2_mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
                      buf.m.offset);

        if (buffers[bufferNum].start == MAP_FAILED) {
            perror("v4l2_mmap");
            exit(EXIT_FAILURE);
        }
        printf("Frame buffer %d: address = %p, length = %ld \n", bufferNum,
               (void*)buffers[bufferNum].start, buffers[bufferNum].length);
    }

    // exchange buffer with the driver
    for (int i = 0; i < bufferNum; ++i) {
        struct v4l2_buffer buf;
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        v4l2_ioctl_safe(fd, VIDIOC_QBUF, &buf);
    }

    // stream on
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_ioctl_safe(fd, VIDIOC_STREAMON, &type);

    // begin capture
    fd_set fds;
    struct timeval tv;
    int ret;
    for (int i = 0; i < 20; i++) {
        do {
            FD_ZERO(&fds);
            FD_SET(fd, &fds);

            tv.tv_sec = 2;
            tv.tv_usec = 0;

            ret = select(fd + 1, &fds, NULL, NULL, &tv);
        } while (ret == -1 && (errno == EINTR));

        // dq buffer
        struct v4l2_buffer buf;
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        v4l2_ioctl_safe(fd, VIDIOC_DQBUF, &buf);

        // do some thing
        printf("buf.index: \t%u\n", buf.index);

        // q buffer
        v4l2_ioctl_safe(fd, VIDIOC_QBUF, &buf);
    }

    // stream off
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_ioctl_safe(fd, VIDIOC_STREAMOFF, &type);

    // unmap buffers
    for (int i = 0; i < bufferNum; i++) {
        v4l2_munmap(buffers[i].start, buffers[i].length);
    }

    // free buffers
    free(buffers);

    // close device
    v4l2_close(fd);

    return 0;
}
