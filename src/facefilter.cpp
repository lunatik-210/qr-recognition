#include "facefilter.h"

#include "opencv/cv.h"

IplImage* FaceFilter::process( IplImage* frame )
{
    const char* face_cascade = "haarcascade_frontalface_default.xml";

    CvHaarClassifierCascade* cascade = 0;

    cascade = (CvHaarClassifierCascade*) cvLoad(face_cascade, 0, 0, 0);

    CvMemStorage* storage = cvCreateMemStorage( 0 );

    CvSeq* faces = cvHaarDetectObjects( frame, cascade, storage, 1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(40, 40), cvSize(40, 40) );

    for( int i = 0; i < ( faces ? faces->total : 0); ++i )
    {
        CvRect* r = (CvRect*)cvGetSetElem( (CvSet*)faces, i );
        CvPoint pt1, pt2;
        pt1.x = r->x;
        pt1.y = r->y;
        pt2.x = r->x + r->width;
        pt2.y = r->y + r->height;

        cvRectangle( frame, pt1, pt2, CV_RGB(255, 0, 0), 3, 8, 0 );
    }

    cvReleaseMemStorage( &storage );
    return frame;
}
