#include <unistd.h>
#include <Camera.hpp>

#include <string>

using namespace std;
using namespace hmwk;

int main(){
    string name = "/dev/video0";
    unsigned int width = 640, height = 480;
    Camera c(name, width, height);
    c.printCapability();
    c.printFormat();
    c.startStream();
    for(int i = 0; i < 20; i++){
        FrameBuffer* buf = c.dqueueBuf();
        printf("buf.index: \t%u\n", buf->index);
        c.queueBuf(buf);
    }
    c.stopStream();
}