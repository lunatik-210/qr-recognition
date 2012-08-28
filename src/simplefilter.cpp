#include "simplefilter.h"
#include "constants.h"

#include <QVector>
#include <QDebug>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>

IplImage* SimpleFilter::process( IplImage* frame )
{
    IplImage* gray = 0;

    gray = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );

    cvCvtColor( frame, gray, CV_RGB2GRAY );

    cvThreshold( gray, gray, 135, 255, CV_THRESH_BINARY );

    //cvLaplace( gray, gray, 1 );

    /*
    for( int i = 0; i < gray->width; ++i )
    {
        for( int j = 0; j < gray->height; ++j )
        {
            if( gray->imageData[ i + gray->widthStep * j] < 130 )
            {
                gray->imageData[ i + gray->widthStep * j] = 255;
            }
            else
            {
                gray->imageData[ i + gray->widthStep * j] = 0;
            }
        }
    }
    */

    //return gray;

    //cvErode( gray, gray, NULL, 1 );

    //if( gray->imageData[ i + gray->widthStep * j] == 0)

    //cvLaplace( gray, gray, 1 );

    //return gray;

    //return gray;

    CvMemStorage* storage = cvCreateMemStorage( 0 );

    CvSeq* root = NULL;
    cvFindContours( gray, storage, &root, sizeof( CvContour ), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint( 0, 0 ) );

    QVector<CvSeq*> contours = collectContours( root );

    QVector<Vertex> qrCodes = findQrCode( contours );

    drawDebugInfo( frame, qrCodes, contours );

    cvReleaseImage( &gray );
    cvReleaseMemStorage( &storage );
    return frame;
}

void SimpleFilter::processKeyPressed( const char key )
{
    switch( key )
    {
        case KEY_A:
            errorAngle -= 0.05;
            break;
        case KEY_Q:
            errorAngle += 0.05;
            break;
        case KEY_W:
            errorSide += 0.05;
            break;
        case KEY_S:
            errorSide -= 0.05;
            break;
        default:
            break;
    }
}

void SimpleFilter::drawDebugInfo( IplImage* frame, QVector<Vertex> qrCodes, QVector<CvSeq*> contours )
{
    CvPoint g;
    g.x=30;
    g.y=30;
    CvFont sized;
    cvInitFont(&sized, CV_FONT_HERSHEY_COMPLEX_SMALL, 1.0, 1.0);
    cvPutText( frame, (QString::number(errorAngle)).toAscii(), g, &sized, CV_RGB( 0, 255, 255 ) );

    g.x=30;
    g.y=90;
    cvPutText( frame, (QString::number(errorSide)).toAscii(), g, &sized, CV_RGB( 0, 255, 255 ) );


    /////////////////////////////////////////////////////
    for( int indx = 0; indx < contours.size(); ++indx )
    {
        cvDrawContours(frame, contours[indx], CV_RGB(255,216,0), CV_RGB(0,0,250), 0, 2, 8);
    }

    /////////////////////////////////////////////////////
    qDebug()<<qrCodes.size();

    for(int i=0;i<qrCodes.count();i++)
    {
        qDebug()<<"POINT: "<<qrCodes.count();
        cvDrawLine( frame, qrCodes.at(i).p1, qrCodes.at(i).p2, CV_RGB(30,216,30), 2, 8, 0 );
        cvDrawLine( frame, qrCodes.at(i).p1, qrCodes.at(i).p3, CV_RGB(30,216,30), 2, 8, 0 );
        cvDrawLine( frame, qrCodes.at(i).p2, qrCodes.at(i).p3, CV_RGB(30,216,30), 2, 8, 0 );
    }
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

CvPoint SimpleFilter::getCenterPoint(CvSeq *contour)
{
    CvPoint point;
    CvSeq* seq = contour;

    CvBox2D box1 = cvMinAreaRect2( seq );
    seq=seq->v_next;

    CvBox2D box2=cvMinAreaRect2( seq );
    seq=seq->v_next;

    CvBox2D box3=cvMinAreaRect2( seq );

    point.x=(box1.center.x+box2.center.x+box3.center.x)/3;
    point.y=(box1.center.y+box2.center.y+box3.center.y)/3;
    return point;
}

QVector<Vertex> SimpleFilter::findQrCode( const QVector<CvSeq *> &contours )
{
    QVector<Vertex> qrCodes;

    if(contours.size()<3)
        return qrCodes;

    for( int indx=0; indx<contours.size()-2; indx++ )
    {
        for( int indx2=indx+1; indx2<contours.size()-1; indx2++ )
        {
            for( int indx3=indx2+1; indx3<contours.size(); indx3++ )
            {
                bool find=false;

                CvPoint cp1,cp2,cp3;
                cp1=getCenterPoint( contours[indx] );
                cp2=getCenterPoint( contours[indx2] );
                cp3=getCenterPoint( contours[indx3] );

                double lenght1 = 0, lenght2 = 0, lenght3 = 0;
                lenght1=getLenghtLine( cp1, cp2 );
                lenght2=getLenghtLine( cp2, cp3 );
                lenght3=getLenghtLine( cp1, cp3 );

                lenght1=pow( lenght1, 2 );
                lenght2=pow( lenght2, 2 );
                lenght3=pow( lenght3, 2 );

                if( lenght1 > lenght2 && lenght1 > lenght3 )
                {
                    find = isEquilateralTriangle( lenght1, lenght2, lenght3 );
                }
                else if( lenght2 > lenght1 && lenght2 > lenght3 )
                {
                    find = isEquilateralTriangle( lenght2, lenght1, lenght3 );
                }
                else if( lenght3 > lenght1 && lenght3 > lenght1 )
                {
                    find = isEquilateralTriangle( lenght3, lenght1, lenght2 );
                }

                if( !find )
                    return qrCodes;

                Vertex buf;
                buf.con1 = contours[indx];
                buf.con2 = contours[indx2];
                buf.con3 = contours[indx3];

                buf.p1 = cp1;
                buf.p2 = cp2;
                buf.p3 = cp3;

                qrCodes.push_back( buf );

            }
        }
    }

    return qrCodes;
}

double SimpleFilter::getLenghtLine( CvPoint p1, CvPoint p2 )
{
    return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}

bool SimpleFilter::isEquilateralTriangle( float side1, float side2, float side3 )
{
    return ( abs( side1-side2-side3 ) < side2 * errorAngle && abs( side2 - side3 ) < side2 * errorSide );
}
