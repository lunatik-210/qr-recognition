#include "filter.h"

Filter::Filter( Source* source )
{
    this->m_source = source;
}

IplImage* Filter::frame()
{
    return process( this->m_source->frame() );
}

IplImage* Filter::process( IplImage* frame )
{
    return frame;
}
