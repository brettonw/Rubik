//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/28/97
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "partition sequence.h"
#include "area.h"

//------------------------------------------------------------------------------
#define	PARTITION_SIZE	160
#define	PARTITION_SCALE	(R(1.0) / R(160.0))

//------------------------------------------------------------------------------
class	StatSeqNode : public ListNode																							//	stationary sequence list node class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				SeqPtr			sequence;
				StatSeqNode (const SeqPtr &seq);																				//	constructor
virtual	~StatSeqNode (void);																										//	destructor
};																																							//	end

//------------------------------------------------------------------------------
inline	StatSeqNode::StatSeqNode (const SeqPtr &seq)														//	constructor
{																																								//	begin
	sequence = seq;
}																																								//	end

//------------------------------------------------------------------------------
StatSeqNode::~StatSeqNode (void)																								//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
PartitionSeq::PartitionSeq (int w, int h)																				//	constructor
{																																								//	begin
	width = (w / PARTITION_SIZE) + ((w % PARTITION_SIZE) ? 1 : 0);
	height = (h / PARTITION_SIZE) + ((h % PARTITION_SIZE) ? 1 : 0);
	grid = new List* [height];
	grid[0] = new List[width * height];
	for (int i = 0; i < height; i++)
		grid[i] = grid[0] + (i * width);
}																																								//	end

//------------------------------------------------------------------------------
PartitionSeq::~PartitionSeq (void)																												//	destructor
{																																								//	begin
	delete[] grid[0];
	delete[] grid;
}																																								//	end

//------------------------------------------------------------------------------
Bool	PartitionSeq::Step (CPoint where)																							//	draw the next frame
{																																								//	begin
	int		loc[2],
				min[2],
				max[2];
	Point_2d	wh = where + gCenter;
	loc[X] = int (wh[X] * PARTITION_SCALE);
	loc[Y] = int (wh[Y] * PARTITION_SCALE);
	#if	SCRN_HEIGHT <= 600
	min[X] = loc[X] - 2;
	min[Y] = loc[Y] - 2;
	max[X] = loc[X] + 2;
	max[Y] = loc[Y] + 2;
	#else
	min[X] = loc[X] - 3;
	min[Y] = loc[Y] - 3;
	max[X] = loc[X] + 3;
	max[Y] = loc[Y] + 3;
	#endif

	if (min[X] < 0)
		min[X] = 0;
	if (min[Y] < 0)
		min[Y] = 0;
	if (min[X] >= width)
		min[X] = width - 1;
	if (min[Y] >= height)
		min[Y] = height - 1;

	if (max[X] < 0)
		max[X] = 0;
	if (max[Y] < 0)
		max[Y] = 0;
	if (max[X] >= width)
		max[X] = width - 1;
	if (max[Y] >= height)
		max[Y] = height - 1;
	
	int		tag = GetTickCount ();
	for (int j = min[Y]; j <= max[Y]; j++)
		for (int i = min[X]; i <= max[X]; i++)
		{
			StatSeqNode	*node = (StatSeqNode	*) grid[j][i].Head ();
			while (node)
			{
				StatSeq	*stat = (StatSeq *) node->sequence ();
				if (stat->tag != tag)
				{
					stat->Step (where);
					stat->tag = tag;
				}
				node = (StatSeqNode	*) grid[j][i].Next (node);
			}
		}
	return TRUE;
}																																								//	end

//------------------------------------------------------------------------------
void	PartitionSeq::Install (StatSeq *seq)																						//	add a sequence to the partitioning
{																																								//	begin
	Point_2d	loc = seq->Location ();
	int				min[2],
						max[2];
	Real			seqw = Real (seq->Width ()) * R(0.5),
						seqh = Real (seq->Height ()) * R(0.5);

	min[X] = int ((loc[X] - seqw) * PARTITION_SCALE);
	min[Y] = int ((loc[Y] - seqh) * PARTITION_SCALE);
	max[X] = int ((loc[X] + seqw) * PARTITION_SCALE);
	max[Y] = int ((loc[Y] + seqh) * PARTITION_SCALE);

	if (min[X] < 0)
		min[X] = 0;
	if (min[Y] < 0)
		min[Y] = 0;
	if (min[X] >= width)
		min[X] = width - 1;
	if (min[Y] >= height)
		min[Y] = height - 1;

	if (max[X] < 0)
		max[X] = 0;
	if (max[Y] < 0)
		max[Y] = 0;
	if (max[X] >= width)
		max[X] = width - 1;
	if (max[Y] >= height)
		max[Y] = height - 1;
	
	for (int j = min[Y]; j <= max[Y]; j++)
		for (int i = min[X]; i <= max[X]; i++)
			grid[j][i].InsertAfter (new StatSeqNode (seq));
}																																								//	end

//------------------------------------------------------------------------------
