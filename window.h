#ifndef WINDOW_H
#define WINDOW_H

#include "opencv/cv.h"
#include "opencv/highgui.h"

class Window
{
public:
    Window( const char* name );

    virtual ~Window();

    virtual void init();

    virtual void blit( IplImage* frame );

private:
    const char* m_name;
};

#endif // WINDOW_H
