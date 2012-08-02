#include <QtCore/QCoreApplication>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QDebug>

#define MAX_AZTEC_RECT 500
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    IplImage* image = 0;

    IplImage* gray = 0;
    IplImage* gray1 = 0;
    IplImage* dst = 0;

    IplImage* bin = 0;

    image=cvLoadImage("images1.jpg",1);

    assert( image != 0 );

    gray = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
    gray1 = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
     dst = cvCloneImage(image);
    bin = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
    // преобразуем в градации серого
    cvCvtColor(image,gray, CV_RGB2GRAY);

    cvThreshold(gray,gray1, 100, 255, CV_THRESH_BINARY);
     //cvCvtColor(image, gray, CV_RGB2GRAY);

            // показываем картинки
                    //cvShowImage("original",image);
                    //cvShowImage("gray",gray);
                    cvShowImage("gray1",gray1);

    //создание хранилище памяти
    CvMemStorage* storage=cvCreateMemStorage(0);

    //cvInRangeS(gray, cvScalar(40), cvScalar(150), bin); // atoi(argv[2])


    CvSeq* firstContour = NULL;
    cvFindContours(gray1,storage, &firstContour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));
    //int i=0;
    // нарисуем контуры

    CvSeq* c_temp=0;
    CvRect Rects[MAX_AZTEC_RECT];
    int all_rects=0;
    double i,i1,i2;
    for(CvSeq* seq0 = firstContour; seq0!=0;seq0 = seq0->h_next)
    {
        /*CvRect con;
        CvBox2D rect;
        con=cvBoundingRect(seq0,1);

        rect=cvContourArea(seq0);
        qDebug()<<i<<con.width*con.height<<" "<<rect.size.height*rect.size.width;
        //if(con.width*con.height-square>=5 && con.width*con.height-square<=20)
          //  cvRectangle(dst,10,10, con.width,con.height);*/

      //  cvDrawContours(dst,seq0, CV_RGB(255,216,0), CV_RGB(0,0,250), 2, 1, 8); // рисуем контур
    //}

        CvRect r=cvBoundingRect(seq0);
        CvBox2D b=cvMinAreaRect2(seq0);
        if (b.size.height<0.1 || b.size.width<0.1 ) i1=0;
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
               if (all_rects>MAX_AZTEC_RECT) break;
        }
        else
        {
                if (c_temp==0) firstContour=firstContour->h_next;
                else
                        c_temp->h_next=c_temp->h_next->h_next;
        }



    }
    //seq0->

    qDebug()<<"rects"<<all_rects;
    for (int indx = 0; indx < all_rects; ++indx)
    {
        CvPoint p1, p2;
        p1.x=Rects[indx].x;
        p1.y=Rects[indx].y;
        p2.x=Rects[indx].x+Rects[indx].width;
        p2.y=Rects[indx].y+Rects[indx].height;
       cvRectangle(dst,p1,p2,CV_RGB(30,216,30),1,8,0);
        //cvDrawContours();
    }
    cvShowImage("dst",dst);



    cvReleaseImage(&image);
    cvReleaseImage(&gray);

    return a.exec();
}
