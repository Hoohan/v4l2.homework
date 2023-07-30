#include <unistd.h>

#include <Camera.hpp>
#include <FormatConversion.hpp>
#include <ImageWindow.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace hmwk {

ImageWindow::ImageWindow(void* rawBuf, unsigned int width, unsigned int height)
    : area_(rawBuf, width, height), width_(width), height_(height) {
    set_title("Image Window");
    set_default_size(width, height);

    this->add(area_);
    area_.show();
}

ImageWindow::~ImageWindow() {}

ImageArea::ImageArea(void* rawBuf, unsigned int width, unsigned int height)
    : rawBuf_(rawBuf), width_(width), height_(height) {
    // format conversion
    rgbBuf_ = static_cast<unsigned char*>(
        calloc(width_ * height_ * 3, sizeof(unsigned char)));
    yuyv2rgb(static_cast<unsigned char*>(rawBuf_), rgbBuf_, width_, height_);

    // create and set image
    try {
        image_ = Gdk::Pixbuf::create_from_data(
            rgbBuf_, Gdk::Colorspace::COLORSPACE_RGB, false, 8, width, height,
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

ImageArea::~ImageArea() { free(rgbBuf_); }

bool ImageArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    if (!image_) {
        return false;
    }
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    double pixbuf_x = (width - image_->get_width()) / 2.0;
    double pixbuf_y = (height - image_->get_height()) / 2.0;

    // Draw the image in the middle of the drawing area, or (if the image is
    // larger than the drawing area) draw the middle part of the image.
    Gdk::Cairo::set_source_pixbuf(cr, image_, pixbuf_x, pixbuf_y);
    cr->paint();

    return true;
}

}  // namespace hmwk
