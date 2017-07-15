/**
 *	The fundamental widget class implementation
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2017 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0. 
 *	(See accompanying file LICENSE_1_0.txt or copy at 
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/widgets/widget.hpp
 */

#ifndef NANA_GUI_WIDGET_HPP
#define NANA_GUI_WIDGET_HPP

#include <nana/push_ignore_diagnostic>
#include <nana/gui/programming_interface.hpp>
#include <nana/text/internationalization.hpp>
#include <nana/gui/detail/drawer.hpp>

namespace nana
{
	namespace detail
	{
		//Forward declaration of widget_notifier_interface
		class widget_notifier_interface;
	}

	/**
	 * Abstract class for widgets. Defines the capacity interface.
	 */
	class widget
	{
		//Notifier
		friend class detail::widget_notifier_interface;
		///Implementation of widget notifier
		class inner_widget_notifier;

		typedef void(*dummy_bool_type)(widget* (*)(const widget&));

		
		//Noncopyable
		widget(const widget&) = delete;
		widget& operator=(const widget&) = delete;

		//Nonmovable
		widget(widget&&) = delete;
		widget& operator=(widget&&) = delete;
		
	public:
		using native_string_type = detail::native_string_type;

		widget() = default;

		virtual ~widget() = default;
		/**
		 * \brief Returns the handle of window, returns 0 if window is not created.
		 * \returns The window handle
		 */
		virtual window handle() const = 0;
		/**
		 * 	Determines whether the manipulator is handling a window.
		 */
		bool empty() const;					
		void close();

		window parent() const;

		::std::string caption() const throw();
		::std::wstring caption_wstring() const throw();
		native_string_type caption_native() const throw();

		widget& caption(std::string utf8);
		widget& caption(std::wstring);

		template<typename ...Args>
		void i18n(std::string msgid, Args&&... args)
		{
			_m_caption(::nana::to_nstring(::nana::internationalization().get(msgid, std::forward<Args>(args)...)));
		}

		void i18n(i18n_eval);

		void cursor(nana::cursor);
		nana::cursor cursor() const;		///< Retrieves the shape of cursor

		void typeface(const paint::font& font);
		paint::font typeface() const;

		bool enabled() const;				///< Determines whether the window is enabled for mouse and keyboard input.
		void enabled(bool);

		void enable_dropfiles(bool);		///< Enables/Disables a window to accept dropped files.

		void focus();
		bool focused() const;

		/**
		 * \brief Show the window.
		 */
		void show();
		/**
		 * \brief Hide the window
		 */
		void hide();
		/**
		 * \brief Returns whether the window is visible.
		 * \returns Whether the window is visible.
		 */
		bool visible() const;

		nana::size size() const;
		void size(const nana::size&);

		/*
		 * Enables the widget to grab the mouse input.
		 * \param ignore_children Indicates whether to redirect the mouse input to its children if the mouse pointer is over its children.
		 */
		void set_capture(bool ignore_children);

		/**
		 * \brief Stops capturing mouse input for this widget.
		 */
		void release_capture();
		
		point pos() const;
		void move(int x, int y);
		void move(const point&);
		void move(const rectangle&);

		void fgcolor(const nana::color&);
		nana::color fgcolor() const;
		void bgcolor(const nana::color&);
		nana::color bgcolor() const;

		general_events& events() const;

		void umake_event(event_handle eh) const;              ///< Deletes an event callback by a handle.

		widget& register_shortkey(wchar_t);	///< Registers a shortkey. To remove a registered key, pass 0.

		widget& take_active(bool activated, window take_if_not_activated);
		widget& tooltip(const ::std::string&);

		operator dummy_bool_type() const;
		operator window() const;
	protected:
		/**
		 * \brief Constructs a pointer to a new inner notifier implementation
		 * \returns Pointer to the notifier implementation.
		 */
		std::unique_ptr<::nana::detail::widget_notifier_interface> _m_wdg_notifier();
	private:
		virtual void _m_notify_destroy() = 0;

	protected:
		//protected members, a derived class must call this implementation if it overrides an implementation
		virtual void _m_complete_creation();

