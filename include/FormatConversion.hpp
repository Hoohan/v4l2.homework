#ifndef __FORMAT_CONVERSION_HPP__
#define __FORMAT_CONVERSION_HPP__

namespace hmwk {

void yuyv2rgb(unsigned char* yuyvBuf, unsigned char* rgbBuf, unsigned int width,
              unsigned int height);

}  // namespace hmwk

#endif