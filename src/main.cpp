
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

    //Source* filter = new SimpleFilter( video );
    Source* filter = new QrPatternFilter( camera );

    Display* display = new Display( window, filter );

    display->loop();

    delete filter;
    delete camera;
    delete video;
    delete window;
    delete display;
    return 0;
}
