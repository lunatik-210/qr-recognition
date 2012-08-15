#ifndef QRPATTERNFILTER_H
#define QRPATTERNFILTER_H

#include "filter.h"

class QrPatternFilter : public Filter
{
public:
    QrPatternFilter( Source* source )
        : Filter( source )
    {}

    virtual IplImage* process( IplImage* frame );
};

struct Block
{
    int count;
    int color;
    int x;
    int y;
};

#endif // QRPATTERNFILTER_H
