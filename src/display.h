#ifndef DISPLAY_H
#define DISPLAY_H

#include "window.h"
#include "source.h"

class Display
{
public:
    Display( Window* window, Source* source );

    void loop();

protected:
    virtual void processKeyPressed( const char key );

private:
    bool m_isExit;

    Window* m_window;

    Source* m_source;

    CvVideoWriter *writer;

};

#endif // DISPLAY_H
