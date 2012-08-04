#ifndef SOURCE_H
#define SOURCE_H

#include <opencv/cv.h>

class Source
{
public:
    Source() {};

    virtual ~Source() {};

    virtual IplImage* frame() = 0;
};

#endif // SOURCE_H
