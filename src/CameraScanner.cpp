#include <bits/types/FILE.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/media.h>
#include <linux/v4l2-common.h>
#include <linux/v4l2-controls.h>
#include <linux/videodev2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <CameraScanner.hpp>
#include <algorithm>
#include <iostream>
#include <map>

namespace hmwk {

CameraScanner::CameraScanner() {
    scanFiles();
    getDevInfo();
}

CameraScanner::~CameraScanner() {}

void CameraScanner::scanFiles() {
    const std::string devRootPath = "/dev/";
    DIR* pdir = opendir(devRootPath.c_str());
    if (pdir == NULL) {
        printf("Fatal Error: Can not read /dev.");
        exit(-1);
    }
    struct dirent* pdirent;
    while ((pdirent = readdir(pdir)) != NULL) {
        if (strlen(pdirent->d_name) > 5 &&
            !strncmp("video", pdirent->d_name, 5)) {
            std::string file = devRootPath + pdirent->d_name;
            const int fd = open(file.c_str(), O_RDWR);
            if (fd < 0) {
                continue;
            }
            v4l2_capability capability;
            if (ioctl(fd, VIDIOC_QUERYCAP, &capability) >= 0) {
                files_.push_back(file);
            }
            close(fd);
        }
    }
    std::sort(files_.begin(), files_.end());
}

void CameraScanner::getDevInfo() {
    struct v4l2_capability vcap;
    std::map<std::string, DeviceInfo> device_map;
    for (const auto& file : files_) {
        std::string busInfo;
        std::string card;

        int fd = open(file.c_str(), O_RDWR);
        if (fd < 0) {
            continue;
        }

        int err = ioctl(fd, VIDIOC_QUERYCAP, &vcap);

        if (err) {
            struct media_device_info mdi;

            err = ioctl(fd, MEDIA_IOC_DEVICE_INFO, &mdi);
            if (!err) {
                if (mdi.bus_info[0])
                    busInfo = mdi.bus_info;
                else
                    busInfo = std::string("platform:") + mdi.driver;

                if (mdi.model[0])
                    card = mdi.model;
                else
                    card = mdi.driver;
            }
        } else {
            busInfo = reinterpret_cast<const char*>(vcap.bus_info);
            card = reinterpret_cast<const char*>(vcap.card);
        }

        close(fd);

        if (!busInfo.empty() && !card.empty()) {
            if (device_map.find(busInfo) != device_map.end()) {
                DeviceInfo& device = device_map.at(busInfo);
                device.devicePaths.emplace_back(file);
            } else {
                DeviceInfo device;
                device.devicePaths.emplace_back(file);
                device.busInfo = busInfo;
                device.deviceDescription = card;
                device_map.insert(
                    std::pair<std::string, DeviceInfo>(busInfo, device));
            }
        }
    }

    for (const auto& row : device_map) {
        cameras_.emplace_back(row.second);
    }
}

std::vector<DeviceInfo>& CameraScanner::getList() { return cameras_; }

void CameraScanner::printAll() {
    for (const auto& device : cameras_) {
        std::cout << "[" << device.deviceDescription << "] at ["
                  << device.busInfo << "] is attached to:\n";
        for (const auto& path : device.devicePaths) {
            std::cout << "\t" << path << "\n";
        }
    }
}

}  // namespace hmwk