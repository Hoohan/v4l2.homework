#include <bits/types/struct_timeval.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <unistd.h>

#include <Camera.hpp>
#include <cstring>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

namespace hmwk {

static void ioctl_safe(int fd, int request, void* arg) {
    int ret;

    do {
        ret = ioctl(fd, request, arg);
    } while (ret == -1 && ((errno == EINTR) || (errno == EAGAIN)));

    if (ret == -1) {
        fprintf(stderr, "error %d, %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

Camera::Camera(std::string devName, unsigned int width, unsigned int height)
    : devName_(devName), bufferNum_(2) {
    // open device
    fd_ = open(devName.c_str(), O_RDWR | O_NONBLOCK, 0);
    if (fd_ < 0) {
        perror("Can not open device");
    }
    getCapability();
    setFormat(width, height);
    requestBuf();
    mmapBuf();
    queueAllBuf();
}

Camera::~Camera() {
    // close device
    close(fd_);
}

bool Camera::setFormat(unsigned int width, unsigned int height) {
    fmt_.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt_.fmt.pix.width = width;
    fmt_.fmt.pix.height = height;
    fmt_.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    // fmt_.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
    fmt_.fmt.pix.field = V4L2_FIELD_INTERLACED;

    ioctl_safe(fd_, VIDIOC_S_FMT, &fmt_);

    if (fmt_.fmt.pix.pixelformat != V4L2_PIX_FMT_YUYV) {
        printf("v4l didn't accept YUYV format. Can't proceed.\n");
        exit(EXIT_FAILURE);
    }
    if ((fmt_.fmt.pix.width != width) || (fmt_.fmt.pix.height != height)) {
        printf(
            "Warning: driver is sending image at %dx%d, not expected %dx%d\n",
            fmt_.fmt.pix.width, fmt_.fmt.pix.height, width, height);
        return false;
    }
    return true;
}

bool Camera::requestBuf() {
    struct v4l2_requestbuffers req;
    req.count = bufferNum_;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    ioctl_safe(fd_, VIDIOC_REQBUFS, &req);
    if (req.count < 2) {
        perror("buffer memory is Insufficient! \n");
        return false;
    }
    return true;
}

bool Camera::mmapBuf() {
    buffers_ =
        static_cast<FrameBuffer*>(calloc(bufferNum_, sizeof(FrameBuffer)));
    for (int i = 0; i < bufferNum_; ++i) {
        struct v4l2_buffer buf;
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        ioctl_safe(fd_, VIDIOC_QUERYBUF, &buf);

        buffers_[i].index = i;
        buffers_[i].length = buf.length;
        buffers_[i].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE,
                                 MAP_SHARED, fd_, buf.m.offset);

        if (buffers_[i].start == MAP_FAILED) {
            perror("v4l2 mmap");
            exit(EXIT_FAILURE);
        }
        // printf("Frame buffer %d: address = %p, length = %ld \n", i,
        //        (void*)buffers_[i].start, buffers_[i].length);
    }
    return true;
}

bool Camera::munmapBuf() {
    // unmap buffers
    for (int i = 0; i < bufferNum_; i++) {
        munmap(buffers_[i].start, buffers_[i].length);
    }

    // free buffers
    free(buffers_);
    return true;
}

void Camera::queueAllBuf() {
    for (int i = 0; i < bufferNum_; ++i) {
        struct v4l2_buffer buf;
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        ioctl_safe(fd_, VIDIOC_QBUF, &buf);
    }
    return;
}

void Camera::startStream() {
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl_safe(fd_, VIDIOC_STREAMON, &type);
}

void Camera::stopStream() {
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl_safe(fd_, VIDIOC_STREAMOFF, &type);
}

FrameBuffer* Camera::dqueueBuf() {
    fd_set fds;
    struct timeval tv;
    int ret;
    do {
        FD_ZERO(&fds);
        FD_SET(fd_, &fds);

        tv.tv_sec = 2;
        tv.tv_usec = 0;

        ret = select(fd_ + 1, &fds, NULL, NULL, &tv);
        if (ret == 0 && errno == 0) {
            printf("ERROR: Select timeout. Please check your camera.");
        }
    } while (ret == -1 && (errno == EINTR));
    // dq buffer
    struct v4l2_buffer buf;
    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    ioctl_safe(fd_, VIDIOC_DQBUF, &buf);
    return buffers_ + buf.index;
}

void Camera::queueBuf(FrameBuffer* buffer) {
    struct v4l2_buffer buf;
    CLEAR(buf);
    buf.index = buffer->index;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    ioctl_safe(fd_, VIDIOC_QBUF, &buf);
}

void Camera::getCapability() { ioctl_safe(fd_, VIDIOC_QUERYCAP, &cap_); }

void Camera::printCapability() const {
    printf("driver:\t\t%s\n", cap_.driver);
    printf("card:\t\t%s\n", cap_.card);
    printf("bus_info:\t%s\n", cap_.bus_info);
    printf("version:\t%d\n", cap_.version);
    printf("capabilities:\t%x\n", cap_.capabilities);
    if ((cap_.capabilities & V4L2_CAP_VIDEO_CAPTURE) ==
        V4L2_CAP_VIDEO_CAPTURE) {
        printf("Device %s: supports capture.\n", devName_.c_str());
    }
    if ((cap_.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING) {
        printf("Device %s: supports streaming.\n", devName_.c_str());
    }
}

void Camera::printFormat() const {
    printf("fmt.type:\t\t%d\n", fmt_.type);
    printf("pix.pixelformat:\t%c%c%c%c\n", fmt_.fmt.pix.pixelformat & 0xFF,
           (fmt_.fmt.pix.pixelformat >> 8) & 0xFF,
           (fmt_.fmt.pix.pixelformat >> 16) & 0xFF,
           (fmt_.fmt.pix.pixelformat >> 24) & 0xFF);
    printf("pix.width:\t\t%d\n", fmt_.fmt.pix.width);
    printf("pix.height:\t\t%d\n", fmt_.fmt.pix.height);
    printf("pix.field:\t\t%d\n", fmt_.fmt.pix.field);
}

}  // namespace hmwk