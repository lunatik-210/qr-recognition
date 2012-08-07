#ifndef CONSTANTS_H
#define CONSTANTS_H

/********************
// QR Pattern
// 1 | 1 | 3 | 1 | 1
/////////////////////
// 7*7 = 49
// 5*5 = 25
// 3*3 = 9
//
// 49 / 25 ~= 1.96
// 25 / 9 ~= 2.78
*********************/

const float QR_MARKER_RATIO_1 = 1.96;

const float QR_MARKER_RATIO_2 = 2.778;

const float QR_MARKER_ERROR = 0.2;

const float MIN_CONTOURS_AREA_RATIO = 0.65;

#endif // CONSTANTS_H
