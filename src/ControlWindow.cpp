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
    // dqueue buf
    FrameBuffer* buf = camera_.dqueueBufWithFlush();
    // create window
    imageWindow_ = new ImageWindow(buf->start, default_width, default_height);
    imageWindow_->show();
    // queuebuf
    camera_.queueBuf(buf);
}

}  // namespace hmwk