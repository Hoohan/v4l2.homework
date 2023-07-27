#include <ImageWindow.hpp>

#include "gdkmm/general.h"
#include "gdkmm/pixbuf.h"

#include <iostream>

hmwk::ImageWindow::ImageWindow() {
    set_title("Image Window");
    set_default_size(640, 480);
    set_child(area_);
}

hmwk::ImageWindow::~ImageWindow() {}

hmwk::ImageArea::ImageArea() {
    try {
        image_ = Gdk::Pixbuf::create_from_file("data/test.png");
    } catch (const Gio::ResourceError& ex) {
        std::cerr << "ResourceError: " << ex.what() << std::endl;
    } catch (const Gdk::PixbufError& ex) {
        std::cerr << "PixbufError: " << ex.what() << std::endl;
    }

    if(image_){
        set_content_width(image_->get_width() / 2);
        set_content_height(image_->get_height() / 2);
    }

    set_draw_func(sigc::mem_fun(*this, &ImageArea::on_draw));
}

hmwk::ImageArea::~ImageArea() {}

void hmwk::ImageArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width,
                        int height) {
    if (!image_) {
        return;
    }
    Gdk::Cairo::set_source_pixbuf(cr, image_, (width - image_->get_width()) / 2,
                                  (height - image_->get_height()) / 2);
    cr->paint();
}
