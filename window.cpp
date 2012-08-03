#include "window.h"

Window::Window( const char* name )
{
    this->m_name = name;
}

Window::~Window()
{
    cvDestroyWindow( this->m_name );
}

void Window::init()
{
    cvNamedWindow( this->m_name, CV_WINDOW_AUTOSIZE );
}

void Window::blit( IplImage* frame )
{
    cvShowImage( this->m_name, frame );
}