		virtual general_events& _m_get_general_events() const = 0;
		virtual native_string_type _m_caption() const throw();
		virtual void _m_caption(native_string_type&&);
		virtual nana::cursor _m_cursor() const;
		virtual void _m_cursor(nana::cursor);
		virtual void _m_close();
		virtual bool _m_enabled() const;
		virtual void _m_enabled(bool);
		virtual bool _m_show(bool);
		virtual bool _m_visible() const;
		virtual void _m_size(const nana::size&);
		virtual void _m_move(int x, int y);
		virtual void _m_move(const rectangle&);

		virtual void _m_typeface(const nana::paint::font& font);
		virtual nana::paint::font _m_typeface() const;

		virtual void _m_fgcolor(const nana::color&);
		virtual nana::color _m_fgcolor() const;
		virtual void _m_bgcolor(const nana::color&);
		virtual nana::color _m_bgcolor() const;
	};

	namespace detail
	{
		class widget_base
			: public widget
		{
		public:
			window handle() const override;
		protected:
			void _m_notify_destroy() override;
		protected:
			window handle_{ nullptr };
		};
	}

            /// Base class of all the classes defined as a widget window. Defaultly a widget_tag
	template<typename Category, typename DrawerTrigger, typename Events = ::nana::general_events, typename Scheme = ::nana::widget_geometrics>
	class widget_object: public detail::widget_base
	{
	protected:
		typedef DrawerTrigger drawer_trigger_t;
	public:
		using scheme_type	= Scheme;
		using event_type	= Events;

		widget_object()
			:	events_{ std::make_shared<Events>() },
				scheme_{ API::dev::make_scheme<Scheme>() }
		{}

		~widget_object()
		{
			API::close_window(handle());
		}

		/**
		 * \brief Returns the associated events of the widget
		 * \returns The events object
		 */
		event_type& events() const
		{
			return *events_;
		}

		/**
		 * Creates a zero-size widget in a widget/root window specified by parent_wd.
		 * \param parent_wd The parent window
		 * \param visible Whether to make the widget visible
		 * \returns Whether the creation was succesful
		 */
		bool create(window parent_wd, bool visible)
		{
			return create(parent_wd, rectangle(), visible);
		}

		/**
		 * \brief  Creates a widget in a widget/root window of the specificied dimensions and visibility.
		 * \param parent_wd The window to creates this widget in
		 * \param r The location to occupy
		 * \param visible Whether the widget should be visible
		 * \returns Whether creation of the widget was succesful.
		 */
		bool create(window parent_wd, const rectangle & r = {}, bool visible = true)
		{
			if(parent_wd && this->empty())
			{
				handle_ = API::dev::create_widget(parent_wd, r, this);
				API::dev::set_events(handle_, events_);
				API::dev::set_scheme(handle_, scheme_.get());
				API::dev::attach_drawer(*this, trigger_);
				if(visible)
					API::show_window(handle_, true);
				
				this->_m_complete_creation();
			}
			return (this->empty() == false);
		}

		/**
		 * \brief Makes the widget borderless or not
		 * \param enable Whether to enable or disable this
		 * \returns Modified widget_object reference
		 */
		widget_object& borderless(bool enable)
		{
			API::widget_borderless(handle_, enable);
			return *this;
		}

		/**
		 * \brief Returns whether the window is borderless.
		 * \returns Whether the window is borderless.
		 */
		bool borderless() const
		{
			return API::widget_borderless(handle_);
		}

		/**
		 * \brief Returns the scheme of the widget
		 * \returns Reference to the scheme.
		 */
		scheme_type& scheme() const
		{
			return *scheme_;
		}
	protected:
		/**
		 * \brief Returns the drawer trigger
		 * \returns Reference to the drawer trigger
		 */
		DrawerTrigger& get_drawer_trigger()
		{
			return trigger_;
		}
		/**
		* \brief Returns the drawer trigger as const
		* \returns const reference to the drawer trigger
		*/
		const DrawerTrigger& get_drawer_trigger() const
		{
			return trigger_;
		}
	private:
		/**
		 * \returns A reference to the general events
		 */
		general_events& _m_get_general_events() const override
		{
			return *events_;
		}

