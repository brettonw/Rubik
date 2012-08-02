//-----------------------------------------------------------------------------
// Copyright (C) 2002 Microsoft, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _WIDE_LINE_H_
#define     _WIDE_LINE_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _CHAIN_LIST_H_
#include    "chain_list.h"
#endif      _CHAIN_LIST_H_

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
template<class real>
uInt    ExpandLine (TChainList<real>& chainsOut, const TChain<real>& chain, real rWidth, uInt iIDStart);

template<class real>
void    ExpandLines (TChainList<real>& chainsOut, const TChainList<real>& chainsIn, real rWidth);

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _WIDE_LINE_INL_
#include    "wide_line.inl"
#endif  //  _WIDE_LINE_INL_

//-----------------------------------------------------------------------------

#endif  //  _WIDE_LINE_H_
