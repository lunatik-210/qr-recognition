#include "simplefilter.h"

IplImage* SimpleFilter::process( IplImage* frame )
{
    const int MAX_AZTEC_RECT = 500;

    IplImage* gray = 0;

    gray = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );

    cvCvtColor(frame,gray, CV_RGB2GRAY);

    cvThreshold(gray,gray, 100, 255, CV_THRESH_BINARY);

    //создание хранилище памяти
    CvMemStorage* storage=cvCreateMemStorage(0);

    CvSeq* firstContour = NULL;
    cvFindContours(gray,storage, &firstContour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));

    CvSeq* c_temp=0;
    CvRect Rects[MAX_AZTEC_RECT];
    int all_rects=0;
    double i,i1,i2;
    for(CvSeq* seq0 = firstContour; seq0!=0;seq0 = seq0->h_next)
    {
        CvRect r=cvBoundingRect(seq0);
        CvBox2D b=cvMinAreaRect2(seq0);
        if (b.size.height<0.1 || b.size.width<0.1 )
        {
            i1=0;
        }
        else
        {
            i=fabs(cvContourArea(seq0));
            i1=i/(double)(b.size.width*b.size.height);
            i2=((b.size.width<b.size.height)?
               (double)b.size.width/(double)b.size.height:
               (double)b.size.height/(double)b.size.width);
        }
        if(i1>=0.65 &&  i2>=0.65 && abs(b.size.width*b.size.height-i)<i*0.3) {
            c_temp=seq0;
            Rects[all_rects]=r;
            all_rects++;
            if (all_rects>MAX_AZTEC_RECT)
                break;
        }
        else
        {
            if (c_temp==0)
                firstContour=firstContour->h_next;
            else
                c_temp->h_next=c_temp->h_next->h_next;
        }
    }

    for (int indx = 0; indx < all_rects; ++indx)
    {
        CvPoint p1, p2;
        p1.x=Rects[indx].x;
        p1.y=Rects[indx].y;
        p2.x=Rects[indx].x+Rects[indx].width;
        p2.y=Rects[indx].y+Rects[indx].height;
        cvRectangle(frame,p1,p2,CV_RGB(30,216,30),1,8,0);
    }

    cvReleaseImage(&gray);
    cvReleaseMemStorage( &storage );
    return frame;
}
