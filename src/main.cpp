#include <gtkmm/application.h>
#include <stdio.h>
#include <unistd.h>

#include <Camera.hpp>
#include <CameraScanner.hpp>
#include <ControlWindow.hpp>
#include <FormatConversion.hpp>
#include <ImageWindow.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace hmwk;

void printUsage() {
    printf(
        "Usage:\n \
        \t-s Scan all cameras\n \
        \t-d [device path] open camera and capture\n");
}

int main(int argc, char* argv[]) {
    int ret;
    const char* optstring = "sd:";
    bool is_scan = false;
    bool is_capture = false;
    std::string devName;
    while ((ret = getopt(argc, argv, optstring)) != -1) {
        switch (ret) {
            case 's':
                is_scan = true;
                break;
            case 'd':
                is_capture = true;
                devName = std::string(optarg);
                break;
            case '?':
                printf("error opt: %c\n", optopt);
                break;
        }
    }

    if (is_scan) {
        hmwk::CameraScanner cs;
        cs.printAll();
        return 0;
    } else if (is_capture) {
        argc = 1;
        auto app =
            Gtk::Application::create(argc, argv, "cn.hoohan.v4l2.homework");
        ControlWindow win(devName);
        return app->run(win);
    } else {
        printUsage();
        return 0;
    }
}
