/*
 * todo
 */

#include <ImageWindow.hpp>
// #include <iostream>

#include "gtkmm/application.h"

// using std::cout;
// using std::endl;

using namespace hmwk;

int main(int argc, char** argv) {
    // cout << "hello world" << endl;
    auto app = Gtk::Application::create("cn.hoohan.l4v2.homework");
    return app->make_window_and_run<ImageWindow>(argc, argv);
}