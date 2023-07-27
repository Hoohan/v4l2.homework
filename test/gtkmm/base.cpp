#include <gtkmm.h>

#include <iostream>

#include "cairomm/enums.h"
#include "cairomm/refptr.h"
#include "gdkmm/pixbuf.h"
#include "glibmm/refptr.h"
#include "gtkmm/drawingarea.h"

class MyArea : public Gtk::DrawingArea {
public:
    MyArea();
    virtual ~MyArea();

protected:
    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width,
                 int height);
    Glib::RefPtr<Gdk::Pixbuf> m_image;
};

class MyWindow : public Gtk::Window {
public:
    MyWindow();
    virtual ~MyWindow();

protected:
    MyArea m_area;
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("org.gtkmm.examples.base");

    return app->make_window_and_run<MyWindow>(argc, argv);
}

MyWindow::MyWindow() {
    set_title("Basic application Test");
    set_default_size(600, 300);
    set_child(m_area);
}

MyWindow::~MyWindow() {}

MyArea::MyArea() {
    try {
        m_image = Gdk::Pixbuf::create_from_file("data/test.png");
    } catch (const Gio::ResourceError& ex) {
        std::cerr << "ResourceError: " << ex.what() << std::endl;
    } catch (const Gdk::PixbufError& ex) {
        std::cerr << "PixbufError: " << ex.what() << std::endl;
    }

    if (m_image) {
        set_content_width(m_image->get_width() / 2);
        set_content_height(m_image->get_height() / 2);
    }

    set_draw_func(sigc::mem_fun(*this, &MyArea::on_draw));
}

MyArea::~MyArea() {}

void MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width,
                     int height) {
    if (!m_image) return;
    Gdk::Cairo::set_source_pixbuf(cr, m_image,
                                  (width - m_image->get_width()) / 2,
                                  (height - m_image->get_height()) / 2);
    cr->paint();
}