#include "camera.h"

#include <opencv/cv.h>

Camera::Camera()
{
    m_capture = NULL;
}

void Camera::init()
{
    this->m_capture = cvCreateCameraCapture( CV_CAP_ANY );
    this->m_width = cvGetCaptureProperty( this->m_capture, CV_CAP_PROP_FRAME_WIDTH );
    this->m_height = cvGetCaptureProperty( this->m_capture, CV_CAP_PROP_FRAME_HEIGHT );
}

IplImage* Camera::frame()
{
    return cvQueryFrame( this->m_capture );
}

Camera::~Camera()
{
    cvReleaseCapture( &( this->m_capture ) );
}
