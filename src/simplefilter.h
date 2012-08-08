#ifndef SIMPLEFILTER_H
#define SIMPLEFILTER_H

#include "filter.h"

#include <QVector>

struct vertex
{
    CvPoint p1;
    CvPoint p2;
    CvPoint p3;

    CvSeq   *con1;
    CvSeq   *con2;
    CvSeq   *con3;
};

class SimpleFilter : public Filter
{
public:
    SimpleFilter( Source* source )
        : Filter(source), er1(0.1), er2(0.2) {}

    virtual IplImage* process( IplImage* frame );

protected:
    QVector<CvSeq*> collectContours( CvSeq* current );

    bool checkContourDepth( CvSeq* conture, const int requiredDepth = 2 );

    bool isQRMarker( CvSeq* contour, float error );

    CvPoint getCenterPoint(CvSeq* contour);

    inline double getLenghtLine(CvPoint p1, CvPoint p2);

    IplImage* findQrCode( const QVector<CvSeq*>& contours, IplImage* frame, float errorAngle, float errorSide );

private:
    float er1, er2;

};

#endif // SIMPLEFILTER_H
