//-----------------------------------------------------------------------------
// Copyright (C) 2002 Microsoft, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _WIDE_LINE_INL_
#define     _WIDE_LINE_INL_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _WIDE_LINE_H_
#include    "wide_line.h"
#endif  //  _WIDE_LINE_H_

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
template<class real>
uInt
BuildChainFromSegment (TChainList<real>& chainsOut, const TPoint2<real>& ptA, const TPoint2<real>& ptB, 
                        const TVector2<real>& vExpandA, const TVector2<real>& vExpandB, real rScaleA, real rScaleB, 
                        uInt iSegment, uInt recur = 0)
{
    // four points of a trapezoid
    TPoint2<real>   pt1 = ptA + (vExpandA * rScaleA);
    TPoint2<real>   pt2 = ptB + (vExpandB * rScaleB);
    TPoint2<real>   pt3 = ptB - (vExpandB * rScaleB);
    TPoint2<real>   pt4 = ptA - (vExpandA * rScaleA);
    
    // four angles, two ratios, one final
    real            a1 = acos ((pt2 - pt1).Normalized () DOT (pt3 - pt1).Normalized ());
    real            a2 = acos ((pt4 - pt3).Normalized () DOT (pt1 - pt3).Normalized ());
    real            r1 = Maximum (a1, a2) / Minimum (a1, a2);
    real            a3 = acos ((pt1 - pt2).Normalized () DOT (pt4 - pt2).Normalized ());
    real            a4 = acos ((pt3 - pt4).Normalized () DOT (pt2 - pt4).Normalized ());
    real            r2 = Maximum (a3, a4) / Minimum (a3, a4);
    real            ratio = Maximum (r1, r2) / Minimum (r1, r2);

    // if the ratio of the ratios exceeds some value, we need to do something smarter than
    // just make two triangles, otherwise it will look really bad.
    if ((ratio > 1.1) and (recur < 2))
    {
        // set up a pair of new chains
        TChain<real>    chainOut;
        
        // the half point of the trapezoid
        TVector2<real>  vExpandHalf = (vExpandA + vExpandB) * static_cast<real> (0.5);
        real            rScaleHalf = (rScaleA + rScaleB) * static_cast<real> (0.45);
        TPoint2<real>   ptHalf = ptA + ((ptB - ptA) * static_cast<real> (0.5));
        TPoint2<real>  vertexHalf (ptHalf);
        
        // recur on both halves
        BuildChainFromSegment (chainsOut, ptA, vertexHalf, vExpandA, vExpandHalf, rScaleA, rScaleHalf, iSegment, recur + 1);
        BuildChainFromSegment (chainsOut, vertexHalf, ptB, vExpandHalf, vExpandB, rScaleHalf, rScaleB, iSegment, recur + 1);
    }
    else
    {
        // set up a new chain
        TChain<real>    chainOut;
        
        // four points of a trapezoid
        chainOut.push_back (pt1);
        chainOut.push_back (pt2);
        chainOut.push_back (pt3);
        chainOut.push_back (pt4);
        
        // add the polygon to the output list
        Assert (chainOut.TestSimple ());
        chainsOut.push_back (chainOut);
    }
    
    // return the segment so it can be propogated
    return iSegment;
    
}
                        
