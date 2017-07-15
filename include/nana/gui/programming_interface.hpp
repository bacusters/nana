/*
 *	Nana GUI Programming Interface Implementation
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2017 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/programming_interface.hpp
 */

#ifndef NANA_GUI_PROGRAMMING_INTERFACE_HPP
#define NANA_GUI_PROGRAMMING_INTERFACE_HPP
#include <nana/config.hpp>
#include <nana/gui/effects.hpp>
#include <nana/gui/events/general_events.hpp>
#include <nana/gui/detail/color_schemes.hpp>
#include <nana/gui/layout/widget_content_measurer_interface.hpp>
#include <nana/paint/image.hpp>
#include <memory>

namespace nana
{
	class drawer_trigger;
	class widget;

	namespace dev
	{
		/// Traits for widget classes
		template<typename Widget>
		struct widget_traits
		{
			using event_type = typename Widget::event_type;
			using scheme_type = typename Widget::scheme_type;
		};

		template<>
		struct widget_traits<widget>
		{
			using event_type = ::nana::general_events;
			using scheme_type = ::nana::widget_geometrics;
		};
	}

namespace API
{
	namespace detail
	{
		::nana::widget_geometrics* make_scheme(::nana::detail::scheme_factory_interface&&);
	}

	void effects_edge_nimbus(window, effects::edge_nimbus);
	effects::edge_nimbus effects_edge_nimbus(window);

	void effects_bground(window, const effects::bground_factory_interface&, double fade_rate);
	void effects_bground(std::initializer_list<window> wdgs, const effects::bground_factory_interface&, double fade_rate);

	bground_mode effects_bground_mode(window);
	void effects_bground_remove(window);

	//namespace dev
	//@brief: The interfaces defined in namespace dev are used for developing the nana.gui
	namespace dev
	{
		/**
		 * \brief Executes the specified function in the thread associated with the window
		 * \param window_handle The window to derive the thread ID from
		 * \param func The function to execute
		 */
		void affinity_execute(window window_handle, const std::function<void()>& func);

		/**
		 * \brief Sets the specified events on the given window
		 * \param window The window
		 * \param events Pointer to the events
		 * \returns False
		 */
		bool set_events(window window, const std::shared_ptr<general_events>& events);

		template<typename Scheme>
		std::unique_ptr<Scheme> make_scheme()
		{
			return std::unique_ptr<Scheme>{static_cast<Scheme*>(API::detail::make_scheme(::nana::detail::scheme_factory<Scheme>()))};
		}

		/**
		 * \brief Sets the scheme on the specified window
		 * \param window The target window
		 * \param scheme The scheme
		 */
		void set_scheme(window window, widget_geometrics* scheme);
		/**
		 * \brief Returns the scheme for the given window
		 * \param wd The window
		 * \retursn Pointer to scheme
		 */
		widget_geometrics* get_scheme(window wd);

		/// Sets a content measurer
		void set_measurer(window, ::nana::dev::widget_content_measurer_interface*);

		/**
		 * \brief Attaches a drawer trigger to the given widget
		 * \param widget Reference to the widget
		 * \param drawerTrigger Reference to the drawer trigger
		 */
		void attach_drawer(widget& widget, drawer_trigger& drawerTrigger);
		::nana::detail::native_string_type window_caption(window) throw();

		/**
		 * \brief Sets the title of the window
		 * \param window The target window
		 * \param title The title
		 */
		void window_caption(window window, ::nana::detail::native_string_type title);

		window create_window(window, bool nested, const rectangle&, const appearance&, widget* attached);
		window create_widget(window, const rectangle&, widget* attached);
		window create_lite_widget(window, const rectangle&, widget* attached);
#ifndef WIDGET_FRAME_DEPRECATED
		window create_frame(window, const rectangle&, widget* attached);
#endif
		/**
		 * \brief Returns the associated graphics object of the specified window
		 * \param window The window
		 * \returns Pointer to graphics object.
		 */
		paint::graphics* window_graphics(window window);

		void delay_restore(bool);

		void register_menu_window(window, bool has_keyboard);

		/**
		 * \brief Attaches or detaches the specified window as menubar
		 * \param wd The window
		 * \param attach Whether to attach or detach.
		 */
		void set_menubar(window wd, bool attach);

		/**
		 * \brief Enables or disables the behaviour that pressing the spacebar will invoke a click.
		 * \param window The target window
		 * \param enable Whether to enable or not
		 */
		void enable_space_click(window window, bool enable);

		bool copy_transparent_background(window, paint::graphics&);
		bool copy_transparent_background(window, const rectangle& src_r, paint::graphics&, const point& dst_pt);

