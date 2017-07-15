/*
 *	Platform Implementation
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2016 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0. 
 *	(See accompanying file LICENSE_1_0.txt or copy at 
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/detail/native_window_interface.hpp
 */

#ifndef NANA_GUI_DETAIL_NATIVE_WINDOW_INTERFACE_HPP
#define NANA_GUI_DETAIL_NATIVE_WINDOW_INTERFACE_HPP

#include "../basis.hpp"
#include <nana/paint/image.hpp>

namespace nana
{
namespace detail
{
	/**
	 * \brief Class containing utility functions for native windows
	 */
	struct native_interface
	{
		/**
		 * \brief Result struct for creation of a window
		 */
		struct window_result
		{
			///The native handle to the window
			native_window_type native_handle;

			///Client width
			unsigned width;
			///Client height
			unsigned height;

			///Extra border width. Useful in Windows, ignored in X11 (i.e. set to 0).
			unsigned extra_width;
			///Extra border height. Useful in Windows, ignored in X11 (i.e. set to 0).
			unsigned extra_height;
		};

		using native_string_type = ::nana::detail::native_string_type;

		/**
		 * \brief Execute a function in a thread which is associated with the specified native window.
		 */
		static void affinity_execute(native_window_type, const std::function<void()>&);

		/**
		 * \brief Retrieves the dimensions of the primary monitor
		 * \returns Dimensions of the primary monitor
		 */
		static nana::size	primary_monitor_size();

		/**
		 * \brief Returns the area of the monitor that the given point is in.
		 * \param pnt The point to query the monitor with
		 * \returns Rectangle representing the monitor area (locaiton and dimensions)
		 */
		static rectangle screen_area_from_point(const point& pnt);

		/**
		 * \brief Creates a native window at the given location with the given appearance
		 * \param owner The owner window, if applicable
		 * \param nested  Whether the window is nested within the owner window
		 * \param area The target area to display the window
		 * \param appearance The appearance of the window
		 * \returns Window creation result struct.
		 */
		static window_result create_window(native_window_type owner, bool nested, const rectangle& area, const appearance& appearance);

		/**
		 * \brief Creates a child window for the given parent, in the designated area
		 * \param parent Native handle to parent window
		 * \param area Area of the child window
		 * \return Native window handle to the child window
		 */
		static native_window_type create_child_window(native_window_type parent, const rectangle& area);

#if defined(NANA_X11)
		static void set_modal(native_window_type);
#endif
		/**
		 * \brief Enables or disables the possibility to drop files on the given native window
		 * \param window The window
		 * \param enable Whether to enable or disable
		 */
		static void enable_dropfiles(native_window_type window, bool enable);
		
		/**
		 * \brief Enables or disables the window
		 * \param window The target window
		 * \param enable Whether to enable or disable
		 */
		static void enable_window(native_window_type window, bool enable);
		// (On Windows) The system displays the large icon in the ALT+TAB dialog box, and the small icon in the window caption.
		static bool window_icon(native_window_type, const paint::image& big_icon, const paint::image& small_icon);
		static void activate_owner(native_window_type);
		static void activate_window(native_window_type);
		static void close_window(native_window_type);
		static void show_window(native_window_type, bool show, bool active);
		static void restore_window(native_window_type);
		static void zoom_window(native_window_type, bool ask_for_max);
		static void	refresh_window(native_window_type);
		static bool is_window(native_window_type);
		static bool	is_window_visible(native_window_type);
		static bool is_window_zoomed(native_window_type, bool ask_for_max);

		static nana::point	window_position(native_window_type);
		/**
		 * \brief Moves the given window to the designated location
		 * \param window The window
		 * \param x New x coordinate
		 * \param y New y coordinate
		 */
		static void	move_window(native_window_type window, int x, int y);

		/**
		 * \brief Moves and/or resizes the target window
		 * \param window The window
		 * \param area New area to be occupied by the window
		 */
		static void	move_window(native_window_type window, const rectangle& area);

		/**
		 * \brief Moves the given window to the top of possibly overlapping windows
		 * \param window The window
		 * \param activated Whether to activate the window when bringing to front
		 */
		static void bring_top(native_window_type window, bool activated);

		/**
		 * \brief Sets the Z-order of the given window
		 * \param window The target window
		 * \param wd_after Window to place this window after
		 * \param action_if_no_wd_after Action to apply if no after is specified
		 */
		static void	set_window_z_order(native_window_type window, native_window_type wd_after, z_order_action action_if_no_wd_after);

		static void	window_size(native_window_type, const size&);
		static void	get_window_rect(native_window_type, rectangle&);
		static void	window_caption(native_window_type, const native_string_type&);
		static native_string_type	window_caption(native_window_type);
		static void	capture_window(native_window_type, bool);
		static nana::point	cursor_position();
		static native_window_type get_owner_window(native_window_type);
		static native_window_type parent_window(native_window_type);
		static native_window_type parent_window(native_window_type child, native_window_type new_parent, bool returns_previous);
		//For Caret
		static void caret_create(native_window_type, const ::nana::size&);
		static void caret_destroy(native_window_type);
		static void caret_pos(native_window_type, const ::nana::point&);
		static void caret_visible(native_window_type, bool);

		static void	set_focus(native_window_type);
		static native_window_type get_focus_window();
		static bool calc_screen_point(native_window_type, nana::point&);
		static bool calc_window_point(native_window_type, nana::point&);

		static native_window_type find_window(int x, int y);
		static nana::size check_track_size(nana::size sz, unsigned extra_width, unsigned extra_height, bool true_for_max);
	};


}//end namespace detail
}//end namespace nana
#endif
