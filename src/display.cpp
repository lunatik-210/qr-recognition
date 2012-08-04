#include "display.h"

#include <opencv/cv.h>

Display::Display( Window* window, Source* source )
{
    this->m_isExit = false;
    this->m_window = window;
    this->m_source = source;
}

void Display::loop()
{
    while( !this->m_isExit )
    {
        processKeyPressed( cvWaitKey( 10 ) );
        this->m_window->blit( m_source->frame() );
    }
}

void Display::processKeyPressed( const char key )
{
    if ( key == 27 )
    {
        m_isExit = true;
    }
}