		/*
		 * \brief Copies the frame to the native frame after all events have been processed.
		 */
		void lazy_refresh();
	}//end namespace dev

	/**
	 * \brief Returns the widget pointer of the specified window.
	 * \param window_handle A handle to a window owning the widget.
	 * \returns A widget pointer.
	 */
	widget* get_widget(window window_handle);

	namespace detail
	{
		/**
		 * \brief Returns the base pointer of the events for the specified window.
		 * Returns nullptr when the window is not available.
		 * \param window The window
		 * \returns Pointer to the general_events baseclass, associated with the window
		 */
		general_events* get_general_events(window window);

		/**
		 * \brief Emits the event with the given code on the given window with the given arguments
		 * \param code The event code
		 * \param window The target window
		 * \param args The supplied event arguments
		 */
		bool emit_event(event_code code, window window, const ::nana::event_arg& args);

		/**
		 * Base class for a widgets function class
		 */
		class enum_widgets_function_base
		{
		public:
			virtual ~enum_widgets_function_base() = default;
			/**
			 * \brief Applies the _m_enum_fm function to all widgets of the specified window, possibly recursive.
			 * \param window The window to derive the widgets from
			 * \parma recursive Whether to apply the function recursively.
			 */
			void enum_widgets(window window, bool recursive);
		private:
			/**
			 * \brief Function to apply on widgets
			 * \param widget Pointer to the widget to apply the function on
			 * \returns Whether the function was applied
			 */
			virtual bool _m_enum_fn(::nana::widget* widget) = 0;
		};

		/**
		 * \brief Templated version for a widgets function
		 */
		template<typename Widget, typename EnumFunction>
		class enum_widgets_function
			: public enum_widgets_function_base
		{
		public:
			enum_widgets_function(EnumFunction && enum_fn)
				: enum_fn_(static_cast<EnumFunction&&>(enum_fn))
			{}
		private:
			/**
			 * \brief Applies the given EnumFunction on the widget.
			 * \param wd Pointer to widget
			 * \returns Whether the function was applied
			 */
			bool _m_enum_fn(::nana::widget* wd) override
			{
				return _m_enum_call<Widget>(wd);
			}

			/**
			 * \brief Apply the function if the specified type equals the nana::widget type.
			 * \param wd Pointer to the widget to apply the function to
			 * \returns True, since the function is always applied.
			 */
			template<typename T, typename std::enable_if<std::is_same<::nana::widget, T>::value>::type* = nullptr>
			bool _m_enum_call(::nana::widget* wd)
			{
				enum_fn_(*wd);
				return true;
			}

			/**
			* \brief Apply the function if the specified type is castable to the nana::widget type.
			* \param wd Pointer to the (possible) widget to apply the function to
			* \returns True if the pointer was castable, false otherwise.
			*/
			template<typename T, typename std::enable_if<!std::is_same<::nana::widget, T>::value>::type* = nullptr>
			bool _m_enum_call(::nana::widget* wd)
			{
				auto ptr = dynamic_cast<Widget*>(wd);
				if (nullptr == ptr)
					return false;

				enum_fn_(*ptr);
				return true;
			}
		private:
			///\brief Function to apply on widgets
			EnumFunction && enum_fn_;
		};
	}//end namespace detail

	/**
	 * \brief Closes all windows in current thread
	 */
	void exit();	

	/**
	 * \brief Closes all windows
	 */
	void exit_all();

	/**
	 * \brief Searches whether the text contains a '&' and removes the character for transforming.
	 * Searchs whether the text contains a '&' and removes the character for transforming.
	 * If the text contains more than one '&' charachers, the others are ignored. e.g
	 * text = "&&a&bcd&ef", the result should be "&abcdef", shortkey = 'b', and pos = 2.
	 * \param text Text to transform
	 * \param shortkey The shortkey to find
	 * \param skpos Set to the shortkey position, if present
	 * \returns Transformed string.
	 */
	std::string transform_shortkey_text( std::string text, wchar_t &shortkey, std::string::size_type *skpos);
	bool register_shortkey(window, unsigned long);
	void unregister_shortkey(window);

	/**
	 * \brief Retrieves the current location of the cursor.
	 * \returns The current cursor position
	 */
	nana::point	cursor_position();

	/**
	 * \brief Creates a rectangle of the specified dimensions, centered in the screen.
	 * Places the rectangle at (0,0) if the specified dimensions exceed the sceen size.
	 * \param width The width for the rectangle.
	 * \param height The height for the rectangle
	 * \returns The rectangle
	 */
	rectangle make_center(unsigned width, unsigned height);        
	
