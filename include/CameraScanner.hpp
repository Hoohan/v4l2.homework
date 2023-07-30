#ifndef __CAMERA_SCANNER_HPP__
#define __CAMERA_SCANNER_HPP__

#include <vector>
#include <string>

namespace hmwk {

struct DeviceInfo{
    std::string deviceDescription;
    std::string busInfo;
    std::vector<std::string> devicePaths;
};

class CameraScanner{
public:
    CameraScanner();
    ~CameraScanner();
    std::vector<DeviceInfo>& getList();

private:
    void scanFiles();
    void getDevInfo();

private:
    std::vector<DeviceInfo> cameras_;
    std::vector<std::string> files_;
};


} // namespace hmwk 

#endif