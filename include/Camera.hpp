#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <linux/v4l2-controls.h>
#include <linux/videodev2.h>

#include <string>

namespace hmwk {

struct FrameBuffer {
    unsigned int index;
    size_t length;
    void* start;
};

class Camera {
public:
    Camera(std::string devName, unsigned int width, unsigned int height);
    // static void scanAllCamera();
    ~Camera();

    void startStream();
    FrameBuffer* dqueueBuf();
    FrameBuffer* dqueueBufWithFlush();
    void queueBuf(FrameBuffer* buffer);
    void stopStream();

    void printCapability() const;
    void printFormat() const;

private:
    int fd_;
    std::string devName_;

    struct v4l2_capability cap_;
    struct v4l2_format fmt_;

    int bufferNum_;
    struct FrameBuffer* buffers_;

private:
    void getCapability();
    bool setFormat(unsigned int width, unsigned int height);
    bool requestBuf();
    bool mmapBuf();
    void queueAllBuf();
    bool munmapBuf();
    void flushAllBuf();
};

}  // namespace hmwk

#endif