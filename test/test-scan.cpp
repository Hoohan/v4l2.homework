#include <CameraScanner.hpp>
#include <iostream>
#include <string>
#include <vector>

int main() {
    hmwk::CameraScanner cs;
    std::vector<hmwk::DeviceInfo>& vdi = cs.getList();
    for (const auto& device : vdi) {
        std::cout << "[" << device.deviceDescription << "] at [" << device.busInfo
                  << "] is attached to:\n";
        for (const auto& path : device.devicePaths) {
            std::cout << "\t" << path << "\n";
        }
    }
}