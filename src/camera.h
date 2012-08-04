#ifndef CAMERA_H
#define CAMERA_H

#include "source.h"

#include <opencv/highgui.h>

class Camera : public Source
{
public:
    Camera();

    virtual void init();

    virtual IplImage* frame();

    virtual ~Camera();

private:
    double m_width;

    double m_height;

    CvCapture* m_capture;
};

#endif // CAMERA_H
