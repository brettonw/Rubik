//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Modified by:		Jeremy Biddle
//	Last Revision:	01/30/97
//	02/23/97	DS		Added assembly additive pixel write, fixed old-style
//									EDI/ESI move, added DrawFlash and DrawAdd.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#ifdef	SIXTEENBITCOLOR

//------------------------------------------------------------------------------
#include "sprite.h"
#include "display.h"
#include "debugger.h"

//------------------------------------------------------------------------------
#define	USE_ALPHA_PIXELS	1																											//	whether or not to support transparent pixel writes

//------------------------------------------------------------------------------
Sprite::Sprite (MemFile *file, const Vector_2d &off)														//	constructor
{																																								//	begin
	Bound	bound;																																	//	bounding rectangle
	file->Read (&bound, sizeof (Bound));																					//	read the bounding box for this sprite
	Vector_2d	bottom_left (bound.left, bound.bottom);															//	compute the vector representing the bottom left corner of this sprite with respect to the bound
	offset = bottom_left - off;																										//	compute the offset from the sprite center to the bottom left corner
	width = bound.right - bound.left;																							//	compute the width of the sprite
	height = bound.top - bound.bottom;																						//	compute the number of rows of data in this sprite
	if (height > 0)																																//	if the height is positive
	{																																							//	begin
		rows = new Row[height];																											//	allocate the rows
		for (int i = 0; i < height; i++)																						//	loop over all of the rows
		{																																						//	begin
			file->Read (&rows[i].numRuns, sizeof (ulong));														//	read the number of runs in this row
			rows[i].runs = new Run*[rows[i].numRuns];																	//	allocate the run handles
			for (ulong j = 0; j < rows[i].numRuns; j++)																//	loop over all of the runs
			{																																					//	begin
				rows[i].runs[j] = (Run*) (file->CurrentPtr ());													//	set the run pointer into the buffer space
				file->Move (sizeof (Run));																							//	skip the buffer ahead
				rows[i].runs[j]->pixels = pPtr (file->CurrentPtr ());										//	set the pixel data pointer
				int		extra = rows[i].runs[j]->length * sizeof (Pixel);									//	compute the size of the pixel data
				file->Move (extra);																											//	skip to the end of the pixel data
				if (rows[i].runs[j]->alpha)																							//	if this run has alpha data
				{																																				//	begin
					rows[i].runs[j]->alpha = uPtr (file->CurrentPtr ());									//	set the alpha pointer
					file->Move (rows[i].runs[j]->length);																	//	skip to the end of the alpha data
					extra += rows[i].runs[j]->length;																			//	add the alpha data length into the extra computation
				}																																				//	end
				file->Move ((4 - (extra & 3)) & 3);																			//	skip ahead to the next long word boundary
			}																																					//	end
		}																																						//	end
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
Sprite::~Sprite (void)																													//	destructor
{																																								//	begin
	for (int i = 0; i < height; i++)																							//	loop over all of the rows
		delete[] rows[i].runs;																											//	releasing the run handles
	delete[] rows;																																//	release the rows
}																																								//	end

//------------------------------------------------------------------------------
void		Sprite::Draw (CPoint where, Bool clip)																	//	draw the sprite
{																																								//	begin
	Point_2d	start = where + offset;																							//	compute the start point point for the rendering
	int				x = int (start[X]),																									//	map the x coordinate to integer screen space
						y = int (start[Y]);																									//	map the y coordinate to integer screen space
	Bound			bound;																															//	bounding rectangle for the sprite in screen space
	bound.top = y + height;																												//	compute the top coordinate of the bound
	bound.bottom = y;																															//	set the bottom coordinate of the bound
	bound.left = x;																																//	set the left coordinate of the bound
	bound.right = x + width;																											//	compute the right coordinate of the bound
	if (clip)																																			//	if the clip flag is set
		switch (bound.TestClip (gBound))																						//	test the bound against the screen clipping bound
		{																																						//	begin
			case INSIDE:																															//	the bound is entirely inside the screen bound
				DrawUnclipped (x, y);																										//	draw the sprite without any clipping
				break;																																	//	end case
			case INTERSECTING:																												//	the bound intersects the screen bound
				DrawClipped (x, y);																											//	draw the sprite with clipping
				break;																																	//	end case
			case OUTSIDE:																															//	the sprite is completely outside the screen bound
				break;																																	//	end case
		}																																						//	end
	else																																					//	otherwise
		DrawUnclipped (x, y);																												//	draw the sprite without any clipping
}																																								//	end

//------------------------------------------------------------------------------
void	Sprite::DrawUnclipped (int x, int y)																			//	draw the sprite without any clipping
{																																								//	begin
	pPtr			pixels = gDisplay->PPtr (x, y);																			//	get a pointer to the screen space
	for (int i = 0; i < height; i++)																							//	loop over all of the rows in the sprite
	{																																							//	begin
		Row		*row = &rows[i];																											//	get a pointer to the current row
		for (ulong j = 0; j < row->numRuns; j++)																		//	loop over all of the runs in the row
		{																																						//	begin
			Run		*run = row->runs[j];																								//	get a pointer to the current run
			#if USE_ALPHA_PIXELS
			if (run->alpha)																														//	if the run has alpha data
			for (int k = 0; k < run->length; k++)																			//	loop over all of the pixels in the run
				pixels[k + run->x] = pixels[k + run->x].Scale (run->alpha[k]) + run->pixels[k];	//	scaling the existing screen pixel and adding the new one
			else																																			//	otherwise
			#endif
			{																																					//	begin
				int	len = run->length;																									//	get the run length
				long	*isrc = (long*) run->pixels,																			//	get a pointer to the source pixels
							*idst = (long*) &(pixels[run->x]);																//	get a pointer to the destination pixels

				#include "copy16jb.asm"																									//	16-bit blit

			}																																					//	end
		}																																						//	end
		pixels += gDisplay->Stride ();																							//	increment the pixel pointer to the next scanline
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void	Sprite::DrawClipped (int x, int y)																				//	draw the sprite clipped to the screen bound
{																																								//	begin
	int		yStart = 0,																															//	which row to start with
				yStop = height,																													//	which row to end with
				xStart = 0,																															//	which column to start with
				xStop = width;																													//	which column to end with
	if (y < gBound.bottom)																												//	if y is outside the viewable area
	{																																							//	begin
		yStart = gBound.bottom - y;																									//	compute the offset to the viewable area
		y = gBound.bottom;																													//	set y to the viewable area
	}																																							//	end
	if (y + yStop >= gBound.top)																									//	if the sprite will go off the top edge of the screen
		yStop = gBound.top - y;																											//	set the top of the sprite to coincide with the top of the screen
	pPtr		pixels = gDisplay->PPtr (x, y);																				//	get the pointer to the screen at x, y
	if (x < gBound.left)																													//	if the sprite is off the left edge of the viewable area
		xStart = gBound.left - x;																										//	compute the offset to the left edge
	if (x + xStop >= gBound.right)																								//	if the sprite goes off the right edge
		xStop = gBound.right - x;																										//	compute the maximum width to prevent it
	for (int i = yStart; i < yStop; i++)																					//	loop over all of the visible rows
	{																																							//	begin
		Row	*row = &rows[i];																												//	get a pointer to the current row
		for (ulong j = 0; j < row->numRuns; j++)																		//	loop over all of the runs
		{																																						//	begin
			Run		*run = row->runs[j];																								//	get a pointer to the current run
			int		runStart = 0,																												//	where to start drawing the run
						runStop = run->length;																							//	where to stop drawing the run
			if (run->x < xStart)																											//	if the run begins before the left edge of the viewable area
				runStart = xStart - run->x;																							//	advance the beginning to the left edge
			if (run->x + runStop > xStop)																							//	if the run continues off the right edge
				runStop = xStop - run->x;																								//	clip the end off to the right edge
			#if USE_ALPHA_PIXELS
			if (run->alpha)																														//	if this run contains alpha data
			for (int k = runStart; k < runStop; k++)																//	loop over all of the visible pixels
 				pixels[k + run->x] = pixels[k + run->x].Scale (run->alpha[k]) + run->pixels[k];	//	scaling down the existing pixel and blending in the new pixel
			else																																			//	otherwise, the pixel contains no alpha data
			#endif
			{																																					//	begin
				int	len = runStop - runStart;																						//	compute the clipped length
				if (len > 0)																														//	if the clipped length is greater than zero
				{																																				//	begin
					long	*isrc = (long*) &(run->pixels[runStart]),												//	get a pointer to the source pixels
								*idst = (long*) &(pixels[run->x + runStart]);										//	get a pointer to the destination pixels
					#include "copy16jb.asm"																								//	16-bit blit
				}																																				//	end
			}																																					//	end
		}																																						//	end
		pixels += gDisplay->Stride ();																							//	skip the pixel pointer to the next scan line
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void		Sprite::DrawAdd (CPoint where, Bool clip)																//	draw the sprite
{																																								//	begin
	Point_2d	start = where + offset;																							//	compute the start point point for the rendering
	int				x = int (start[X]),																									//	map the x coordinate to integer screen space
						y = int (start[Y]);																									//	map the y coordinate to integer screen space
	Bound			bound;																															//	bounding rectangle for the sprite in screen space
	bound.top = y + height;																												//	compute the top coordinate of the bound
	bound.bottom = y;																															//	set the bottom coordinate of the bound
	bound.left = x;																																//	set the left coordinate of the bound
	bound.right = x + width;																											//	compute the right coordinate of the bound
	if (clip)																																			//	if the clip flag is set
		switch (bound.TestClip (gBound))																						//	test the bound against the screen clipping bound
		{																																						//	begin
			case INSIDE:																															//	the bound is entirely inside the screen bound
				DrawAddUnclipped (x, y);																								//	draw the sprite without any clipping
				break;																																	//	end case
			case INTERSECTING:																												//	the bound intersects the screen bound
				DrawAddClipped (x, y);																									//	draw the sprite with clipping
				break;																																	//	end case
			case OUTSIDE:																															//	the sprite is completely outside the screen bound
				break;																																	//	end case
		}																																						//	end
	else																																					//	otherwise
		DrawAddUnclipped (x, y);																										//	draw the sprite without any clipping
}																																								//	end

//------------------------------------------------------------------------------
void	Sprite::DrawAddUnclipped (int x, int y)																		//	draw the sprite without any clipping
{																																								//	begin
	pPtr			pixels = gDisplay->PPtr (x, y);																			//	get a pointer to the screen space
	for (int i = 0; i < height; i++)																							//	loop over all of the rows in the sprite
	{																																							//	begin
		Row		*row = &rows[i];																											//	get a pointer to the current row
		for (ulong j = 0; j < row->numRuns; j++)																		//	loop over all of the runs in the row
		{																																						//	begin
			Run		*run = row->runs[j];																								//	get a pointer to the current run
			int	len = run->length;																										//	get the run length
			long	*isrc = (long*) run->pixels,																				//	get a pointer to the source pixels
						*idst = (long*) &(pixels[run->x]);																	//	get a pointer to the destination pixels

			#include "alpha16ds.asm"																									//	16-bit additive sprite blit
		}																																						//	end
		pixels += gDisplay->Stride ();																							//	increment the pixel pointer to the next scanline
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void	Sprite::DrawAddClipped (int x, int y)																			//	draw additively, sprite clipped to the screen bound
{																																								//	begin
	int		yStart = 0,																															//	which row to start with
				yStop = height,																													//	which row to end with
				xStart = 0,																															//	which column to start with
				xStop = width;																													//	which column to end with
	if (y < gBound.bottom)																												//	if y is outside the viewable area
	{																																							//	begin
		yStart = gBound.bottom - y;																									//	compute the offset to the viewable area
		y = gBound.bottom;																													//	set y to the viewable area
	}																																							//	end
	if (y + yStop >= gBound.top)																									//	if the sprite will go off the top edge of the screen
		yStop = gBound.top - y;																											//	set the top of the sprite to coincide with the top of the screen
	pPtr		pixels = gDisplay->PPtr (x, y);																				//	get the pointer to the screen at x, y
	if (x < gBound.left)																													//	if the sprite is off the left edge of the viewable area
		xStart = gBound.left - x;																										//	compute the offset to the left edge
	if (x + xStop >= gBound.right)																								//	if the sprite goes off the right edge
		xStop = gBound.right - x;																										//	compute the maximum width to prevent it
	for (int i = yStart; i < yStop; i++)																					//	loop over all of the visible rows
	{																																							//	begin
		Row	*row = &rows[i];																												//	get a pointer to the current row
		for (ulong j = 0; j < row->numRuns; j++)																		//	loop over all of the runs
		{																																						//	begin
			Run		*run = row->runs[j];																								//	get a pointer to the current run
			int		runStart = 0,																												//	where to start drawing the run
						runStop = run->length;																							//	where to stop drawing the run
			if (run->x < xStart)																											//	if the run begins before the left edge of the viewable area
				runStart = xStart - run->x;																							//	advance the beginning to the left edge
			if (run->x + runStop > xStop)																							//	if the run continues off the right edge
			runStop = xStop - run->x;																									//	clip the end off to the right edge
			int	len = runStop - runStart;																							//	compute the clipped length
			if (len > 0)																															//	if the clipped length is greater than zero
			{																																					//	begin
				long	*isrc = (long*) &(run->pixels[runStart]),													//	get a pointer to the source pixels
							*idst = (long*) &(pixels[run->x + runStart]);											//	get a pointer to the destination pixels

				#include "alpha16ds.asm"																								//	additive sprite blit
			}																																					//	end
		}																																						//	end
		pixels += gDisplay->Stride ();																							//	skip the pixel pointer to the next scan line
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void		Sprite::DrawFlash (CPoint where, Bool clip)															//	draw the sprite
{																																								//	begin
	Point_2d	start = where + offset;																							//	compute the start point point for the rendering
	int				x = int (start[X]),																									//	map the x coordinate to integer screen space
						y = int (start[Y]);																									//	map the y coordinate to integer screen space
	Bound			bound;																															//	bounding rectangle for the sprite in screen space
	bound.top = y + height;																												//	compute the top coordinate of the bound
	bound.bottom = y;																															//	set the bottom coordinate of the bound
	bound.left = x;																																//	set the left coordinate of the bound
	bound.right = x + width;																											//	compute the right coordinate of the bound
	if (clip)																																			//	if the clip flag is set
		switch (bound.TestClip (gBound))																						//	test the bound against the screen clipping bound
		{																																						//	begin
			case INSIDE:																															//	the bound is entirely inside the screen bound
				DrawFlashUnclipped (x, y);																							//	draw the sprite without any clipping
				break;																																	//	end case
			case INTERSECTING:																												//	the bound intersects the screen bound
				DrawFlashClipped (x, y);																								//	draw the sprite with clipping
				break;																																	//	end case
			case OUTSIDE:																															//	the sprite is completely outside the screen bound
				break;																																	//	end case
		}																																						//	end
	else																																					//	otherwise
		DrawFlashUnclipped (x, y);																									//	draw the sprite without any clipping
}																																								//	end

//------------------------------------------------------------------------------
void	Sprite::DrawFlashUnclipped (int x, int y)																	//	draw the sprite without any clipping
{																																								//	begin
	pPtr			pixels = gDisplay->PPtr (x, y);																			//	get a pointer to the screen space
	Pixel			gray (15,15,15);
	for (int i = 0; i < height; i++)																							//	loop over all of the rows in the sprite
	{																																							//	begin
		Row		*row = &rows[i];																											//	get a pointer to the current row
		for (ulong j = 0; j < row->numRuns; j++)																		//	loop over all of the runs in the row
		{																																						//	begin
			Run		*run = row->runs[j];																								//	get a pointer to the current run
			#if USE_ALPHA_PIXELS
			if (run->alpha)																														//	if the run has alpha data
			for (int k = 0; k < run->length; k++)																			//	loop over all of the pixels in the run
					pixels[k + run->x] = pixels[k + run->x].Scale (run->alpha[k]) + gray;	//	scaling the existing screen pixel and adding the new one
			else																																			//	otherwise
			#endif
			{																																					//	begin
				int	len = run->length;																									//	get the run length
				long	*isrc = (long*) run->pixels,																			//	get a pointer to the source pixels
							*idst = (long*) &(pixels[run->x]);																//	get a pointer to the destination pixels

				#include "flash16jb.asm"																								//	16-bit blit
			}																																					//	end
		}																																						//	end
		pixels += gDisplay->Stride ();																							//	increment the pixel pointer to the next scanline
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
void	Sprite::DrawFlashClipped (int x, int y)																		//	draw the sprite clipped to the screen bound
{																																								//	begin
	Pixel			gray (15,15,15);
	int		yStart = 0,																															//	which row to start with
				yStop = height,																													//	which row to end with
				xStart = 0,																															//	which column to start with
				xStop = width;																													//	which column to end with
	if (y < gBound.bottom)																												//	if y is outside the viewable area
	{																																							//	begin
		yStart = gBound.bottom - y;																									//	compute the offset to the viewable area
		y = gBound.bottom;																													//	set y to the viewable area
	}																																							//	end
	if (y + yStop >= gBound.top)																									//	if the sprite will go off the top edge of the screen
		yStop = gBound.top - y;																											//	set the top of the sprite to coincide with the top of the screen
	pPtr		pixels = gDisplay->PPtr (x, y);																				//	get the pointer to the screen at x, y
	if (x < gBound.left)																													//	if the sprite is off the left edge of the viewable area
		xStart = gBound.left - x;																										//	compute the offset to the left edge
	if (x + xStop >= gBound.right)																								//	if the sprite goes off the right edge
		xStop = gBound.right - x;																										//	compute the maximum width to prevent it
	for (int i = yStart; i < yStop; i++)																					//	loop over all of the visible rows
	{																																							//	begin
		Row	*row = &rows[i];																												//	get a pointer to the current row
		for (ulong j = 0; j < row->numRuns; j++)																		//	loop over all of the runs
		{																																						//	begin
			Run		*run = row->runs[j];																								//	get a pointer to the current run
			int		runStart = 0,																												//	where to start drawing the run
						runStop = run->length;																							//	where to stop drawing the run
			if (run->x < xStart)																											//	if the run begins before the left edge of the viewable area
				runStart = xStart - run->x;																							//	advance the beginning to the left edge
			if (run->x + runStop > xStop)																							//	if the run continues off the right edge
				runStop = xStop - run->x;																								//	clip the end off to the right edge
			#if USE_ALPHA_PIXELS
			if (run->alpha)																														//	if this run contains alpha data
			for (int k = runStart; k < runStop; k++)																	//	loop over all of the visible pixels
				pixels[k + run->x] = pixels[k + run->x].Scale (run->alpha[k]) + gray;		//	scaling down the existing pixel and blending in the new pixel
			else																																			//	otherwise, the pixel contains no alpha data
			#endif
			{																																					//	begin
				int	len = runStop - runStart;																						//	compute the clipped length
				if (len > 0)																														//	if the clipped length is greater than zero
				{																																				//	begin
					long	*isrc = (long*) &(run->pixels[runStart]),												//	get a pointer to the source pixels
								*idst = (long*) &(pixels[run->x + runStart]);										//	get a pointer to the destination pixels

					#include "flash16jb.asm"																							//	16-bit blit
				}																																				//	end
			}																																					//	end
		}																																						//	end
		pixels += gDisplay->Stride ();																							//	skip the pixel pointer to the next scan line
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
#endif	//	SIXTEENBITCOLOR
