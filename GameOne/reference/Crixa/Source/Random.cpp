//------------------------------------------------------------------------------
//	Copyright �1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/15/97
//
//  02/21/97				MK					Contains Dave's 1023 bugfix
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "random.h"

//------------------------------------------------------------------------------
int			gRandomTable[1024] = {																									//	a pre-computed table of random 10-bit integers with a uniform distribution
																 801,    5,  144,  396,  904,  311,  650,  865, 
																 307,  243,  183,  504,  208,   64,  335,  755, 
																 592,  357,  430,  249,  482,  420,  816,  387, 
																 435,  273,  528,  399,  359,  131,  848,  512, 
																 169,   52,  914,  393,  327,  177,  268,  155, 
																	34,  118,   45,  271,  121,  109,  235, 1003, 
																 402,   70, 1016,   41,  325,  610,  253,  166, 
																 187,  103,  230,  590,  705,  817,  536,  688, 
																 495,  866,   15,  619,  572,  481,  159,  222, 
																 656,  601,  570,  884,  574,  953,  167,  239, 
																 600,  501,  588,  497,   21,  578,  603,  710, 
																 641,  218,  724,   75,  318,  636,  644,  798, 
																 948,  473,  401,  454,  791,  101,  221,  558, 
																 205,  391,  358,  237,  586,  405,  941,  339, 
																 971,  227,   65,   87,  606,   42,   84,  291, 
																 721,  533,  270,  139,  583,  368,  199,  351, 
																 443,  763,  733,  872,  520,  305,  400,  556, 
																 891,  680,  489,  217,  831,  310,  145,  604, 
																 950,  382,  264,  352,  648,  353,  559,  229, 
																 386,  840,  510,  598,  787,  678,  614,  452, 
																 116,  768,  456,  555,  784,  929,  448,  344, 
																 634,  203,  779,  615,  254,  994,  300,  479, 
																 449,  547,  546,  275,   32,  771,  623,  762, 
																 901,  289,  676,   55,  937,  880,  444,  189, 
																	68,  326,  341,  663,  595,  480,  576,  946, 
																 524,   10,  854,  212,   17,  756,  509,  498, 
																 128,  917,  115,  736,  943,  822,    4,  836, 
																 627,  197,   58,  653,  515,  414,  261,  248, 
																 870,  523,  284,  478,  487,  364,  694,  292, 
																 697, 1000,  751,  889,  631,  531,  620,  958, 
																 303,  138,  522,  934,  898,  477,  492,   79, 
																 485,  450,  338,  258,  157,  332,  679,  731, 
																 141,  821,  252,  102,  527,  861,   19,  195, 
																 294,  200, 1022,  373,  130,  925,  911,  563, 
																 436,  802,   82,  534,  740,  476,  309,  394, 
																 876,  566,  754,  342,   73,    1,   26,  959, 
																 758,   47,  677,  397,  875,  201,  913,  296, 
																 687,  240,  225,  389,  571,  375,  334,  283, 
																	50,  367,  972,  462,  730,  549,  827,  774, 
																	12,  226,   85,  594,  637,  180,  716,  589, 
																 998,  824,  123,  398,  608,  360,  355,  905, 
																 753,  417,  702,  786,  415,  613,  939,  331, 
																 545,  422,  910,  767,   36,   60,  470,  191, 
																 713,  411,  224,  952,  899,  377,   77,  297, 
																 966,  684,  895,  922,  503,   18,  657,  863, 
																 392,  673,  120,  366,  814,  277,  649,  838, 
																 796,  263,  433,  257, 1001,  246,  992,  698, 
																 324,  537,  328,  621,  471,  385,    6,  820, 
																 954,  370,  418,  919,  441,  790,  961,  490, 
																 887,  407,  609,  213,  833,  538,  776,  381, 
																 551,  108,  269,  171,  267,  564,  945,  544, 
																 293,  308,  500,  419,  165,  599,  686,  260, 
																 760,  920,  890,  193,  882,  136,  541,  903, 
																 837,  562,  807,  742,  769,  458,  376,  874, 
																 745,  773,  855,  718,  439,   76,  535,  176, 
																 142,  612,  256,  161,    7,  560,  726,  850, 
																 681,  670,  846,  706,   93, 1017,  741,  980, 
																 186,  349,  126,  287,  216,  276,  274,  862, 
																 967,  125,  739,  517,  965,  379,  117,   29, 
																 540,  616,  255,   99,   72,  734,  893,  231, 
																	80,  611,  174,  605,  148,  146,  577,  618, 
																 404,  211,  461,  429,   57,  962,  852,   33, 
																 928,  879,  179,  374,  475,  597,  206, 1021, 
																 340,  750,  371,  290,  567,  132,  154,  207, 
																	53,  285,  539,  830,  639,  251,  819,  472, 
																 960,   31,  864,  457,  346,  974, 1002,  997, 
																 581,  692,  337,  701,  949,  593,  210,  265, 
																 162,  645,  315,  336,  104,  973,  795,  552, 
																 607,  423,  306,  635,  737, 1008,  812,  808, 
																	90,  369,   91,  575,  667,  483,  168,  112, 
																1004,  432,  172,  856,  232,   66,  173,  488, 
																 584,  119,  313,  455,  728,  521,   81,  474, 
																 466,   14,  777,  990,  279,  711,  259,  190, 
																	 9,  518,  782,  907,  561,  638,   44,   43, 
																 993,  775,  348,  175,  233,  977,  506,  640, 
																	23,  970,  323,  362,  446,  319,   74,  280, 
																	56,  909,   63,  322,  424,  530,  999,   69, 
																 298,  630,  519,  111,  525,  406,   88,  152, 
																 732,  416,  940,  410,  826,  857,  182,  885, 
																 484,  185,  282,  930,  847,  133,  923,   96, 
																 915,   92,  542,  314,  709,  996,  330,  818, 
																 587,  361,  460,  427,  828,  931,  659,  317, 
																 591,   22, 1019,  868,  301,   35,  156,  666, 
																 247,  143,  683,  655,  453,  409,  299,   13, 
																 661,  345,  508,  695,   16,  696,  878,  198, 
																 654,  122,  708,  568,  675,  794,   37,  918, 
																 746,  984,  204,  896,  526,   28,  150,  278, 
																 906,  502,  877,  963,  356,  976,  505,  202, 
																 565,  699,  809,   20,  288,   11,  722,  806, 
																	30,  693,  209,  803,  388,  829,  378,  241, 
																 969,  908,  835,   62,  464,  646,   97,  886, 
																 596,  469,  759,   78,  811,  272,  262,  114, 
																 219,   95,  707,  105,  989,  785,  714,  749, 
																 516,  622,  957,   39,  900,  844,  968,  140, 
																 110,  580,  942,  888,  178,  662,  383,  797, 
																 799,  764,  978, 1006,  135,  719,  765,  858, 
																 981,  892,  181,  813,  192,  626,  867,  463, 
																 842,  223,  781,  982,  715,  428,  158,   40, 
																 651,  163,  421,  553,  815,  859,  810, 1023, 
																 543,   54,  160,  991,  228,  772,  869,  629, 
																 727,  778,  987,  924,   86,  220,  425,  354, 
																 964,  107,  266,  672,  951,  825,  894,  250, 
																 682,  897,  343,  496,  196,  569,    2,  800, 
																 832,  783,  153,  853,  625,  624,  703,  789, 
																 843,   49,  617,   67,  238,  511,   27,   25, 
																1018,  632,  426,  860,   48,  757,   24,  281, 
																 995, 1010,  532,  554,  493,   59,  689,  245, 
																 236,  124,  955,  304,  720,  215,  752,  513, 
																 985,  647,  286,  365,  912,    8,  902,   38, 
																 316,  467,  986,  927,  347,  579, 1014,  921, 
																 849,  793,  302,  134,    3,   83,  188,  660, 
																 979,  738,  431,  451,  685,  602,  127,  412, 
																 873,   94,  735,  350,   61,  944,  747, 1009, 
																 573,  841,  550,  491,  669,  975,  668, 1020, 
																 671, 1013,  372,  665,  459,  403,  845,   98, 
																 447,  100,  529, 1005,  916,  557,  395,  129, 
																 935,   51,  507,  548,  329,  700,  585,  137, 
																 234,  147,  333,  936,  851,  486,  434,  658, 
																 712,  933,  788,  384,  164,  988,  437,  642, 
																 244, 1012,  494,  214,  468,  440,  664,  312, 
																1011,  643,  883,  780,  834,  321,  766,  805, 
																 729,  983,  823,  113, 1023,  743,  380,  170, 
																 242,  194,  438, 1007,  445,  871, 1015,  413, 
																 748,  792,  674,  514,  804,  725,  690,  184, 
																 881,  408,  320,   89,  717,  926,  691,  465, 
																 106,  704,  628,  839,  295,   46,  442,  633, 
																 582,  938,  363,  947,  770,  151,  652,  744, 
																 149,  932,  956,  761,  499,   71,  390,  723
															};

