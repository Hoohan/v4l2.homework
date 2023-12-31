#include <FormatConversion.hpp>

#include <algorithm>

namespace hmwk {

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

        rgbBuf[rgbPixIdx + 1] = R1;
        rgbBuf[rgbPixIdx + 2] = G1;
        rgbBuf[rgbPixIdx + 3] = B1;
        rgbBuf[rgbPixIdx + 4] = R2;
        rgbBuf[rgbPixIdx + 5] = G2;
        rgbBuf[rgbPixIdx + 6] = B2;
    }
    return;
}

} // namespace hmwk