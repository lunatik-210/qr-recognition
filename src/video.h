#ifndef VIDEO_H
#define VIDEO_H

#include "source.h"

#include <QString>

#include <opencv/highgui.h>

class Video : public Source
{    
public:
    Video( const QString& filename );

    virtual void init();

    virtual IplImage* frame();

    virtual ~Video();

protected:
    QString m_fileName;

    double m_width;

    double m_height;

    CvCapture* m_capture;

};

#endif // VIDEO_H
