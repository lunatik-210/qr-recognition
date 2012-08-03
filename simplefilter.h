#ifndef SIMPLEFILTER_H
#define SIMPLEFILTER_H

#include "filter.h"

class SimpleFilter : public Filter
{
public:
    SimpleFilter( Source* source )
        : Filter(source) {}

    virtual IplImage* process( IplImage* frame );

};

#endif // SIMPLEFILTER_H
