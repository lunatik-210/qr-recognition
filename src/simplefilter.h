#ifndef SIMPLEFILTER_H
#define SIMPLEFILTER_H

#include "filter.h"

#include <QVector>

struct Vertex
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
        : Filter(source), errorSide(0.1), errorAngle(0.2) {}

    virtual IplImage* process( IplImage* frame );

    virtual void processKeyPressed( const char key );

protected:
    QVector<CvSeq*> collectContours( CvSeq* current );

    bool checkContourDepth( CvSeq* conture, const int requiredDepth = 2 );

    bool isQRMarker( CvSeq* contour, float error );

    CvPoint getCenterPoint(CvSeq* contour);

    QVector<Vertex> findQrCode( const QVector<CvSeq*>& contours );

    void drawDebugInfo( IplImage* frame, QVector<Vertex> qrCodes, QVector<CvSeq*> contours );

    inline double getLenghtLine( CvPoint p1, CvPoint p2 );

    inline bool isEquilateralTriangle( float side1, float side2, float side3 );

private:
    float errorSide;

    float errorAngle;

};

#endif // SIMPLEFILTER_H
