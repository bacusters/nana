/**
 *	A Basic Window Widget Definition
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2017 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0. 
 *	(See accompanying file LICENSE_1_0.txt or copy at 
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file nana/gui/detail/basic_window.hpp
 *  @brief A Basic Window Widget Definition
 */

#ifndef NANA_GUI_DETAIL_BASIC_WINDOW_HPP
#define NANA_GUI_DETAIL_BASIC_WINDOW_HPP
#include <nana/push_ignore_diagnostic>
#include "drawer.hpp"
#include "events_holder.hpp"
#include "widget_geometrics.hpp"
#include "widget_content_measurer_interface.hpp"
#include "widget_notifier_interface.hpp"
#include <nana/datatypes/basic_types.hpp>
#include <nana/system/platform.hpp>
#include <nana/gui/effects.hpp>

namespace nana{
namespace detail
{
	//Forward declaration
	struct basic_window;

	/**
	 * \brief Enum for visibility states of a window
	 */
	enum class visible_state
	{
		invisible, visible, displayed
	};

	class caret
		: public caret_interface
	{
	public:
		caret(basic_window* owner, const size& size);
		~caret();

		void activate(bool activity);
		basic_window* owner() const noexcept;
		void update();
	public:
		//Implement caret_interface functions

		//This function is useless for class caret, see caret_proxy.
		void disable_throw() noexcept override;
		void effective_range(const rectangle& r) override;
		void position(const point& pos) override;
		nana::point position() const override;
		size dimension() const override;
		void dimension(const size& s) override;
		void visible(bool visibility) override;
		bool visible() const override;
	private:
		basic_window * owner_;
		point	position_;
		size	size_;
		size	visual_size_;
		visible_state visibility_{ visible_state::invisible };
		bool	out_of_range_{ false };
		rectangle effect_range_;
	};//end class caret


	/// Define some constant about tab category, these flags can be combine with operator |
	struct tab_type
	{
		enum t
		{
			none,		///< process by nana
			tabstop,	///< move to the next tabstop window
			eating,		///< process by current window
		};
	};


