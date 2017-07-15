/*
*	Widget Geometrics
*	Nana C++ Library(http://www.nanapro.org)
*	Copyright(C) 2003-2016 Jinhao(cnjinhao@hotmail.com)
*
*	Distributed under the Boost Software License, Version 1.0.
*	(See accompanying file LICENSE_1_0.txt or copy at
*	http://www.boost.org/LICENSE_1_0.txt)
*
*	@file: nana/gui/widget_geometrics.hpp
*	@description:
*/
#ifndef NANA_DETAIL_WIDGET_GEOMETRICS_HPP
#define NANA_DETAIL_WIDGET_GEOMETRICS_HPP

#include <nana/gui/basis.hpp>
#include <memory>
#pragma output
namespace nana
{
	/**
	 * \brief Wrapper class for colors
	 */
	class color_proxy
	{
	public:
		color_proxy(const color_proxy&);
		color_proxy(color_rgb);
		color_proxy(colors);
		color_proxy& operator=(const color_proxy&);
		color_proxy& operator=(const ::nana::color&);
		color_proxy& operator=(color_rgb);
		color_proxy& operator=(colors);
		color get_color() const;
		operator color() const;
	private:
		///Pointer to color
		std::shared_ptr<color> color_;
	};//end namespace color_proxy

	/**
	 * \brief Struct representing colors of a widget
	 */
	struct widget_geometrics
	{
		virtual ~widget_geometrics() = default;
		///The activated state color
		color_proxy activated{ static_cast<color_rgb>(0x60C8FD) };
		///The background color
		color_proxy background{colors::button_face};
		///The foreground color
		color_proxy foreground{colors::black};
	};
}

#endif
