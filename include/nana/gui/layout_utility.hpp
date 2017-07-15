/*
 *	Utility Implementation
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2015 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0. 
 *	(See accompanying file LICENSE_1_0.txt or copy at 
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/layout_utility.hpp
 */

#ifndef NANA_GUI_LAYOUT_UTILITY_HPP
#define NANA_GUI_LAYOUT_UTILITY_HPP

#include "basis.hpp"

namespace nana
{
	/**
	 * \brief Tests if two rectangles overlap
	 * \param r1 The first rectangle
	 * \param r2 the second rectangle
	 * \returns Whether the rectangles overlap
	 */
	bool overlapped(const rectangle& r1, const rectangle& r2);

	// overlap, compute the overlap area between r1 and r2. the r is for root
	bool overlap(const rectangle& r1, const rectangle& r2, rectangle& r);

	bool overlap(const rectangle& ir, const size& valid_input_area, const rectangle & dr, const size& valid_dst_area, rectangle& output_src_r, rectangle& output_dst_r);

	bool intersection(const rectangle & r, point pos_beg, point pos_end, point& good_pos_beg, point& good_pos_end);

	/**
	 * \brief Returns the largest rectangle of input that fits in the reference.
	 * Maintains the aspect ratio of the input size, and scales it up to maximally occupy the reference size
	 * \param input_s Input size 
	 * \param ref_s Reference size
	 * \param result_s Output size that maximally fits in the reference size.
	 */
	void fit_zoom(const size& input_s, const size& ref_s, size& result_s);
	/**
	* \brief Same as fit_zoom(const size& input_s, const size& ref_s, size& result_s), only returns a copy of the size
	* \param input_s Input size
	* \param ref_s Reference size
	* \returns Output size that maximally fits in the reference size.
	*/
	size fit_zoom(const size& input_s, size ref_s);

	//zoom
	//@brief:	Calculate the scaled rectangle by refer dst rectangle, that scale factor is same as that between scaled and refer.
	void zoom(const rectangle& refer, const rectangle& scaled, const rectangle& refer_dst, rectangle& r);

	//covered
	//@brief:	Tests a rectangle whether it is wholly covered by another.
	bool covered(const rectangle& underlying, //Rectangle 1 is must under rectangle 2
						const rectangle& cover);
}//end namespace nana
#endif
