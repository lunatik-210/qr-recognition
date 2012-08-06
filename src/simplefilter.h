#ifndef SIMPLEFILTER_H
#define SIMPLEFILTER_H

#include "filter.h"

class SimpleFilter : public Filter
{
public:
    SimpleFilter( Source* source )
        : Filter(source) {}

    virtual IplImage* process( IplImage* frame );

protected:
    CvSeq* nextSeq( CvSeq* current );

    bool isValidContour( CvSeq* conture );

};

#endif // SIMPLEFILTER_H
