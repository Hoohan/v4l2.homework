/*
 * todo
 */

#include <ImageWindow.hpp>
#include <Camera.hpp>
#include <FormatConversion.hpp>

#include "gtkmm/application.h"

using namespace hmwk;

int main(int argc, char** argv) {
    std::string name = "/dev/video0";
    unsigned int width = 640, height = 480;
    Camera c(name, width, height);
    c.printCapability();
    c.printFormat();
    c.startStream();
    // sleep(1);
    FrameBuffer* buf = c.dqueueBuf();
    printf("buf.index: \t%u\n", buf->index);
    unsigned char* rgbBuf = static_cast<unsigned char*>(
        calloc(width * height * 3, sizeof(unsigned char)));
    yuyv2rgb(static_cast<unsigned char*>(buf->start), rgbBuf, width, height);
    c.queueBuf(buf);
    c.stopStream();

    auto app = Gtk::Application::create(argc, argv, "cn.hoohan.v4l2.homework");

    ImageWindow win(rgbBuf, width, height);

    return app->run(win);
}