	/**
	 * \brief Creates a rectangle of the specified dimensions, centered in the target window
	 * \param window The window, in which the rectangle will be centered.
	 * \param width The width for the rectangle.
	 * \param height The height for the rectangle.
	 * \returns The rectangle.
	 */
	rectangle make_center(window window, unsigned width, unsigned height);

	/**
	* \brief Applies the specified function on all widgets of the given window, possibly recursively.
	* \param wd The window to retrieve the widgets from
	* \param recursive Whether to apply the function recursively
	* \param fn The function to apply.
	*/
	template<typename Widget=::nana::widget, typename EnumFunction>
	void enum_widgets(window wd, bool recursive, EnumFunction && fn)
	{
		static_assert(std::is_convertible<typename std::decay<Widget>::type*, ::nana::widget*>::value, "enum_widgets<Widget>: The specified Widget is not a widget type.");

		detail::enum_widgets_function<Widget, EnumFunction> enum_fn(static_cast<EnumFunction&&>(fn));
		enum_fn.enum_widgets(wd, recursive);
	}
	
	/**
	 * \brief Sets the default window icons
	 * \param small_icon The small icon
	 * \param big_icon The big icon. Defaults to an empty image
	 */
	void window_icon_default(const paint::image& small_icon, const paint::image& big_icon = {});

	/**
	 * \brief Sets the window icons for the specified window
	 * \param window The target window
	 * \param small_icon The small icon
	 * \param big_icon The large icon. Defaults to an empty image.
	 */
	void window_icon(window window, const paint::image& small_icon, const paint::image& big_icon = {});

	/**
	* \brief Checks whether the specified window exists
	* \param window The window
	* \returns Whether the window exists
	*/
	bool empty_window(window window);

	/**
	 * \brief Determines whether the window exists and is not an empty window
	 * \param window The window
	 * \returns Whether the window exists and is not empty
	 */
	bool is_window(window window);

	/**
	 * \brief Returns whether the specified window is currently being destroyed.
	 * \param window The window
	 * \returns Whether the window is being destroyed
	 */
	bool is_destroying(window window);

	/**
	 * \brief Enables or disablse drag/drop operations on the specified window
	 * \param window The target window
	 * \param enable Whether to enable or disable the operation
	 */
	void enable_dropfiles(window window, bool enable);

	/**
	 * \brief Returns whether the specified window has a transparent background
	 * \param window The window
	 * \returns Whether the background is transparent
	 */
	bool is_transparent_background(window window);

	/**
	 * \brief Retrieves the native window representation of the root of a Nana.GUI window.
	 * The native window type is platform-dependent. Under Microsoft Windows, a conversion can be employed between
	 * nana::native_window_type and HWND through reinterpret_cast operator. Under X System, a conversion can
     * be employed between nana::native_window_type and Window through reinterpret_cast operator.
     * \param window The window to transform 
     * \return On success, the native window handle to the Nana.GUI window. On fail, a nullptr.
	 */
	native_window_type root(window window);
	window	root(native_window_type);                     ///< Retrieves the native window of a Nana.GUI window.

	/**
	*	\brief Sets the fullscreen mode of the specified window
	*	\param window The window
	*	\param fullscreen Whether to enable or disable fullscreen
	*/
	void fullscreen(window window, bool fullscreen);

	/**
	 * \brief Unimplemented function
	 */
	bool enabled_double_click(window window, bool enable);

#ifndef WIDGET_FRAME_DEPRECATED
	bool insert_frame(window frame, native_window_type);
	native_window_type frame_container(window frame);
	native_window_type frame_element(window frame, unsigned index);
#endif
	void close_window(window);
	void show_window(window, bool show);                  ///< Sets a window visible state.
	void restore_window(window);
	void zoom_window(window, bool ask_for_max);
	bool visible(window);
	window	get_parent_window(window);
	window	get_owner_window(window);
	bool	set_parent_window(window, window new_parent);

	/**
	 * \brief Returns the events for the given window, while returning the events associated with the specified Widget class.
	 * \param wd The window in question
	 * \returns The event object, which depends on the traits of the specified Widget class.
	 */
	template<typename Widget=::nana::widget>
	typename ::nana::dev::widget_traits<Widget>::event_type & events(window wd)
	{
		using event_type = typename ::nana::dev::widget_traits<Widget>::event_type;

		internal_scope_guard lock;
		auto * general_evt = detail::get_general_events(wd);
		if (nullptr == general_evt)
			throw std::invalid_argument("API::events(): bad parameter window handle, no events object or invalid window handle.");

		if (std::is_same<::nana::general_events, event_type>::value)
			return *static_cast<event_type*>(general_evt);

		auto * widget_evt = dynamic_cast<event_type*>(general_evt);
		if (nullptr == widget_evt)
			throw std::invalid_argument("API::events(): bad template parameter Widget, the widget type and window handle do not match.");
		return *widget_evt;
	}

