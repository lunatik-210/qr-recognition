
#include "window.h"
#include "display.h"
#include "camera.h"
#include "simplefilter.h"

int main(int argc, char *argv[])
{
    Window* window = new Window( "capture" );
    window->init();

    Camera* camera = new Camera();
    camera->init();

    SimpleFilter* filter = new SimpleFilter( camera );

    Display* display = new Display( window, filter );

    display->loop();

    delete camera;
    delete window;
    delete display;
    return 0;
}
