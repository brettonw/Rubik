//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/16/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "star sequence.h"
#include "random.h"
#include "display.h"
#include "bound.h"

//------------------------------------------------------------------------------
StarSeq::StarSeq (Real x, Real y, Real par, Real den, Real bright)							//	constructor
{																																								//	begin
	parallax = par;
	density = den;
	brightness = bright;
	Vector_2d	worldSize = Point_2d (x, y) * parallax;
	width = int (worldSize[X] / density);
	height = int (worldSize[Y] / density);
	star = new Point_2d*[height];
	star[0] = new Point_2d[width * height];
	for (int i = 0; i < height; i++)
	{
		star[i] = star[0] + (i * width);
		for (int j = 0; j < width; j++)
			star[i][j] = (Point_2d (j + gRandom.RndReal (), i + gRandom.RndReal ()) * density);
	}
	density = R(1.0) / density;
}																																								//	end

//------------------------------------------------------------------------------
StarSeq::~StarSeq (void)																												//	destructor
{																																								//	begin
	delete[] star[0];
	delete[] star;
}																																								//	end

//------------------------------------------------------------------------------
Bool	StarSeq::Step (CPoint where)																							//	draw the next frame
{																																								//	begin
	int	br = int (brightness * R(31.0));
	int		minx = int (where[X] * density),
				maxx = minx + int (SCRN_HEIGHT * density) + 1,
				miny = int (where[Y] * density),
				maxy = miny + int (SCRN_HEIGHT * density) + 1;
	if (minx < 0) minx = 0;
	if (maxx >= width) maxx = width - 1;
	if (miny < 0) miny = 0;
	if (maxy >= height) maxy = height - 1;
	for (int i = miny; i < maxy; i++)
		for (int j = minx; j < maxx; j++)
		{
			Point_2d	loc = star[i][j] - where;
			int				x = int (loc[X]),
								y = int (loc[Y]);
			if (gBound.TestClip (x, y) == INSIDE)
				(*gDisplay) (x, y) = MakePixel (br, br, br);
		}
	return TRUE;
}																																								//	end

//------------------------------------------------------------------------------
