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
    QVector<CvSeq*> collectContours( CvSeq* current );

    bool checkContourDepth( CvSeq* conture, const int requiredDepth = 2 );

    bool checkQRPattern( CvSeq* contour, float error );

};

#endif // SIMPLEFILTER_H
