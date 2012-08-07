#ifndef SIMPLEFILTER_H
#define SIMPLEFILTER_H

#include "filter.h"

#include <QVector>

class SimpleFilter : public Filter
{
public:
    SimpleFilter( Source* source )
        : Filter(source) {}

    virtual IplImage* process( IplImage* frame );

protected:
    QVector<CvRect> collectRects( CvSeq* current );

    bool isValidContour( CvSeq* conture, const int requiredDepth = 2 );

    bool checkQRPattern( CvSeq* contour );

};

#endif // SIMPLEFILTER_H
