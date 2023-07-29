#include <unistd.h>

#include <Camera.hpp>
#include <ImageWindow.hpp>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "gdkmm/general.h"
#include "gdkmm/pixbuf.h"

namespace hmwk {

ImageWindow::ImageWindow(unsigned char* rgbBuf, unsigned int width,
                         unsigned int height)
    : area_(rgbBuf, width, height), width_(width), height_(height) {
    set_title("Image Window");
    set_default_size(width, height);

    this->add(area_);
    area_.show();
}

ImageWindow::~ImageWindow() {}

ImageArea::ImageArea(unsigned char* rgbBuf, unsigned int width,
                     unsigned int height)
    : rgbBuf_(rgbBuf), width_(width), height_(height) {
    try {
        image_ = Gdk::Pixbuf::create_from_data(
            rgbBuf, Gdk::Colorspace::COLORSPACE_RGB, false, 8, width, height,
            width * 3);
    } catch (const Gio::ResourceError& ex) {
        std::cerr << "ResourceError: " << ex.what() << std::endl;
    } catch (const Gdk::PixbufError& ex) {
        std::cerr << "PixbufError: " << ex.what() << std::endl;
    }

    if (image_) {
        set_size_request(image_->get_width() / 2, image_->get_height() / 2);
    }
}

ImageArea::~ImageArea() {}

bool ImageArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    if (!image_) {
        return false;
    }
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    // Draw the image in the middle of the drawing area, or (if the image is
    // larger than the drawing area) draw the middle part of the image.
    Gdk::Cairo::set_source_pixbuf(cr, image_, (width - image_->get_width()) / 2,
                                  (height - image_->get_height()) / 2);
    cr->paint();

    return true;
}

}  // namespace hmwk

