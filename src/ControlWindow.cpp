#include <Camera.hpp>
#include <ControlWindow.hpp>
#include <FormatConversion.hpp>
#include <ImageWindow.hpp>

#include "ImageWindow.hpp"

namespace hmwk {

const unsigned int default_width = 640;
const unsigned int default_height = 480;

ControlWindow::ControlWindow(std::string devName)
    : camera_(devName, default_width, default_height),
      captureButton_("Capture Image") {
    // window settings
    set_title("Capture Control: " + devName);
    set_border_width(20);

    // camera settings
    camera_.startStream();

    // button settings
    captureButton_.signal_clicked().connect(
        sigc::mem_fun(*this, &ControlWindow::on_button_clicked));

    this->add(captureButton_);
    captureButton_.show();
}

ControlWindow::~ControlWindow() { camera_.startStream(); }

void ControlWindow::on_button_clicked() {
    FrameBuffer* buf = camera_.dqueueBufWithFlush();
    // printf("buf.index: \t%u\n", buf->index);
    unsigned char* rgbBuf = static_cast<unsigned char*>(
        calloc(default_width * default_height * 3, sizeof(unsigned char)));
    yuyv2rgb(static_cast<unsigned char*>(buf->start), rgbBuf, default_width,
             default_height);
    camera_.queueBuf(buf);

    // create window
    imageWindow_ = new ImageWindow(rgbBuf, default_width, default_height);
    imageWindow_->show();
}

}  // namespace hmwk