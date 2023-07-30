#ifndef __IMAGE_WINDOW_HPP__
#define __IMAGE_WINDOW_HPP__

#include <gtkmm.h>

#include <string>

#include "gdkmm/pixbuf.h"
#include "glibmm/refptr.h"

namespace hmwk {

class ImageArea : public Gtk::DrawingArea {
public:
    ImageArea(unsigned char* rgbBuf, unsigned int width, unsigned int height);
    virtual ~ImageArea();

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    Glib::RefPtr<Gdk::Pixbuf> image_;

private:
    unsigned int width_, height_;
    unsigned char* rgbBuf_;
};

class ImageWindow : public Gtk::Window {
public:
    ImageWindow(unsigned char* rgbBuf, unsigned int width, unsigned int height);
    virtual ~ImageWindow();

protected:
    ImageArea area_;
    unsigned int width_, height_;
};

}  // namespace hmwk

#endif