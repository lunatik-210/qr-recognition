#ifndef SOURCE_H
#define SOURCE_H

#include <opencv/cv.h>

class Source
{
public:
    Source() {};

    virtual ~Source() {};

    virtual IplImage* frame() = 0;

    virtual void processKeyPressed( const char key ) {}
};

#endif // SOURCE_H
