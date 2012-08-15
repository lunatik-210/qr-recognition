#include "video.h"

Video::Video( const QString &filename )
    : m_fileName(filename)
{
    m_capture = NULL;
}

void Video::init()
{
    this->m_capture = cvCaptureFromFile( this->m_fileName.toStdString().c_str() );
    this->m_width = cvGetCaptureProperty( this->m_capture, CV_CAP_PROP_FRAME_WIDTH );
    this->m_height = cvGetCaptureProperty( this->m_capture, CV_CAP_PROP_FRAME_HEIGHT );
}

IplImage* Video::frame()
{
    return cvQueryFrame( this->m_capture );
}

Video::~Video()
{
    cvReleaseCapture( &( this->m_capture ) );
}
