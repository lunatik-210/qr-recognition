#ifndef FACEFILTER_H
#define FACEFILTER_H

#include "filter.h"

class FaceFilter : public Filter
{
public:
    FaceFilter( Source* source )
        : Filter(source) {}

    virtual IplImage* process( IplImage* frame );
};

#endif // FACEFILTER_H
