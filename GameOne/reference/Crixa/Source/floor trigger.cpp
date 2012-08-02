//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/08/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "floor trigger.h"
#include "bound.h"
#include "level file.h"
#include "text buffer.h"
#include "trigger array.h"

//------------------------------------------------------------------------------
TFloor	*gFloor;																																//	global floor trigger set

//------------------------------------------------------------------------------
class	BndNode : public ListNode																									//	bound trigger list node class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				Bound				bound;																											//	the bound for this trigger
				int					trigger;																										//	the trigger ID to set off
				int					code1;																											//	the code to pass with the trigger
				int					code2;																											//	the second code to pass with the trigger
				BndNode (int top, int lef, int bot, int rit, int id, int v1, int v2);		//	constructor
virtual	~BndNode (void);																												//	destructor
};																																							//	end

//------------------------------------------------------------------------------
BndNode::BndNode (int top, int lef, int bot, int rit, int id, int v1, int v2) :	//	constructor
bound (top, lef, bot, rit)																											//	constructor	 (continued)
{																																								//	begin
	trigger = id;																																	//	copy the trigger value
	code1 = v1;																																		//	copy the trigger code
	code2 = v2;																																		//	copy the second trigger code
}																																								//	end

//------------------------------------------------------------------------------
BndNode::~BndNode (void)																												//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
TFloor::TFloor (void)																														//	constructor
{																																								//	begin
	LevFile		lev ("Triggers");
	TxtBuffer	txt (&lev);																													//	load the trigger file
	txt >> width >> height;																												//	get the partitioning width and height
	grid = new List**[height];																										//	allocate the indexing array
	grid[0] = new List*[height * width];																					//	allocate the grid map
	for (int i = 0; i < height; i++)																							//	loop over all of the rows
	{																																							//	begin
		grid[i] = grid[0] + (i * width);																						//	set the row pointer
		for (int j = 0; j < width; j++)																							//	loop over all the cells in this row
			grid[i][j] = 0;																														//	setting the list pointer to 0
	}																																							//	end
	cstr	line;																																		//	place to get a token
	txt >> line;																																	//	get a token
	while (strcmp (line, "end"))																									//	read over all of the triggers in the file
	{																																							//	begin
		int		id,																																		//	place to get the trigger id
					code,																																	//	place to get the trigger code
					code2,																																//	place to get the second trigger code
					top,																																	//	place to get the top edge of the bound
					left,																																	//	place to get the left edge of the bound
					bottom,																																//	place to get the bottom edge of the bound
					right;																																//	place to get the right edge of the bound
		txt >> id >> code >> code2 >> top >> left >> bottom >> right >> line;				//	get the trigger data and the next line
		int		minx = left >> 6,																											//	compute the minimum bound in the grid
					maxx = right >> 6,																										//	compute the maximum bound in the grid
					miny = bottom >> 6,																										//	compute the minimum bound in the grid
					maxy = top >> 6;																											//	compute the maximum bound in the grid
		for (int i = miny; i <= maxy; i++)																					//	loop over the rows covered by the bound
			for (int j = minx; j <= maxx; j++)																				//	loop over the cells in the row covered by the grid
			{																																					//	begin
				BndNode	*node = new BndNode (top, left, bottom, right, id, code, code2);//	allocate a new bound node
				if (!grid[i][j])																												//	if the list in the cell is empty
					grid[i][j] = new List;																								//	create a new list
				grid[i][j]->InsertAfter (node);																					//	insert the nod ein the list
			}																																					//	end
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
TFloor::~TFloor (void)																													//	destructor
{																																								//	begin
	for (int i = 0; i < height; i++)																							//	loop over all of the rows
		for (int j = 0; j < width; j++)																							//	loop over all of the cells in this row
			if (grid[i][j])																														//	if there is a list in this cell
				delete grid[i][j];																											//	free the list
	delete[] grid[0];																															//	free the grid memory
	delete[] grid;																																//	free the indexing array
}																																								//	end

//------------------------------------------------------------------------------
void	TFloor::Trigger (const PiecePtr &piece)																		//	test for triggers
{																																								//	begin
	Point_2d	pt = piece->GetBody ()->TLocation ();																//	get the location of the piece
	int				x = int (pt[X]),																										//	compute the x coordinate of the piece
						y = int (pt[Y]);																										//	compute the y coordinate of the piece
	List			*list = grid[y >> 6][x >> 6];																				//	get the list of trigger bounds associated with this position
	if (list)																																			//	if the list has trigger bounds
	{																																							//	begin
		BndNode	*node = (BndNode *) list->Head ();																	//	get the head of the list
		while (node)																																//	loop over all of the nodes in the list
		{																																						//	begin
			if (node->bound.TestClip (x, y) == INSIDE)																//	if the location of the piece is inside a trigger bound
				gTrigger->Trigger (node->trigger, piece, node->code1, node->code2);			//	set that trigger
			node = (BndNode *) list->Next (node);																			//	move to the next node
		}																																						//	end
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
