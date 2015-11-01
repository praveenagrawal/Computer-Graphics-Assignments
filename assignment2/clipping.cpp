#include "clipping.hpp"

//code for 2D clipping taken from wikipedia and edited accordingly for 3D

OutCode ComputeOutCode(float x, float y, float z)
{
	OutCode code;

	code = INSIDE;          // initialised as being inside of clip window

	if (x < XMIN)           // to the left of clip window
		code |= LEFT;
	else if (x > XMAX)      // to the right of clip window
		code |= RIGHT;
	if (y < YMIN)           // below the clip window
		code |= BOTTOM;
	else if (y > YMAX)      // above the clip window
		code |= TOP;
	if (z < ZMIN)           // below the clip window
		code |= BEHIND;
	else if (z > ZMAX)      // above the clip window
		code |= FRONT;

	return code;
}

bool CohenSutherlandLineClipAndDraw(float &x0, float &y0, float &z0, float &x1, float &y1, float &z1)
{
	// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
	OutCode outcode0 = ComputeOutCode(x0, y0,z0);
	OutCode outcode1 = ComputeOutCode(x1, y1,z1);

	while (true) {
		//std::cout<<x0<<"    "<<y0<<"     "<<z0<<"  "<<std::endl;
		if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
	//		std::cout<<x0<<"    "<<y0<<"     "<<z0<<"  "<<std::endl;
  	//		std::cout<<x1<<"    "<<y1<<"     "<<z1<<"  "<<std::endl<<std::endl;
			return true;
		} else if (outcode0 & outcode1) { // Bitwise AND is not 0. Trivially reject and get out of loop
			return false;
		} else {
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge
			float x, y, z;

			// At least one endpoint is outside the clip rectangle; pick it.
			OutCode outcodeOut = outcode0 ? outcode0 : outcode1;

			// Now find the intersection point;
			if (outcodeOut & TOP) {           // point is above the clip rectangle
				x = x0 + (x1 - x0) * (YMAX - y0) / (y1 - y0);
				y = YMAX;
				z = z0 + (z1 - z0) * (YMAX - y0) / (y1 - y0);
			} else if (outcodeOut & BOTTOM) { // point is below the clip rectangle
				x = x0 + (x1 - x0) * (YMIN - y0) / (y1 - y0);
				y = YMIN;
				z = z0 + (z1 - z0) * (YMIN - y0) / (y1 - y0);
			} else if (outcodeOut & RIGHT) {  // point is to the right of clip rectangle
				y = y0 + (y1 - y0) * (XMAX - x0) / (x1 - x0);
				x = XMAX;
				z = z0 + (z1 - z0) * (XMAX - x0) / (x1 - x0);
			} else if (outcodeOut & LEFT) {   // point is to the left of clip rectangle
				y = y0 + (y1 - y0) * (XMIN - x0) / (x1 - x0);
				x = XMIN;
				z = z0 + (z1 - z0) * (XMIN - x0) / (x1 - x0);
			} else if (outcodeOut & BEHIND) {   // point is to the left of clip rectangle
				y = y0 + (y1 - y0) * (ZMIN - z0) / (z1 - z0);
				x = x0 + (x1 - x0) * (ZMIN - z0) / (z1 - z0);
				z = ZMIN;
			} else if (outcodeOut & FRONT) {   // point is to the left of clip rectangle
				y = y0 + (y1 - y0) * (ZMAX - z0) / (z1 - z0);
				x = x0 + (x1 - x0) * (ZMAX - z0) / (z1 - z0);
				z = ZMAX;
			}
			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (outcodeOut == outcode0) {
				x0 = x;
				y0 = y;
				z0 = z;
				outcode0 = ComputeOutCode(x0, y0, z0);
			} else {
				x1 = x;
				y1 = y;
				z1 = z;
				outcode1 = ComputeOutCode(x1, y1,z1);
			}
		}
	}
}