	template<typename EventArg, typename std::enable_if<std::is_base_of< ::nana::event_arg, EventArg>::value>::type* = nullptr>
	bool emit_event(event_code evt_code, window wd, const EventArg& arg)
	{
		return detail::emit_event(evt_code, wd, arg);
	}

	void umake_event(event_handle);

	template<typename Widget = ::nana::widget>
	typename ::nana::dev::widget_traits<Widget>::scheme_type & scheme(window wd)
	{
		using scheme_type = typename ::nana::dev::widget_traits<Widget>::scheme_type;

		internal_scope_guard lock;
		auto * wdg_colors = dev::get_scheme(wd);
		if (nullptr == wdg_colors)
			throw std::invalid_argument("API::scheme(): bad parameter window handle, no events object or invalid window handle.");

		if (std::is_same<::nana::widget_geometrics, scheme_type>::value)
			return *static_cast<scheme_type*>(wdg_colors);

		auto * comp_wdg_colors = dynamic_cast<scheme_type*>(wdg_colors);
		if (nullptr == comp_wdg_colors)
			throw std::invalid_argument("API::scheme(): bad template parameter Widget, the widget type and window handle do not match.");
		return *comp_wdg_colors;
	}

	point window_position(window);
	void move_window(window, const point&);
	void move_window(window wd, const rectangle&);

	void bring_top(window, bool activated);
	bool set_window_z_order(window wd, window wd_after, z_order_action action_if_no_wd_after);

	void draw_through(window, std::function<void()>);
	void map_through_widgets(window, native_drawable_type);

	/**
	 * \brief Returns the window size of the given window
	 * \param window The window
	 * \returns The window size
	 */
	size window_size(window window);

	/**
	 * \brief Sets the window size to the specified size
	 * \param window The target window
	 * \param size The target size
	 */
	void window_size(window window, const size& size);
	size window_outline_size(window);
	void window_outline_size(window, const size&);
	bool get_window_rectangle(window, rectangle&);
	bool track_window_size(window, const size&, bool true_for_max);   ///< Sets the minimum or maximum tracking size of a window.
	
	void window_enabled(window, bool);
	bool window_enabled(window);

	/*
	 * \brief Refresh the window and display it immediately calling the refresh function of its drawer_trigger.
	 * The drawer::refresh() will be called. If the currently state is lazy_refrsh, the window is delayed to update the graphics until an event is finished.
	 * \param window_handle A handle to the window to be refreshed.
	 */
	void refresh_window(window window_handle);
	void refresh_window_tree(window);      ///< Refreshes the specified window and all its children windows, then display it immediately
	void update_window(window);            ///< Copies the off-screen buffer to the screen for immediate display.

	void window_caption(window, const std::string& title_utf8);
	void window_caption(window, const std::wstring& title);
	::std::string window_caption(window);

	void window_cursor(window, cursor);
	cursor window_cursor(window);

	void activate_window(window);

	/// Determines whether the specified window will get the keyboard focus when its root window gets native system focus.
	bool is_focus_ready(window);

	/// Returns the current keyboard focus window.
	window focus_window();

	/// Sets the keyboard focus for a specified window.
	void focus_window(window);

	/**
	 * \brief Returns the window which captured the mouse input.
	 * \returns The window
	 */
	window	capture_window();

	/**
	 * \brief Enables mouse input capture on the specified window
	 * \param window_handle The window
	 * \param ignore_children Indicates whether to redirect the mouse input to its children if the mouse pointer is over its children.
	 */
	void set_capture(window window_handle, bool ignore_children);

	/**
	 * \brief Make a window stop capturing mouse input
	 * \param window_handle A handle to a window to release grab of mouse input.
	 */
	void release_capture(window window_handle);

	/**
	 * \brief Blocks the execution and other windows' messages until the specified window is closed.
	 * \param window The window
	 */
	void modal_window(window window);

	/**
	 * \brief Blocks the execution until the specified window is closed.
	 * \param window The window
	 */
	void wait_for(window window);

	/**
	 * \brief Retrieves the foreground of the specified window
	 * \param window The window
	 * \returns The foreground color
	 */
	color fgcolor(window window);
	
