#include <unistd.h>

#include <Camera.hpp>
#include <ImageWindow.hpp>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "gdkmm/general.h"
#include "gdkmm/pixbuf.h"

void yuyv2rgb(unsigned char* yuyvBuf, unsigned char* rgbBuf, unsigned int width,
              unsigned int height);

hmwk::ImageWindow::ImageWindow() {
    set_title("Image Window");
    set_default_size(640, 480);
    set_child(area_);
}

hmwk::ImageWindow::~ImageWindow() {}

hmwk::ImageArea::ImageArea() {
    std::string name = "/dev/video0";
    unsigned int width = 640, height = 480;
    Camera c(name, width, height);
    c.printCapability();
    c.printFormat();
    c.startStream();
    sleep(1);
    FrameBuffer* buf = c.dqueueBuf();
    printf("buf.index: \t%u\n", buf->index);
    unsigned char* rgbBuf = static_cast<unsigned char*>(
        calloc(width * height * 3, sizeof(unsigned char)));
    // memset(rgbBuf, 0, width * height * 3);
    yuyv2rgb(static_cast<unsigned char*>(buf->start), rgbBuf, width, height);
    c.queueBuf(buf);
    c.stopStream();

    try {
        // image_ = Gdk::Pixbuf::create_from_file("data/test.png");
        image_ = Gdk::Pixbuf::create_from_data(
            rgbBuf, Gdk::Colorspace::RGB, false, 8, width, height, 640 * 3);
    } catch (const Gio::ResourceError& ex) {
        std::cerr << "ResourceError: " << ex.what() << std::endl;
    } catch (const Gdk::PixbufError& ex) {
        std::cerr << "PixbufError: " << ex.what() << std::endl;
    }

    // free(rgbBuf);

    if (image_) {
        set_content_width(image_->get_width() / 2);
        set_content_height(image_->get_height() / 2);
    }

    set_draw_func(sigc::mem_fun(*this, &ImageArea::on_draw));
}

hmwk::ImageArea::~ImageArea() {}

void hmwk::ImageArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr,
                              int width, int height) {
    if (!image_) {
        return;
    }
    Gdk::Cairo::set_source_pixbuf(cr, image_, (width - image_->get_width()) / 2,
                                  (height - image_->get_height()) / 2);
    cr->paint();
}

void yuyv2rgb(unsigned char* yuyvBuf, unsigned char* rgbBuf, int unsigned width,
              unsigned int height) {
    using uchar = unsigned char;
    using uint = unsigned int;
    using std::max;
    using std::min;
    uchar Y1, Cb, Y2, Cr;
    uchar R1, G1, B1;
    uchar R2, G2, B2;
    uint totalPixNum = width * height;
    for (uint i = 0; i < totalPixNum / 2; ++i) {
        uint yuyvPixIdx = i * 4;
        uint rgbPixIdx = i * 6;

        Y1 = yuyvBuf[yuyvPixIdx + 0];
        Cb = yuyvBuf[yuyvPixIdx + 1];
        Y2 = yuyvBuf[yuyvPixIdx + 2];
        Cr = yuyvBuf[yuyvPixIdx + 3];

        R1 = max(0.0, min(255.0, 1.164 * (Y1 - 16) + 1.596 * (Cr - 128)));
        G1 = max(0.0, min(255.0, 1.164 * (Y1 - 16) - 0.813 * (Cr - 128) -
                                     0.391 * (Cb - 128)));
        B1 = max(0.0, min(255.0, 1.164 * (Y1 - 16) + 2.018 * (Cr - 128)));

        R2 = max(0.0, min(255.0, 1.164 * (Y2 - 16) + 1.596 * (Cr - 128)));
        G2 = max(0.0, min(255.0, 1.164 * (Y2 - 16) - 0.813 * (Cr - 128) -
                                     0.391 * (Cb - 128)));
        B2 = max(0.0, min(255.0, 1.164 * (Y2 - 16) + 2.018 * (Cr - 128)));

        // R1 = Y1;
        // G1 = Y1;
        // B1 = Y1;

        // R2 = Y2;
        // G2 = Y2;
        // B2 = Y2;

        rgbBuf[rgbPixIdx + 1] = R1;
        rgbBuf[rgbPixIdx + 2] = G1;
        rgbBuf[rgbPixIdx + 3] = B1;
        rgbBuf[rgbPixIdx + 4] = R2;
        rgbBuf[rgbPixIdx + 5] = G2;
        rgbBuf[rgbPixIdx + 6] = B2;

        // printf("yuyvPixIdx = %u, rgbPixIdx = %u\n", yuyvPixIdx, rgbPixIdx);
        // printf("Y1 = %d, Cb = %d, Y2 = %d, Cr = %d\n", Y1, Cb, Y2, Cr);
        // printf("R1 = %d, G1 = %d, B1 = %d, R2 = %d, G2 = %d, B1 = %d\n", R1,
        // G1,
        //        B1, R2, G2, B2);
    }
    return;
}