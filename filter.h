#ifndef FILTER_H
#define FILTER_H

#include "source.h"

class Filter : public Source
{
public:
    Filter( Source* source );

    virtual IplImage* frame();

    virtual IplImage* process( IplImage* frame );

private:
    Source* m_source;
};

#endif // FILTER_H