//------------------------------------------------------------------------------
#ifdef	USE_RANDOM_TABLE
QRandom	gRandom (1024, gRandomTable);																						//	global random number generator with 10 random bits
#else
QRandom	gRandom (10);																														//	global random number generator with 10 random bits
#endif
QRandom gLocalRandom (1024, gRandomTable);																			//	use this to avoid network sync problems (visual only)

//------------------------------------------------------------------------------
QRandom::QRandom (int bits)																											//	constructor
{																																								//	begin
	srand (ulong (time (0)));																											//	initialize the random number generator from the clock
	size = 1 << bits;																															//	compute the size of the lookup table
	mask = size - 1;																															//	compute the mask
	index = 0;																																		//	start the index at 0
	table = new Couple[size];																											//	allocate the table
	for (int i = 0; i < size; i++)																								//	loop over all of the table elements
		table[i].integer = i + 1;																										//	assigning a regular distribution to them
	bits = (16 - bits) >> 1;																											//	adjust the bits value to help get random bits from the "rand" function
	for (i = 0; i < size; i++)																										//	loop over all of the cells again
	{																																							//	begin
		int		j = (rand () >> bits) & mask,																					//	compute a random index into the table
					tmp = table[j].integer;																								//	save the value at that index
		table[j].integer = table[i].integer;																				//	move the ith value to the randomly indexed slot
		table[i].integer = tmp;																											//	store the randomly indexed value in the ith slot
	}																																							//	end
	Real	multiplier = R(1.0) / Real (size);																			//	compute the multiplier
	for (i = 0; i < size; i++)																										//	loop over all of the cells again
		table[i].real = table[i].integer * multiplier;															//	compute the value of the real component of the couple
}																																								//	end

//------------------------------------------------------------------------------
QRandom::QRandom (int siz, int *tab)																						//	constructor
{																																								//	begin
	size = siz;																																		//	copy the table size
	mask = size - 1;																															//	compute the mask
	index = 0;																																		//	start the index at 0
	table = new Couple[size];																											//	allocate the table
	Real	multiplier = R(1.0) / Real (size);																			//	compute the multiplier
	for (int i = 0; i < size; i++)																								//	loop over all of the table elements
	{																																							//	begin
		table[i].integer = tab[i];																									//	copying from the external table
		table[i].real = table[i].integer * multiplier;															//	compute the value of the real component of the couple
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
QRandom::~QRandom (void)																												//	destructor
{																																								//	begin
	delete[] table;																																//	release the table
}																																								//	end

//------------------------------------------------------------------------------