		/**
		 * \brief Notifies the widget to destroy itself.
		 * Resets all events.
		 */
		void _m_notify_destroy() override final
		{
			widget_base::_m_notify_destroy();
			events_ = std::make_shared<Events>();
		}
	private:
		///The drawer trigger
		DrawerTrigger trigger_;
		///Pointer to events object
		std::shared_ptr<Events> events_;
		///Pointer to scheme
		std::unique_ptr<scheme_type> scheme_;
	};//end class widget_object

	/**
	 * Base class for all widgets with a non-graphics-buffer widget window. No drawer will be created for the widget.
	 * The DrawerTrigger template parameter is always ignored.\see nana::panel
	 */
	template<typename DrawerTrigger, typename Events, typename Scheme>
	class widget_object<category::lite_widget_tag, DrawerTrigger, Events, Scheme>: public detail::widget_base
	{
	protected:
		typedef DrawerTrigger drawer_trigger_t;
	public:
		using scheme_type = Scheme;
		using event_type = Events;

		widget_object()
			: events_{ std::make_shared<Events>() }, scheme_{ API::dev::make_scheme<scheme_type>() }
		{}

		~widget_object()
		{
			API::close_window(handle());
		}

		event_type& events() const
		{
			return *events_;
		}

		/**
		 * Creates a no-size (zero-size) widget in a widget/root window specified by parent_wd.
		 * \param parent_wd The parent window/widget
		 * \param visible Whether the widget should be visible
		 * \returns Whether the construction was succesful 
		*/
		bool create(window parent_wd, bool visible)    ///< 
		{
			return create(parent_wd, rectangle(), visible);
		}

		/**
		* Creates a widget of specified size in a widget/root window specified by parent_wd.
		* \param parent_wd The parent window/widget
		* \param r Size of the widget
		* \param visible Whether the widget should be visible
		* \returns Whether the construction was succesful
		*/
		bool create(window parent_wd, const rectangle& r = rectangle(), bool visible = true)  
		{
			if(parent_wd && this->empty())
			{
				handle_ = API::dev::create_lite_widget(parent_wd, r, this);
				API::dev::set_events(handle_, events_);
				API::dev::set_scheme(handle_, scheme_.get());
				if(visible)
					API::show_window(handle_, true);
				this->_m_complete_creation();
			}
			return (this->empty() == false);
		}
		
		scheme_type& scheme() const
		{
			return *scheme_;
		}
	private:
		general_events& _m_get_general_events() const override
		{
			return *events_;
		}

		void _m_notify_destroy() override final
		{
			widget_base::_m_notify_destroy();
			events_ = std::make_shared<Events>();
		}
	private:
		std::shared_ptr<Events> events_;
		std::unique_ptr<scheme_type> scheme_;
	};//end class widget_object


	/**
	 * Base class for all root window widgets. \see nana::form
	 */
	template<typename DrawerTrigger, typename Events, typename Scheme>
	class widget_object<category::root_tag, DrawerTrigger, Events, Scheme>: public detail::widget_base
	{
	protected:
		typedef DrawerTrigger drawer_trigger_t;
	public:
		using scheme_type = Scheme;
		using event_type = Events;

		widget_object()
			: widget_object(nullptr, false, API::make_center(300, 150), appearance(), this)
		{
		}

		widget_object(window owner, bool nested, const rectangle& r = {}, const appearance& apr = {})
		{
			handle_ = API::dev::create_window(owner, nested, r, apr, this);
			_m_bind_and_attach();
		}

		~widget_object()
		{
			API::close_window(handle());
		}

		event_type& events() const
		{
			return *events_;
		}

		/**
		 * \brief Activates the window
		 */
		void activate()
		{
			API::activate_window(handle_);
		}

		/**
		 * \brief Returns a handle to the native window representing the widget.
		 * \returns The native window handle
		 */
		native_window_type native_handle() const
		{
			return API::root(handle_);
		}

		void bring_top(bool activated)
		{
			API::bring_top(handle(), activated);
		}

		window owner() const
		{
			return API::get_owner_window(handle_);
		}

