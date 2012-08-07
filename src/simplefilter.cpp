#include "simplefilter.h"

#include <QVector>

IplImage* SimpleFilter::process( IplImage* frame )
{
    IplImage* gray = 0;

    gray = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );

    cvCvtColor( frame, gray, CV_RGB2GRAY );
    cvThreshold( gray, gray, 100, 255, CV_THRESH_BINARY );

    //создание хранилище памяти
    CvMemStorage* storage = cvCreateMemStorage( 0 );

    CvSeq* firstContour = NULL;
    cvFindContours( gray, storage, &firstContour, sizeof( CvContour ), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint( 0, 0 ) );

    QVector<CvRect> rects = collectRects( firstContour );

    for( int indx = 0; indx < rects.size(); ++indx )
    {
        CvPoint p1, p2;
        p1.x = rects[indx].x;
        p1.y = rects[indx].y;
        p2.x = rects[indx].x+rects[indx].width;
        p2.y = rects[indx].y+rects[indx].height;
        cvRectangle( frame, p1, p2, CV_RGB(30,216,30), 1, 8, 0 );
    }

    cvReleaseImage( &gray );
    cvReleaseMemStorage( &storage );
    return frame;
}

bool SimpleFilter::isValidContour( CvSeq* contour, const int requiredDepth /* = 2 */ )
{
    CvSeq* seq = contour;
    int c = 0;
    while( ( seq = seq->v_next ) != NULL )
    {
        ++c;
    }

    if( c != requiredDepth )
    {
        return false;
    }
    return true;
}

QVector<CvRect> SimpleFilter::collectRects( CvSeq* contour )
{
    QVector<CvRect> rects;

    if( !contour )
    {
        return rects;
    }

    if( isValidContour( contour ) && checkQRPattern( contour ) )
    {
        CvRect rect = cvBoundingRect( contour );
        CvBox2D box = cvMinAreaRect2( contour );

        double countourArea = 0.0;
        double i1, i2 = 0.0;

        if( box.size.height < 0.1 || box.size.width < 0.1 )
        {
            i1=0;
        }
        else
        {
            countourArea = fabs( cvContourArea( contour ) );
            i1 = countourArea / (double)( box.size.width * box.size.height );
            i2 =( ( box.size.width < box.size.height ) ?
               (double) box.size.width / (double) box.size.height:
               (double) box.size.height / (double) box.size.width );
        }

        if( i1>=0.65 && i2>=0.65 && abs( box.size.width * box.size.height - countourArea ) < countourArea * 0.3 )
        {
            rects.push_back( rect );
        }
    }

    QVector<CvRect> rects1 = collectRects( contour->h_next );
    QVector<CvRect> rects2 = collectRects( contour->v_next );

    foreach( CvRect r, rects1 )
        rects.push_back(r);

    foreach( CvRect r, rects2 )
        rects.push_back(r);

    return rects;
}

bool SimpleFilter::checkQRPattern( CvSeq* contour )
{
    CvSeq* seq = contour;
    CvBox2D box1 = cvMinAreaRect2( seq );
    float area1 = ( box1.size.width * box1.size.height );

    seq = seq->v_next;
    CvBox2D box2 = cvMinAreaRect2( seq );
    float area2 = ( box2.size.width * box2.size.height );

    seq = seq->v_next;
    CvBox2D box3 = cvMinAreaRect2( seq );
    float area3 = ( box3.size.width * box3.size.height );

    float q1 = area1/area2;
    float q2 = area2/area3;

    /*****************
    // 7*7 = 49
    // 5*5 = 25
    // 3*3 = 9
    //
    // 49 / 25 ~= 1.96
    // 25 / 9 ~= 2.78
    ******************/

    if( ( 1.70 < q1 && q1 < 2.2 ) && ( 2.5 < q2 && q2 < 3.0 ) )
        return true;

    return false;
}
