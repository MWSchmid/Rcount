// ==========================================================================
//                 SeqAn - The Library for Sequence Analysis
// ==========================================================================
// Copyright (c) 2006-2013, Knut Reinert, FU Berlin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Knut Reinert or the FU Berlin nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL KNUT REINERT OR THE FU BERLIN BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ==========================================================================
// Author: Tobias Rausch <rausch@embl.de>
// ==========================================================================

#ifndef SEQAN_CORE_INCLUDE_SEQAN_ALIGN_ALIGN_CONFIG_H_
#define SEQAN_CORE_INCLUDE_SEQAN_ALIGN_ALIGN_CONFIG_H_

namespace seqan {

// ============================================================================
// Forwards
// ============================================================================

// ============================================================================
// Tags, Classes, Enums
// ============================================================================

// ----------------------------------------------------------------------------
// Class AlignConfig
// ----------------------------------------------------------------------------

/**
.Class.AlignConfig:
..cat:Alignments
..summary:The AlignConfig class encapsulates how DP is carried out. 
It indicates at what ends gaps are free, the so-called free ends-space alignments.
..signature:AlignConfig<bool TOP, bool LEFT, bool RIGHT, bool BOTTOM, TSpec>
..param.TOP:If true then 0's in top row.
...default:$false$
..param.LEFT:If true then 0's in the left row.
...default:$false$
..param.RIGHT:If true then maximum is also searched in the last column.
...default:$false$
..param.BOTTOM:If true then maximum is also searched in the last row.
...default:$false$
..param.TSpec:The specializing type.
...metafunction:Metafunction.Spec
...default:$Default$, see @Tag.Default@.
..include:seqan/align.h
*/

template <bool TOP = false, bool LEFT = false, bool RIGHT = false, bool BOTTOM = false, typename TSpec = Default>
class AlignConfig
{
};

// ============================================================================
// Metafunctions
// ============================================================================

// ============================================================================
// Functions
// ============================================================================

// TODO(holtgrew): Make this follow the header structure.

template<bool TTop, bool TRight, bool TBottom, typename TSpec, typename TElement, typename TCost>
inline void
_initFirstColumn(AlignConfig<TTop, false, TRight, TBottom, TSpec> const,
				 TElement& element,
				 TCost const cost)
{
	SEQAN_CHECKPOINT
	element = cost;
}

//////////////////////////////////////////////////////////////////////////////

template<bool TTop, bool TRight, bool TBottom, typename TSpec, typename TElement, typename TCost>
inline void
_initFirstColumn(AlignConfig<TTop, true, TRight, TBottom, TSpec> const,
				 TElement& element,
				 TCost const)
{
	SEQAN_CHECKPOINT
	element = 0;
}

//////////////////////////////////////////////////////////////////////////////

template<bool TLeft, bool TRight, bool TBottom, typename TSpec, typename TElement, typename TCost>
inline void
_initFirstRow(AlignConfig<false, TLeft, TRight, TBottom, TSpec> const,
			  TElement& element,
			  TCost const cost)
{
	SEQAN_CHECKPOINT
	element = cost;
}

//////////////////////////////////////////////////////////////////////////////

template<bool TLeft, bool TRight, bool TBottom, typename TSpec, typename TElement, typename TCost>
inline void
_initFirstRow(AlignConfig<true, TLeft, TRight, TBottom, TSpec> const,
			  TElement& element,
			  TCost const)
{
	SEQAN_CHECKPOINT
	element = 0;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

template<bool TTop, bool TLeft, bool TRight, typename TSpec, typename TValue1, typename TIndex1, typename TValue2, typename TIndex2>
inline void
_lastRow(AlignConfig<TTop, TLeft, TRight, false, TSpec> const,
		 TValue1&,
		 TIndex1&,
		 TValue2 const,
		 TIndex2 const)
{
	SEQAN_CHECKPOINT
	// Nop
}

//////////////////////////////////////////////////////////////////////////////

template<bool TTop, bool TLeft, bool TRight, typename TSpec, typename TValue1, typename TIndex1, typename TValue2, typename TIndex2>
inline void
_lastRow(AlignConfig<TTop, TLeft, TRight, true, TSpec> const,
		 TValue1& maxValue,
		 TIndex1& maxIndex,
		 TValue2 const val,
		 TIndex2 const index)
{
	SEQAN_CHECKPOINT
	if (val > maxValue[0]) {
		maxValue[0] = val;
		maxIndex[0] = index;
	}
}

//////////////////////////////////////////////////////////////////////////////

template<bool TTop, bool TLeft, bool TBottom, typename TSpec, typename TValue1, typename TIndex1, typename TColumn>
inline void
_lastColumn(AlignConfig<TTop, TLeft, false, TBottom, TSpec> const,
			TValue1& maxValue,
			TIndex1&,
			TColumn const& column)
{
	SEQAN_CHECKPOINT
	maxValue[1] = column[length(column) - 1];
}

//////////////////////////////////////////////////////////////////////////////

template<bool TTop, bool TLeft, bool TBottom, typename TSpec, typename TValue1, typename TIndex1, typename TColumn>
inline void
_lastColumn(AlignConfig<TTop, TLeft, true, TBottom, TSpec> const,
			TValue1& maxValue,
			TIndex1& maxIndex,
			TColumn const& column)
{
	SEQAN_CHECKPOINT;
	typedef typename Size<TColumn>::Type TSize;
	typedef typename Iterator<TColumn, Standard>::Type TColIter;
	TSize limit = length(column) - 1;
	maxValue[1] = column[limit];
	TColIter itCol = begin(column, Standard());
	TColIter itColEnd = end(column, Standard());
	for(TSize i = 0;itCol != itColEnd; ++i, ++itCol) {
		if (*itCol > maxValue[1]) {
			maxValue[1] = *itCol;
			maxIndex[1] = i;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////

template<typename TScoreValue, bool TTop, bool TLeft, typename TSpec, typename TValue, typename TIndex, typename TSize>
inline TScoreValue
_maxOfAlignment(AlignConfig<TTop, TLeft, false, false, TSpec> const,
				TValue& maxValue,
				TIndex&,
				TSize const,
				TSize const)
{
	SEQAN_CHECKPOINT
	return maxValue[1];
}

//////////////////////////////////////////////////////////////////////////////

template<typename TScoreValue, bool TTop, bool TLeft, typename TSpec, typename TValue, typename TIndex, typename TSize>
inline TScoreValue
_maxOfAlignment(AlignConfig<TTop, TLeft, true, false, TSpec> const,
				TValue& maxValue,
				TIndex& maxIndex,
				TSize const len1,
				TSize const)
{
	SEQAN_CHECKPOINT
	maxIndex[0] = len1;
	return maxValue[1];
}

//////////////////////////////////////////////////////////////////////////////

template<typename TScoreValue, bool TTop, bool TLeft, typename TSpec, typename TValue, typename TIndex, typename TSize>
inline TScoreValue
_maxOfAlignment(AlignConfig<TTop, TLeft, false, true, TSpec> const,
				TValue& maxValue,
				TIndex& maxIndex,
				TSize const,
				TSize const len2)
{
	SEQAN_CHECKPOINT
	maxIndex[1] = len2;
	return maxValue[0];
}

//////////////////////////////////////////////////////////////////////////////

template<typename TScoreValue, bool TTop, bool TLeft, typename TSpec, typename TValue, typename TIndex, typename TSize>
inline TScoreValue
_maxOfAlignment(AlignConfig<TTop, TLeft, true, true, TSpec> const,
				TValue& maxValue,
				TIndex& maxIndex,
				TSize const len1,
				TSize const len2)
{
	SEQAN_CHECKPOINT
	// Find the maximum
	if (maxValue[1] > maxValue[0]) maxIndex[0] = len1;
	else maxIndex[1] = len2;
	return (maxValue[0] > maxValue[1]) ? maxValue[0] : maxValue[1];
}

//////////////////////////////////////////////////////////////////////////////

template<bool TTop, bool TLeft, bool TBottom, typename TSpec, typename TValue1, typename TIndex1, typename TValue2, typename TIndex2>
inline void
_lastColumn(AlignConfig<TTop, TLeft, false, TBottom, TSpec> const,
			TValue1& maxValue,
			TIndex1& maxIndex,
			TValue2 const val,	
			TIndex2 const row,
			TIndex2 const col)
{
	SEQAN_CHECKPOINT
	maxValue[1] = val; maxIndex[2] = row; maxIndex[3] = col;
}

//////////////////////////////////////////////////////////////////////////////

template<bool TTop, bool TLeft, bool TBottom, typename TSpec, typename TValue1, typename TIndex1, typename TValue2, typename TIndex2>
inline void
_lastColumn(AlignConfig<TTop, TLeft, true, TBottom, TSpec> const,
			TValue1& maxValue,
			TIndex1& maxIndex,
			TValue2 const val,
			TIndex2 const row,
			TIndex2 const col)
{
	SEQAN_CHECKPOINT
	if (val > maxValue[1]) {maxValue[1] = val; maxIndex[2] = row; maxIndex[3] = col; }
}

//////////////////////////////////////////////////////////////////////////////

template<bool TTop, bool TLeft, bool TRight, typename TSpec, typename TValue1, typename TIndex1, typename TValue2, typename TIndex2>
inline void
_lastRow(AlignConfig<TTop, TLeft, TRight, false, TSpec> const,		
		 TValue1& maxValue,
		 TIndex1& maxIndex,
		 TValue2 const val,
		 TIndex2 const row,
		 TIndex2 const col)
{
	SEQAN_CHECKPOINT
	maxValue[0] = val; maxIndex[0] = row; maxIndex[1] = col;
}

//////////////////////////////////////////////////////////////////////////////

template<bool TTop, bool TLeft, bool TRight, typename TSpec, typename TValue1, typename TIndex1, typename TValue2, typename TIndex2>
inline void
_lastRow(AlignConfig<TTop, TLeft, TRight, true, TSpec> const,
		 TValue1& maxValue,
		 TIndex1& maxIndex,
		 TValue2 const val,
		 TIndex2 const row,
		 TIndex2 const col)
{
	SEQAN_CHECKPOINT
	if (val > maxValue[0]) {maxValue[0] = val; maxIndex[0] = row; maxIndex[1] = col; }
}


//////////////////////////////////////////////////////////////////////////////

template<bool TLeft, bool TRight, bool TBottom, typename TSpec>
inline bool
_configValueTop(AlignConfig<true, TLeft, TRight, TBottom, TSpec> const)
{
	return true;
}

template<bool TLeft, bool TRight, bool TBottom, typename TSpec>
inline bool
_configValueTop(AlignConfig<false, TLeft, TRight, TBottom, TSpec> const)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////////

template<bool TTop, bool TRight, bool TBottom, typename TSpec>
inline bool
_configValueLeft(AlignConfig<TTop, true, TRight, TBottom, TSpec> const)
{
	return true;
}

template<bool TTop, bool TRight, bool TBottom, typename TSpec>
inline bool
_configValueLeft(AlignConfig<TTop, false, TRight, TBottom, TSpec> const)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////////

template<bool TTop, bool TLeft, bool TBottom, typename TSpec>
inline bool
_configValueRight(AlignConfig<TTop, TLeft, true, TBottom, TSpec> const)
{
	return true;
}

template<bool TTop, bool TLeft, bool TBottom, typename TSpec>
inline bool
_configValueRight(AlignConfig<TTop, TLeft, false, TBottom, TSpec> const)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////////

template<bool TTop, bool TLeft, bool TRight, typename TSpec>
inline bool
_configValueBottom(AlignConfig<TTop, TLeft, TRight, true, TSpec> const)
{
	return true;
}

template<bool TTop, bool TLeft, bool TRight, typename TSpec>
inline bool
_configValueBottom(AlignConfig<TTop, TLeft, TRight, false, TSpec> const)
{
	return false;
}

}  // namespace seqan

#endif  // #ifndef SEQAN_CORE_INCLUDE_SEQAN_ALIGN_ALIGN_CONFIG_H_