	/**
	 * \brief Updates the forground color of the specified window
	 * \param window The window in question
	 * \param color The new foreground color
	 * \returns The previous color of the background, if set.
	 */
	color fgcolor(window window, const color& color);
	
	
	color bgcolor(window window);
	color bgcolor(window, const color&);
	color activated_color(window);
	color activated_color(window, const color&);

	void create_caret(window, const size&);
	void destroy_caret(window);

	/**
	 * \brief Opens an existing caret of a window.
	 * This function returns an object to operate caret. The object doesn't create or destroy the caret.
	 * When you are finished with the caret, be sure to reset the pointer.
	 *
	 * @param window_handle A handle to a window whose caret is to be retrieved
	 * @return a pointer to the caret proxy.
	 * @except throws std::runtime if the window doesn't have a caret when disable_throw is false
	 */
	::std::unique_ptr<caret_interface> open_caret(window window_handle, bool disable_throw = false);

	/**
	 * \brief Enables focussing the window on a TAB press.
	 * \param window The target window
	 */
	void tabstop(window window);

	/**
	 * \brief Enables or disables a window to receive a key_char event for pressing TAB key.
	 * \param window_handle A handle to the window to catch TAB key through key_char event.
	 * \param enable Indicates whether to enable or disable catch of TAB key. If this parameter is *true*, the window is
	 * received a key_char when pressing TAB key, and the input focus is not changed. If this parameter is *false*, the
	 * input focus is changed to the next tabstop window.
	 */
	void eat_tabstop(window window_handle, bool enable);

	/// Sets the input focus to the window which the tabstop is near to the specified window.
	/*
	 * @param window_handle A handle to the window.
	 * @param forward Indicates whether forward or backward window to be given the input focus.
	 * @return A handle to the window which to be given the input focus.
	 */
	window move_tabstop(window window_handle, bool forward);

	/// Sets the window active state. If a window active state is false, the window will not obtain the focus when a mouse clicks on it wich will be obteined by take_if_has_active_false.
	void take_active(window, bool has_active, window take_if_has_active_false);

	/**
	 * \brief Sets a copy of the graphics object of a window on the reference
	 * \param window The window from which to retrieve the graphics object
	 * \param graphicsRef The reference to set the graphics object on
	 * \returns Whether the graphics object was succesfully retrieved
	 */
	bool window_graphics(window window, nana::paint::graphics& graphicsRef);
	/**
	* \brief Sets a copy of the graphics object of the root window of the specified window on the reference
	* \param window The window from which to retrieve the root
	* \param graphicsRef The reference to set the graphics object on
	* \returns Whether the graphics object was succesfully retrieved
	*/
	bool root_graphics(window window, nana::paint::graphics& graphicsRef);

	bool get_visual_rectangle(window, nana::rectangle&);

	void typeface(window, const nana::paint::font&);
	paint::font typeface(window);

	/**
	 * \brief Converts window coordinates to screen coordinates
	 * \param window The window
	 * \param point The target point struct to set the coordinates on
	 * \returns Whether the point was succesfully calculated
	 */
	bool calc_screen_point(window window, point& point);
	bool calc_window_point(window, point&);   ///<Converts screen coordinates to window coordinates.

	/**
	 * \brief Finds a nana.GUI window by position on the screen
	 * \param mspos Position on the screen
	 * \returns A pointer to a window, or a nullptr if no window was found.
	 */
	window find_window(const nana::point& mspos);

	bool is_window_zoomed(window, bool ask_for_max);  ///<Tests a window whether it is maximized or minimized.

	void widget_borderless(window, bool);	///<Enables or disables a borderless widget.
	bool widget_borderless(window);			///<Tests a widget whether it is borderless.

	nana::mouse_action mouse_action(window);
	nana::element_state element_state(window);

	bool ignore_mouse_focus(window, bool ignore);	///< Enables/disables the mouse focus, it returns the previous state
	bool ignore_mouse_focus(window);				///< Determines whether the mouse focus is enabled

	void at_safe_place(window, std::function<void()>);

	/**
	 * \brief Returns a widget content extent size
	 * \param wd A handle to a window that returns its content extent size.
	 * \param limited_px Specifies the max pixels of width or height. If this parameter is zero, this parameter will be ignored.
	 * \param limit_width Indicates whether the it limits the width or height. If this parameter is *true*, the width is limited.
	 * If the parameter is *false*, the height is limited. This parameter is ignored if limited_px = 0.
	 * \returns if optional has a value, the first size indicates the content extent, the second size indicates the size of
	 * widget by the content extent. 
	 */
	optional<std::pair<::nana::size, ::nana::size>> content_extent(window wd, unsigned limited_px, bool limit_width);
}//end namespace API

}//end namespace nana

#endif

