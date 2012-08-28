#include "qrpatternfilter.h"

#include "opencv/cv.h"

#include <QDebug>

IplImage* QrPatternFilter::process( IplImage* frame )
{
    IplImage* gray = 0;

    gray = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );

    cvCvtColor( frame, gray, CV_RGB2GRAY );

    cvThreshold( gray, gray, 135, 255, CV_THRESH_BINARY );

    int count = 0;
    int prevPixel = gray->imageData[ 0 ];

    QVector<Block> blocks;
    QVector<Block> qrMarkers;

    for( int i = 0; i < gray->width; ++i )
    {
        for( int j = 0; j < gray->height; ++j )
        {
            int pixelColor = gray->imageData[ i + gray->widthStep * j ];
            if( prevPixel == pixelColor )
            {
                ++count;
            }
            else
            {
                Block block;
                block.color = pixelColor;
                block.count = count;
                block.x = i;
                block.y = j - count;

                blocks.push_back( block );

                count = 1;
                prevPixel = pixelColor;
            }
        }
        count = 0;
    }

    bool isQRMarker = true;

    for( int i = 0; i<blocks.size()-4; ++i )
    {
        if( abs( (float)(blocks[i+2].count) / (float)(blocks[i].count) - 3 ) > 0.1 )
        {
            isQRMarker = false;
        }
        if( abs( (float)(blocks[i+2].count) / (float)(blocks[i+1].count) - 3 ) > 0.1 )
        {
            isQRMarker = false;
        }
        if( abs( (float)(blocks[i+2].count) / (float)(blocks[i+3].count) - 3 ) > 0.1 )
        {
            isQRMarker = false;
        }
        if( abs( (float)(blocks[i+2].count) / (float)(blocks[i+4].count) - 3 ) > 0.1 )
        {
            isQRMarker = false;
        }
        if( ( blocks[i+0].color != -1 ) ||
            ( blocks[i+1].color != 0  ) ||
            ( blocks[i+2].color != -1 ) ||
            ( blocks[i+3].color != 0  ) ||
            ( blocks[i+4].color != -1 )   )
        {
            isQRMarker = false;
        }

        if( isQRMarker == false )
        {
            isQRMarker = true;
            qrMarkers.push_back(blocks[i+2]);
            continue;
        }

        CvPoint p1 = cvPoint(blocks[i+2].x-2, blocks[i+2].y-2);
        CvPoint p2 = cvPoint(blocks[i+2].x+2, blocks[i+2].y+2);

        cvDrawRect( frame, p1, p2, CV_RGB(0,0,250), 2, 8, 0 );
    }

    return frame;
}
