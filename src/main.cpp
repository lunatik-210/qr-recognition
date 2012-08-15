
#include "window.h"
#include "display.h"
#include "camera.h"
#include "simplefilter.h"
#include "video.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "qrpatternfilter.h"

#include "facefilter.h"

int main(int argc, char *argv[])
{
    Window* window = new Window( "capture" );
    window->init();

    Camera* camera = new Camera();
    camera->init();

    Video* video = new Video("./resources/test.avi");
    video->init();

    SimpleFilter* filter = new SimpleFilter( video );

    QrPatternFilter* qrPatternFilter = new QrPatternFilter( video );

    Display* display = new Display( window, qrPatternFilter );

    display->loop();

    delete qrPatternFilter;
    delete filter;
    delete camera;
    delete video;
    delete window;
    delete display;
    return 0;
}
