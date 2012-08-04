#include "simplefilter.h"

#include <QVector>

IplImage* SimpleFilter::process( IplImage* frame )
{
    IplImage* gray = 0;

    gray = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );

    cvCvtColor( frame, gray, CV_RGB2GRAY );
    cvThreshold( gray, gray, 100, 255, CV_THRESH_BINARY );

    //создание хранилище памяти
    CvMemStorage* storage=cvCreateMemStorage( 0 );

    CvSeq* firstContour = NULL;
    cvFindContours( gray, storage, &firstContour, sizeof( CvContour ), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint( 0, 0 ) );

    QVector<CvRect> rects;

    for( CvSeq* seq0 = firstContour; seq0!=0; seq0 = seq0->h_next )
    {
        CvRect rect = cvBoundingRect( seq0 );
        CvBox2D box = cvMinAreaRect2( seq0 );
        CvSeq* c_temp=0;
        double countourArea = 0.0;
        double i1, i2 = 0.0;
        
        if( box.size.height < 0.1 || box.size.width < 0.1 )
        {
            i1=0;
        }
        else
        {
            countourArea = fabs( cvContourArea( seq0 ) );
            i1 = countourArea / (double)( box.size.width * box.size.height );
            i2 =( ( box.size.width < box.size.height ) ?
               (double) box.size.width / (double) box.size.height:
               (double) box.size.height / (double) box.size.width );
        }
        if( i1>=0.65 && i2>=0.65 && abs( box.size.width * box.size.height - countourArea ) < countourArea * 0.3 )
        {
            c_temp=seq0;
            rects.push_back( rect );
        }
        else
        {
            if ( c_temp == 0 )
                firstContour=firstContour->h_next;
            else
                c_temp->h_next=c_temp->h_next->h_next;
        }
    }

    for( int indx = 0; indx < rects.size(); ++indx )
    {
        CvPoint p1, p2;
        p1.x=rects[indx].x;
        p1.y=rects[indx].y;
        p2.x=rects[indx].x+rects[indx].width;
        p2.y=rects[indx].y+rects[indx].height;
        cvRectangle( frame, p1, p2, CV_RGB(30,216,30), 1, 8, 0 );
    }

    cvReleaseImage( &gray );
    cvReleaseMemStorage( &storage );
    return frame;
}