//-----------------------------------------------------------------------------
template<class real>
uInt
ExpandLine (TChainList<real>& chainsOut, const TChain<real>& chain, real rWidth, uInt iIDStart)
{
    // only do this for lines
    Assert (not chain.GetClosed ());

    // constant describing the maximum width of a line relative to the segment
    // length
    real    rMaxLengthScalar = static_cast<real> (0.666);

    // check to see if the first and last point are the same, e.g. the open 
    // line is actually a closed poly. If the test comes back positive, move
    // the back point to make sure they match exactly
    bool    bClosed = (chain.front () == chain.back ());

    // compute a perpendicular vector and scale vector for each edge of the chain, 
    // except the  imaginary one at the end
    std::vector<TVector2<real> >    vPerpVec;
    std::vector<real>               rScaleVec;
    uInt                            iSize = static_cast<uInt> (chain.size ());
    uInt                            iStop = iSize - 1;
    vPerpVec.reserve (iStop + 2);
    rScaleVec.reserve (iStop + 2);
    for (uInt i = 0; i < iStop; i++)
    {
        // get the current point and the next point
        const TPoint2<real>&   ptA = chain[i];
        const TPoint2<real>&   ptB = chain[i + 1];
        
        // compute the vector between the points
        TVector2<real>          vDelta = ptB - ptA;
        
        // check the scale of the vector against the line width. If the line 
        // width is larger than half the length of the segment, then there's
        // a risk of producing bow-ties, or non-simple polys. We mitigate that
        // by clamping the width.
        real                    rDeltaLength = vDelta.Norm ();
        real                    rMaxWidth = rDeltaLength * rMaxLengthScalar;
        rScaleVec.push_back (Minimum (rWidth, rMaxWidth));
        
        // compute the perpendicular vector and push it onto the cache vector
        TVector2<real>          vPerp = (vDelta / rDeltaLength).Perpendicular ();
        vPerpVec.push_back (vPerp);
    }
    
    // If this is a closed poly, we will store a pair of sentinels that
    // will simplify the main expansion loop
    if (bClosed)
    {
        // for the perpendicular cache, push a copy of the front and then the 
        // back on the cache vector
        TVector2<real>  vTmp = vPerpVec.back ();
        vPerpVec.push_back (vPerpVec.front ());
        vPerpVec.push_back (vTmp);
        
        // do the same with the scale vector
        real            rTmp = rScaleVec.back ();
        rScaleVec.push_back (rScaleVec.front ());
        rScaleVec.push_back (rTmp);
    }
    else
    {
        // push two empty vectors
        TVector2<real>  vZero (0, 0);
        vPerpVec.push_back (vZero);
        vPerpVec.push_back (vZero);

        // do the same with the scale vector, just small values instead of zeroes
        rScaleVec.push_back (rWidth * static_cast<real> (0.333));
        rScaleVec.push_back (rWidth * static_cast<real> (0.333));
    }
    
    // loop over all of the points in the chain, generating a chain from each
    // segment of the chain. The first expansion vector is cached, as is the 
    // first scale factor
    TVector2<real>                  vExpandA = (vPerpVec.back () + vPerpVec.front ()).Normalized ();
    real                            rScaleA = Minimum (rScaleVec.back (), rScaleVec.front ());
    uInt                            iSegment = 0;
    for (uInt i = 0; i < iStop; i++)
    {
        // compute the "next" index
        uInt                    j = i + 1;
        
        // get the current point and the next point
        const TPoint2<real>&   ptA = chain[i];
        const TPoint2<real>&   ptB = chain[j];
        
        // get the current and next perpendicular vectors
        const TVector2<real>&   vPerpA = vPerpVec[i];
        const TVector2<real>&   vPerpB = vPerpVec[j];
        
        // compute the second expansion vector and scale factor
        TVector2<real>          vExpandB = (vPerpA + vPerpB).Normalized ();
        real                    rScaleB = Minimum (rScaleVec[i], rScaleVec[j]);
        
        // build a chain from the segment, test it for validity
        iSegment = BuildChainFromSegment (chainsOut, ptA, ptB, vExpandA, vExpandB, rScaleA, rScaleB, iSegment);
        
        // propogate the expansion vector and scale factor
        vExpandA = vExpandB;
        rScaleA = rScaleB;
    }
    
    // return the number of chains created
    return iSize;
}

//-----------------------------------------------------------------------------
template<class real>
void    
ExpandLines (TChainList<real>& chainsOut, const TChainList<real>& chainsIn, real rWidth)
{
    // set up the id counter
    uInt    iIDCounter = 0;
    
    // loop over all of the input chains
    TChainList<real>::const_iterator    iter = chainsIn.begin ();
    TChainList<real>::const_iterator    iterEnd = chainsIn.end ();
    while (iter != iterEnd)
    {
        // fetch the current chain
        const TChain<real>& chainIn = *iter++;
        
        // expand the line represented by the chain and propogate the number
        // of new chains to the id counter
        iIDCounter += ExpandLine (chainsOut, chainIn, rWidth, iIDCounter);
    }
}

//-----------------------------------------------------------------------------

#endif  //  _WIDE_LINE_INL_
