#include "simplefilter.h"
#include "constants.h"

#include <QVector>

IplImage* SimpleFilter::process( IplImage* frame )
{
    IplImage* gray = 0;

    gray = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );

    cvCvtColor( frame, gray, CV_RGB2GRAY );
    cvThreshold( gray, gray, 100, 255, CV_THRESH_BINARY );

    //создание хранилище памяти
    CvMemStorage* storage = cvCreateMemStorage( 0 );

    CvSeq* root = NULL;
    cvFindContours( gray, storage, &root, sizeof( CvContour ), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint( 0, 0 ) );

    QVector<CvSeq*> contours = collectContours( root );

    for( int indx = 0; indx < contours.size(); ++indx )
    {
        CvRect rect = cvBoundingRect( contours[indx] );
        CvPoint p1, p2;
        p1.x = rect.x;
        p1.y = rect.y;
        p2.x = rect.x+rect.width;
        p2.y = rect.y+rect.height;
        cvRectangle( frame, p1, p2, CV_RGB(30,216,30), 1, 8, 0 );
    }

    cvReleaseImage( &gray );
    cvReleaseMemStorage( &storage );
    return frame;
}

bool SimpleFilter::checkContourDepth( CvSeq* contour, const int requiredDepth /* = 2 */ )
{
    CvSeq* seq = contour;
    int c = 0;
    while( ( seq = seq->v_next ) != NULL )
    {
        if( ++c > requiredDepth )
            return false;
    }

    if( c != requiredDepth )
    {
        return false;
    }
    return true;
}

QVector<CvSeq*> SimpleFilter::collectContours( CvSeq* contour )
{
    QVector<CvSeq*> contours;

    if( !contour )
    {
        return contours;
    }

    if( isQRMarker( contour, QR_MARKER_ERROR ) )
    {
        CvBox2D box = cvMinAreaRect2( contour );

        double countourArea = 0.0;
        double i1, i2 = 0.0;

        countourArea = fabs( cvContourArea( contour ) );
        i1 = countourArea / (double)( box.size.width * box.size.height );
        i2 =( ( box.size.width < box.size.height ) ?
           (double) box.size.width / (double) box.size.height:
           (double) box.size.height / (double) box.size.width );

        if( i1 >= MIN_CONTOURS_AREA_RATIO &&
            i2 >= MIN_CONTOURS_AREA_RATIO &&
            abs( box.size.width * box.size.height - countourArea ) < countourArea * 0.3 )
        {
            contours.push_back( contour );
        }
    }

    QVector<CvSeq*> contours1 = collectContours( contour->h_next );
    QVector<CvSeq*> contours2 = collectContours( contour->v_next );

    foreach( CvSeq* c, contours1 )
        contours.push_back(c);

    foreach( CvSeq* c, contours2 )
        contours.push_back(c);

    return contours;
}

bool SimpleFilter::isQRMarker( CvSeq* contour, float error  )
{
    if( !checkContourDepth( contour ) )
        return false;

    CvSeq* seq = contour;
    CvBox2D box1 = cvMinAreaRect2( seq );
    float area1 = ( box1.size.width * box1.size.height );

    seq = seq->v_next;
    CvBox2D box2 = cvMinAreaRect2( seq );
    float area2 = ( box2.size.width * box2.size.height );

    seq = seq->v_next;
    CvBox2D box3 = cvMinAreaRect2( seq );
    float area3 = ( box3.size.width * box3.size.height );

    float k1 = area1/area2;
    float k2 = area2/area3;

    if( abs( k1 - QR_MARKER_RATIO_1 ) < error && abs( k2 - QR_MARKER_RATIO_2 ) < error )
        return true;

    return false;
}
