#ifndef __IMAGE_WINDOW_HPP__
#define __IMAGE_WINDOW_HPP__

#include <gtkmm.h>
#include "gdkmm/pixbuf.h"
#include "glibmm/refptr.h"

#include <string>

namespace hmwk{

class ImageArea : public Gtk::DrawingArea {
public:
    ImageArea();
    virtual ~ImageArea();

protected:
    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width,
                 int height);
    Glib::RefPtr<Gdk::Pixbuf> image_;
};

class ImageWindow : public Gtk::Window{
public:
    ImageWindow();
    virtual ~ImageWindow();
protected:
    ImageArea area_;
};

} // namespace hmwk

#endif