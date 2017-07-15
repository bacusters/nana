/*
 *	A Drawer Implementation
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2017 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0. 
 *	(See accompanying file LICENSE_1_0.txt or copy at 
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/detail/drawer.hpp
 */

#ifndef NANA_GUI_DETAIL_DRAWER_HPP
#define NANA_GUI_DETAIL_DRAWER_HPP

#include <nana/push_ignore_diagnostic>
#include <nana/gui/events/general_events.hpp>
#include <nana/paint/graphics.hpp>
#include <functional>

namespace nana
{
	//Forward declarations
	class widget;

	namespace detail
	{
		class drawer;
	}

	/**
	 * \brief Base class for drawing triggers. 
	 * The virtual functions can be reimplemented to respond to drawing triggers.
	 */
	class drawer_trigger
	{
		//Designate nana::drawer as friend
		friend class detail::drawer;

		//Noncopyable
		drawer_trigger(const drawer_trigger&) = delete;
		drawer_trigger& operator=(const drawer_trigger&) = delete;

		//Nonmovable
		drawer_trigger(drawer_trigger&&) = delete;
		drawer_trigger& operator=(drawer_trigger&&) = delete;

	public:
		using widget_reference = widget&;
		using graph_reference = paint::graphics&;

		drawer_trigger() = default;
		virtual ~drawer_trigger() = default;
		virtual void attached(widget_reference, graph_reference);	//none-const
		virtual void detached();	//none-const

		virtual void typeface_changed(graph_reference);
		virtual void refresh(graph_reference);

		virtual void resizing(graph_reference, const arg_resizing&);
		virtual void resized(graph_reference, const arg_resized&);
		virtual void move(graph_reference, const arg_move&);
		virtual void click(graph_reference, const arg_click&);
		virtual void dbl_click(graph_reference, const arg_mouse&);
		virtual void mouse_enter(graph_reference, const arg_mouse&);
		virtual void mouse_move(graph_reference, const arg_mouse&);
		virtual void mouse_leave(graph_reference, const arg_mouse&);
		virtual void mouse_down(graph_reference, const arg_mouse&);
		virtual void mouse_up(graph_reference, const arg_mouse&);
		virtual void mouse_wheel(graph_reference, const arg_wheel&);
		virtual void mouse_dropfiles(graph_reference, const arg_dropfiles&);

		virtual void focus(graph_reference, const arg_focus&);
		virtual void key_press(graph_reference, const arg_keyboard&);
		virtual void key_char(graph_reference, const arg_keyboard&);
		virtual void key_release(graph_reference, const arg_keyboard&);
		virtual void shortkey(graph_reference, const arg_keyboard&);

	private:
		void _m_reset_overrided();
		bool _m_overrided(event_code) const;
	private:
		unsigned overrided_{ 0xFFFFFFFF };
	};

	namespace detail
	{
		//Forward declare basic_window struct
		struct basic_window;

		/**
		 * \brief Class for drawing a window
		 * Contains a reference to a drawer trigger that handles actual drawing for the given events
		 * Is associated with a widget.
		 */
		class drawer
			: nana::noncopyable, nana::nonmovable
		{
			/**
			 * Enum for current method state
			 */
			enum class method_state
			{
				pending,
				overrided,
				not_overrided
			};
		public:
			/**
			 * \brief Initializes the drawer with the underlying implementation.
			 */
			drawer();
			~drawer();

			/**
			 * \brief Binds the drawer to a window
			 * \param window The window to bind to
			 */
			void bind(basic_window* window);

			/**
			 * \brief Relays the typeface_changed event to the associated drawer_trigger, if present
			 */
			void typeface_changed();
			void click(const arg_click&);
			void dbl_click(const arg_mouse&);
			void mouse_enter(const arg_mouse&);
			void mouse_move(const arg_mouse&);
			void mouse_leave(const arg_mouse&);
			void mouse_down(const arg_mouse&);
			void mouse_up(const arg_mouse&);
			void mouse_wheel(const arg_wheel&);
			void mouse_dropfiles(const arg_dropfiles&);
			void resizing(const arg_resizing&);
			void resized(const arg_resized&);
			void move(const arg_move&);
			void focus(const arg_focus&);
			void key_press(const arg_keyboard&);
			void key_char(const arg_keyboard&);
			void key_release(const arg_keyboard&);
			void shortkey(const arg_keyboard&);
			void map(window, bool forced, const rectangle* update_area = nullptr);	//Copy the root buffer to screen
			void refresh();
			/**
			 * \brief Returns a pointer to the associated realizer
			 * \returns The drawer_trigger realizer
			 */
			drawer_trigger* realizer() const;
			/**
			 * \brief Triggers the attached event
			 * Calls drawer_trigger::attached() and drawer_trigger::typeface_changed()
			 * \param widget The attached widget
			 * \param trigger The drawer trigger 
			 */
			void attached(widget& widget, drawer_trigger& trigger);

			/**
			 * \brief Calls the detached event on the drawer_trigger. Returns the old drawer_trigger.
			 * Returns a nullptr when the realizer drawer_trigger was not set.
			 * \returns The old drawer_trigger
			 */
			drawer_trigger* detached();
		public:
			/**
			 * \brief Clears all draw functions, except for those designated 'diehard'.
			 */
			void clear();
			void* draw(std::function<void(paint::graphics&)> &&, bool diehard);
			void erase(void* diehard);
		private:
			void _m_effect_bground_subsequent();
			/**
			 * \brief Returns the method state for the given event position (number, corresponds to an nana::event_code enum value). Can be overridden (1) or not-overridden(0)
			 * \returns The method state.
			 */
			method_state& _m_mth_state(int pos);

			/**
			 * \brief Relays the specified event to the drawer_trigger.
			 * The realizer is checked for existence.
			 * \param evt_code The code of the event to relay
			 * \param arg The associated arguments for the event
			 * \param mfptr Pointer to member function to invoke on the drawer_trigger (i.e. the event handler).
			 */
			template<typename Arg, typename Mfptr>
			void _m_emit(event_code evt_code, const Arg& arg, Mfptr mfptr)
			{
				const int pos = static_cast<int>(evt_code);

				auto realizer = this->realizer();
				auto & mth_state = _m_mth_state(pos);

				if (realizer && (method_state::not_overrided != mth_state))
				{
					if (method_state::pending == mth_state)
					{
						//Apply the event function
						(realizer->*mfptr)(graphics, arg);
						
						//Check realizer, when the window is closed in that event handler, the drawer will be
						//detached and realizer will be a nullptr
						if (realizer)
							mth_state = (realizer->_m_overrided(evt_code) ? method_state::overrided : method_state::not_overrided);
					}
					else
						(realizer->*mfptr)(graphics, arg);

					_m_effect_bground_subsequent();
				}
			}
		public:
			nana::paint::graphics graphics;
		private:
			//Forward declare data_implement struct
			struct data_implement;

			///The data implement 
			data_implement * const data_impl_;
		};
	}//end namespace detail
}//end namespace nana

#include <nana/pop_ignore_diagnostic>

#endif
