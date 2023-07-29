// #include <gtkmm.h>

#include <cairomm/context.h>
#include <gdkmm/general.h>
#include <gdkmm/pixbuf.h>
#include <giomm/resource.h>
#include <glibmm/fileutils.h>
#include <gtkmm/application.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/window.h>

#include <iostream>

class MyArea : public Gtk::DrawingArea {
public:
    MyArea();
    virtual ~MyArea();

protected:
    // Override default signal handler:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

    Glib::RefPtr<Gdk::Pixbuf> m_image;
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    Gtk::Window win;
    win.set_title("DrawingArea");
    win.set_default_size(600, 400);

    MyArea area;
    win.add(area);
    area.show();

    return app->run(win);
}
MyArea::MyArea() {
    try {
        // The fractal image has been created by the XaoS program.
        // http://xaos.sourceforge.net
        m_image = Gdk::Pixbuf::create_from_file("data/test.png");
    } catch (const Gio::ResourceError& ex) {
        std::cerr << "ResourceError: " << ex.what() << std::endl;
    } catch (const Gdk::PixbufError& ex) {
        std::cerr << "PixbufError: " << ex.what() << std::endl;
    }

    // Show at least a quarter of the image.
    if (m_image)
        set_size_request(m_image->get_width() / 2, m_image->get_height() / 2);
}

MyArea::~MyArea() {}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    if (!m_image) return false;

    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    // Draw the image in the middle of the drawing area, or (if the image is
    // larger than the drawing area) draw the middle part of the image.
    Gdk::Cairo::set_source_pixbuf(cr, m_image,
                                  (width - m_image->get_width()) / 2,
                                  (height - m_image->get_height()) / 2);
    cr->paint();

    return true;
}