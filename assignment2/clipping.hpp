#include <iostream>

typedef int OutCode;

const int INSIDE = 0; // 000000
const int LEFT = 1;   // 000001
const int RIGHT = 2;  // 000010
const int BOTTOM = 4; // 000100
const int TOP = 8;    // 001000
const int BEHIND=16  ;// 010000
const int FRONT=32   ;// 100000

const float XMIN=-1, XMAX=1.0, YMIN=-1.0, YMAX=1.0, ZMIN=-1.0, ZMAX=1.0;

OutCode ComputeOutCode(float x, float y, float z);

bool CohenSutherlandLineClipAndDraw(float &x0, float &y0, float &z0, float &x1, float &y1, float &z1);