		void icon(const nana::paint::image& ico)
		{
			API::window_icon(handle_, ico);
		}

		void restore()
		{
			API::restore_window(handle_);
		}

		void zoom(bool ask_for_max)
		{
			API::zoom_window(handle_, ask_for_max);
		}

		bool is_zoomed(bool check_maximized) const
		{
			return API::is_window_zoomed(handle_, check_maximized);
		}

		widget_object& z_order(window wd_after, z_order_action action_if_no_wd_after)
		{
			API::set_window_z_order(handle_, wd_after, action_if_no_wd_after);
			return *this;
		}

		scheme_type& scheme() const
		{
			return *scheme_;
		}

		void draw_through(std::function<void()> draw_fn)
		{
			API::draw_through(handle(), draw_fn);
		}

		void map_through_widgets(native_drawable_type drawable)
		{
			API::map_through_widgets(handle(), drawable);
		}

		void outline_size(const ::nana::size& sz)
		{
			API::window_outline_size(handle(), sz);
		}

		::nana::size outline_size() const
		{
			return API::window_outline_size(handle());
		}
	protected:
		DrawerTrigger& get_drawer_trigger()
		{
			return trigger_;
		}

		const DrawerTrigger& get_drawer_trigger() const
		{
			return trigger_;
		}
	private:
		void _m_bind_and_attach()
		{
			events_ = std::make_shared<Events>();
			API::dev::set_events(handle_, events_);

			scheme_ = API::dev::make_scheme<scheme_type>();
			API::dev::set_scheme(handle_, scheme_.get());
			API::dev::attach_drawer(*this, trigger_);
		}

		general_events& _m_get_general_events() const override
		{
			return *events_;
		}

		void _m_notify_destroy() override final
		{
			widget_base::_m_notify_destroy();
			events_ = std::make_shared<Events>();
		}
	private:
		DrawerTrigger					trigger_;
		std::shared_ptr<Events>			events_;
		std::unique_ptr<scheme_type>	scheme_;
	};//end class widget_object<root_tag>

#ifndef WIDGET_FRAME_DEPRECATED
	           /// Base class of all the classes defined as a frame window. \see nana::frame
	template<typename Drawer, typename Events, typename Scheme>
	class widget_object<category::frame_tag, Drawer, Events, Scheme>: public widget{};

	           /// Especialization. Base class of all the classes defined as a frame window. \see nana::frame
	template<typename Events, typename Scheme>
	class widget_object<category::frame_tag, int, Events, Scheme>: public detail::widget_base
	{
	protected:
		typedef int drawer_trigger_t;
	public:
		using scheme_type = Scheme;
		using event_type = Events;

		widget_object()
			: events_{ std::make_shared<Events>() }, scheme_{ API::dev::make_scheme<scheme_type>() }
		{}

		~widget_object()
		{
			API::close_window(handle());
		}

		event_type& events() const
		{
			return *events_;
		}

		bool create(window parent_wd, bool visible)    ///< Creates a no-size (zero-size) widget. in a widget/root window specified by parent_wd.
		{
			return create(parent_wd, rectangle(), visible);
		}
                 /// Creates in a widget/root window specified by parent_wd.
		bool create(window parent_wd, const rectangle& r = rectangle(), bool visible = true)
		{
			if(parent_wd && this->empty())
			{
				handle_ = API::dev::create_frame(parent_wd, r, this);
				API::dev::set_events(handle_, events_);
				API::dev::set_scheme(handle_, scheme_.get());
				API::show_window(handle_, visible);
				this->_m_complete_creation();
			}
			return (this->empty() == false);
		}

		scheme_type& scheme() const
		{
			return *scheme_;
		}
	private:
		virtual drawer_trigger* get_drawer_trigger()
		{
			return nullptr;
		}

		general_events& _m_get_general_events() const override
		{
			return *events_;
		}
	private:
		std::shared_ptr<Events> events_;
		std::unique_ptr<scheme_type> scheme_;
	};//end class widget_object<category::frame_tag>
#endif
}//end namespace nana

#include <nana/pop_ignore_diagnostic>
#endif