	/**
	 * \brief A window data structure descriptor
	 */ 
	struct basic_window
		: public events_holder
	{
		using container = std::vector<basic_window*>;

		/**
		 * \brief Enum for update state of the window
		 */
		enum class update_state
		{
			none, lazy, refreshed, request_refresh
		};

		struct edge_nimbus_action
		{
			basic_window * window;
			bool rendered;
		};

		/**
		 * \brief Constructs a root window
		 * \param owner Owner window of the root window
		 * \param notifier The widget notifier to link to this window
		 * \param tag Tag for a root window
		 */
		basic_window(basic_window* owner, std::unique_ptr<widget_notifier_interface>&& notifier, category::root_tag** tag);

		template<typename Category>
		basic_window(basic_window* parent, std::unique_ptr<widget_notifier_interface>&& wdg_notifier, const rectangle& r, Category**)
			: widget_notifier(std::move(wdg_notifier)), other(Category::value)
		{
			drawer.bind(this);
			if(parent)
			{
				_m_init_pos_and_size(parent, r);
				_m_initialize(parent);
			}
		}

		~basic_window();

		/**
		 * \brief Binds this window to a native window, represented by the given handle. Sets up all relevant data on the window
		 * \param nativeHandle Handle to the native window
		 * \param width Width for the window
		 * \param height Height for the window
		 * \param extra_width Extra width
		 * \param extra_height Extra height
		 * \param graph Reference to graphics object to associate with the window
		 */
		void bind_native_window(native_window_type nativeHandle, unsigned width, unsigned height, unsigned extra_width, unsigned extra_height, paint::graphics& graph);

#ifndef WIDGET_FRAME_DEPRECATED
		void frame_window(native_window_type);
#endif
		/**
		 * \brief Checks whether this window is the ancestor of the specified window
		 * \param wd The window to check
		 * \reutrns Whether this window is an ancestor of the given window
		 */
		bool is_ancestor_of(const basic_window* wd) const;
		/**
		 * \brief Returns whether all ancestors of this window are visible
		 * \returns Whether all ancestors are visible
		 */
		bool visible_parents() const;
		/**
		 * \brief Returns whether this window should be displayed
		 * \returns Whether to display this window or not
		 */
		bool displayed() const;

		bool belong_to_lazy() const;
		/**
		 * \brief Returns a pointer to the child window that owns the caret
		 * \returns Pointer to child window
		 */
		const basic_window * child_caret() const;

		/**
		 * \brief Determines whether it is a draw-through window.
		 * \returns Whether the window is draw-through
		 */
		bool is_draw_through() const;

		/**
		 * \brief Tries to find the ''oldest'' ancestor which is a lite_widget.
		 * \returns The ancestor that is a lite_widget.
		 */
		basic_window * seek_non_lite_widget_ancestor() const;

		void set_action(mouse_action);
	public:
		/// Override event_holder
		bool set_events(const std::shared_ptr<general_events>&) override;
		general_events * get_events() const override;
	private:
		/**
		 * \brief Sets up position and size for this window
		 * Takes into account the position of the parent, if specified
		 * \param parent Parent of this window
		 * \param rect Rectangle specifying location and dimensions of the window
		 */
		void _m_init_pos_and_size(basic_window* parent, const rectangle& rect);

		void _m_initialize(basic_window* parent);
	public:
#if defined(NANA_LINUX) || defined(NANA_MACOS)
		point	pos_native;
#endif
		///Coordinates of the root window
		point	pos_root;
		///Coordinates of the owner
		point	pos_owner;
		///Dimensions of the window
		size	dimension;
		///Track sizes minimum
		::nana::size	min_track_size;
		///Track sizes maximum
		::nana::size	max_track_size;

		///Whether the window is visible
		bool	visible;

		///Extra width
		unsigned extra_width;
		///Extra height
		unsigned extra_height;

		///Pointer to parent window
		basic_window	*parent;
		///Pointer to owning window
		basic_window	*owner;

		///Title of the window
		native_string_type		title;
		///Drawer for this window
		::nana::detail::drawer	drawer;
		///A pointer to the root basic window. If the window is a root, the pointer refers to itself.
		basic_window*		root_widget;	
		///Refer to the root buffer graphics
		paint::graphics*	root_graph;
		///Cursor of the window
		cursor	predef_cursor;
		///Pointer to widget notifier
		std::unique_ptr<widget_notifier_interface> widget_notifier;

		/**
		 * \brief Window flags
		 */
		struct flags_type
		{
			bool enabled	:1;
			bool dbl_click	:1;
			bool captured	:1;	///< if mouse button is down, it always receive mouse move even the mouse is out of its rectangle
			bool modal		:1;
			bool take_active:1;	///< If take_active is false, other.active_window still keeps the focus.
			bool refreshing	:1;
			bool destroying	:1;
			bool dropable	:1; ///< Whether the window has make mouse_drop event.
			bool fullscreen	:1;	///< When the window is maximizing whether it fit for fullscreen.
			bool borderless :1;
			bool make_bground_declared	: 1;	///< explicitly make bground for bground effects
			bool ignore_menubar_focus	: 1;	///< A flag indicates whether the menubar sets the focus.
			bool ignore_mouse_focus		: 1;	///< A flag indicates whether the widget accepts focus when clicking on it
			bool space_click_enabled : 1;		///< A flag indicates whether enable mouse_down/click/mouse_up when pressing and releasing whitespace key.
			unsigned Reserved	:18;
			unsigned char tab;		///< indicate a window that can receive the keyboard TAB
			mouse_action	action;
			mouse_action	action_before;
		}flags;

		/**
		 * \brief Additional data associated with the window
		 */
		struct annex_components
		{
			///Pointer to caret object
			caret* caret_ptr{ nullptr };

			///Pointer to event handlers
			std::shared_ptr<general_events> events_ptr;
			///Pointer to color scheme
			widget_geometrics* scheme{ nullptr };
			///Pointer to content measurerer
			::nana::dev::widget_content_measurer_interface* content_measurer{ nullptr };
		}annex;

		/**
		 * \brief Edge effect data
		 */
		struct
		{
			///The edge nimbus
			effects::edge_nimbus	edge_nimbus;
			///Background rendering interface
			effects::bground_interface * bground;
			///Fade rate of background
			double	bground_fade_rate;
		}effect;
		
		/**
		 * \brief Tag containing related data of the window
		 */
		struct other_tag
		{
#ifndef WIDGET_FRAME_DEPRECATED
			struct	attr_frame_tag
			{
				native_window_type container{nullptr};
				std::vector<native_window_type> attach;
			};
#endif
			/**
			 * \brief Container for root window data
			 */
			struct	attr_root_tag
			{
#ifndef WIDGET_FRAME_DEPRECATED
				container	frames;	///< initialization is null, it will be created while creating a frame widget. Refer to WindowManager::create_frame
#endif
				///Collection of tabstop elements
				container	tabstop;
				///Effects collection
				std::vector<edge_nimbus_action> effects_edge_nimbus;
				///Currently focussed window
				basic_window*	focus{nullptr};
				///Window representing the menubar
				basic_window*	menubar{nullptr};
				///Whether the root is IME (Input Method Editor) enabled
				bool			ime_enabled{false};
				///Current cursor
				cursor			state_cursor{nana::cursor::arrow};
				///Window associated with state cursor??
				basic_window*	state_cursor_window{ nullptr };

				///Renderer for draw through for root widgets
				std::function<void()> draw_through;
			};

			///Window category type
			const category::flags category;

			/**
			 * \brief The active window
			 * If flags.take_active is false, the active_window still keeps the focus,
			 * If the active_window is null, the parent of this window keeps focus.
			 */
			basic_window *active_window;	
			///If effect.bground is avaiable. Refer to window_layout::make_bground.
			paint::graphics glass_buffer;	
			///Current update state of the window
			update_state	upd_state;

			union
			{
				attr_root_tag * root;
#ifndef WIDGET_FRAME_DEPRECATED
				attr_frame_tag * frame;
#endif
			}attribute;

			other_tag(category::flags);
			~other_tag();
		}other;

		///Window handle of the native root
		native_window_type	root;
		///The identifier of the thread that created the window.
		unsigned			thread_id;
		///Index of the window (within what ??)
		unsigned			index;
		///Container for children
		container			children;
	};

}//end namespace detail
}//end namespace nana
#include <nana/pop_ignore_diagnostic>
#endif

