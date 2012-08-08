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

    cvThreshold( gray, gray, 90, 255, CV_THRESH_BINARY );

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
        cvRectangle( frame, p1, p2, CV_RGB(30,216,30), 2, 8, 0 );
    }
    int key=cvWaitKey(1);
            if(key==97)er2-=0.05;
            if(key==113)er2+=0.05;
            if(key==119)er1+=0.05;
            if(key==115)er1-=0.05;
            CvPoint g;
            g.x=30;
            g.y=30;
            CvFont sized;
            cvInitFont(&sized, CV_FONT_HERSHEY_COMPLEX_SMALL, 1.0, 1.0);
            cvPutText(frame,(QString::number(er2)).toAscii(),g,&sized,CV_RGB(0,255,255));

            g.x=30;
            g.y=90;
            cvPutText(frame,(QString::number(er1)).toAscii(),g,&sized,CV_RGB(0,255,255));
    frame=findQrCode(contours,frame,er2,er1);

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

double SimpleFilter::getLenghtLine(CvPoint p1, CvPoint p2)
{
    return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}


IplImage*  SimpleFilter::findQrCode(const QVector<CvSeq *> &contours, IplImage* frame,float errorAngle, float errorSide)
{

    QVector <vertex> qrCode;

    if(contours.size()<3)
        return frame;

    for( int indx=0; indx<contours.size()-2; indx++ )
    {
        for( int indx2=indx+1; indx2<contours.size()-1; indx2++ )
        {
            for( int indx3=indx2+1; indx3<contours.size(); indx3++ )
            {
                bool find=false;
                CvPoint cp1,cp2,cp3;
                cp1=getCenterPoint(contours[indx]);
                cp2=getCenterPoint(contours[indx2]);
                cp3=getCenterPoint(contours[indx3]);

                double lenght1=0,lenght2=0,lenght3=0;
                lenght1=getLenghtLine(cp1,cp2);
                lenght2=getLenghtLine(cp2,cp3);
                lenght3=getLenghtLine(cp1,cp3);

                lenght1=pow(lenght1,2);
                lenght2=pow(lenght2,2);
                lenght3=pow(lenght3,2);

                if( lenght1 > lenght2 && lenght1 > lenght3 )
                {
                    if( abs( lenght1-lenght2-lenght3 ) < lenght2 * errorAngle && abs( lenght2 - lenght3 ) < lenght2 * errorSide )
                        find=true;
                }
                else
                {
                    if( lenght2 > lenght1 && lenght2 > lenght3 )
                    {
                        if( abs( lenght2 - lenght1 - lenght3 ) < lenght1 * errorAngle && abs( lenght1 - lenght3 ) < lenght1 * errorSide )
                            find=true;
                    }
                    else
                    {
                        if( lenght3 > lenght1 && lenght3 > lenght1 )
                        {
                            if( abs( lenght3-lenght1-lenght2 ) < lenght1 * errorAngle && abs( lenght2-lenght1 )< lenght2 * errorSide )
                                find=true;
                        }
                    }
                }

                if(find)
                {
                    vertex buf;
                    buf.con1=contours[indx];
                    buf.con2=contours[indx2];
                    buf.con3=contours[indx3];

                    buf.p1=cp1;
                    buf.p2=cp2;
                    buf.p3=cp3;

                    qrCode.push_back(buf);
                }

            }
        }
    }

    qDebug()<<qrCode.size();

    for(int i=0;i<qrCode.count();i++)
    {
    qDebug()<<"POINT: "<<qrCode.count();
        cvDrawLine(frame,qrCode.at(i).p1,qrCode.at(i).p2,CV_RGB(30,216,30),2,8,0);
        cvDrawLine(frame,qrCode.at(i).p1,qrCode.at(i).p3,CV_RGB(30,216,30),2,8,0);
        cvDrawLine(frame,qrCode.at(i).p2,qrCode.at(i).p3,CV_RGB(30,216,30),2,8,0);
    }
    return frame;
}
