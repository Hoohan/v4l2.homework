#ifndef __CONTROL_WINDOW_HPP__
#define __CONTROL_WINDOW_HPP__

#include <gtkmm/button.h>
#include <gtkmm/window.h>

#include <string>

#include "Camera.hpp"
#include "ImageWindow.hpp"

namespace hmwk {

class ControlWindow : public Gtk::Window {
public:
    ControlWindow(std::string devName);
    virtual ~ControlWindow();

protected:
    void on_button_clicked();
    Gtk::Button captureButton_;

private:
    Camera camera_;
    ImageWindow* imageWindow_;
    // Gtk::Window *win_;
};

}  // namespace hmwk

#endif