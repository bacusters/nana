/*
 *	An Implementation of Place for Layout
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2014 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/place.cpp
 *
 *	@contributions:
 *	min/max and splitter bar initial weight by Ariel Vina-Rodriguez.
 */

#ifndef NANA_GUI_PLACE_HPP
#define NANA_GUI_PLACE_HPP
#include <nana/push_ignore_diagnostic>
#include <nana/gui/basis.hpp>
#include <memory>
#include <functional>

namespace nana
{
	namespace paint
	{
		//Forward declare
		class graphics;
	}
	//Forward declare
	class widget;

	namespace detail
	{

		class place_agent
		{
		public:
			virtual ~place_agent() = default;
			virtual std::unique_ptr<nana::widget> create(nana::window) const = 0;
		};
	}

	template<typename Widget>
	class agent
		: public detail::place_agent
	{
	public:
		agent(std::function<void(Widget&)> initializer)
			: init_(std::move(initializer))
		{}

		agent(const char* text)
			: text_(text)
		{
			throw_not_utf8(text);
		}

		agent(std::string text, std::function<void(Widget&)> initializer = {})
			: text_(std::move(text)), init_(std::move(initializer))
		{
			throw_not_utf8(text_);
		}

	private:
		std::unique_ptr<nana::widget> create(nana::window handle) const override
		{
			std::unique_ptr<Widget> ptr(new Widget(handle));
			ptr->caption(text_);
			if (init_)
				init_(*ptr);
			return std::move(ptr);
		}
	private:
		std::string text_;
		std::function<void(Widget&)> init_;
	};

    ///  Layout managment - an object of class place is attached to a widget, and it automatically positions and resizes the children widgets.
	class place
		: ::nana::noncopyable
	{
		//Forward declare implementation
		struct implement;

		/**
		 * \interface field_interface
		 * \brief Defines interactions with fields in the layout.
		 */
		class field_interface
		{
			field_interface(const field_interface&) = delete;
			field_interface& operator=(const field_interface&) = delete;
			field_interface(field_interface&&) = delete;
			field_interface& operator=(field_interface&&) = delete;
		public:
			field_interface() = default;
			virtual ~field_interface() = default;
			virtual field_interface& operator<<(const char* label) = 0;
			virtual field_interface& operator<<(std::string label) = 0;
			virtual field_interface& operator<<(window) = 0;
			virtual field_interface& fasten(window) = 0;
			
			template<typename Widget>
			field_interface& operator<<(const agent<Widget>& ag)
			{
				_m_add_agent(ag);
				return *this;
			}
		private:
			virtual void _m_add_agent(const detail::place_agent&) = 0;
		};
	public:
        ///  reference to a field manipulator which refers to a field object created by place 
		using field_reference = field_interface &;

		/**
		 * \brief Sets up the unbound place object
		 */
		place();
		/**
		 * \brief Sets up a place object for the specified window
		 * \param wd The window to attach to
		 */
		place(window wd);

		/**
		 * Cleans up the placer
		 */
		~place();

		/** @brief Bind to a window
		 *	@param handle	A handle to a window which the place wants to attach.
		 *	@remark	It will throw an exception if the place has already binded to a window.
		 */
		void bind(window handle);
		/**
		 * \brief Returns the associated window handle
		 * \returns The window handle
		 */
		window window_handle() const;

		/**
		 * \brief Sets a custom renderer for the splitters in the layout.
		 * \param fn The custom render function.
		 */
		void splitter_renderer(std::function<void(window, paint::graphics&, mouse_action)> fn);
        
		/**
		 * \brief Sets up the layout for the associated window of this object
		 * \param s The string describing the layout of the widget
		 */
		void div(const char* s);
		/**
		 * \brief Returns div-text that depends on fields status.
		 * \returns The layout text
		 */
		const std::string& div() const noexcept;
		/**
		 * \brief Modifies the specified field in the specified layout
		 * \param field_name The name of the field to modify
		 * \param div_text New layout description for the specified field
		 */
		void modify(const char* field_name, const char* div_text);

		/**
		 * \brief Returns a field reference for the field with the specified name within the specified layout.
		 * \param name Name of the field to return
		 * \returns A field_interface& object
		 */
		field_reference field(const char* name);

		/**
		 * \brief Sets the visibility of a field within the layout
		 * \param field_name Name of the field
		 * \param visible Whether to make the field visible or not
		 */
		void field_visible(const char* field_name, bool visible);

		/**
		 * \brief Returns whether the specified field is visible
		 * \param field_name The field
		 * \returns Whether the field is visible or not
		 */
		bool field_visible(const char* field_name) const;

		void field_display(const char* field_name, bool display); ///<Displays/Discards an existing field.
		bool field_display(const char* field_name) const;	///<Determines whether the specified field is displayed.

		/**
		 * \brief Applies the layout with the given child widgets to the associated window
		 * \remark Call is relayed to the implementation struct
		 */
		void collocate();

 		void erase(window handle);				///< Erases a window from field.
		
		/**
		 * \brief Syntactic sugar for the place::field(const char* name) methods
		 * \param name The name of the field to return.
		 * \returns The specified field, as field_interface&.
		 */
		field_reference operator[](const char* name);

		/// Add a panel factory
		template<typename Panel, typename ...Args>
		place& dock(const std::string& dockname, const std::string& factory_name, Args&& ... args)
		{
			return dock(dockname, factory_name, std::bind([](window parent, Args & ... args)
			{
				return std::unique_ptr<widget>(new Panel(parent, std::forward<Args>(args)...));
			}, std::placeholders::_1, args...));
		}
		/**
		 * \brief 
		 */
		place& dock(const std::string& dockname, std::string factory_name, std::function<std::unique_ptr<widget>(window)> factory);
		widget* dock_create(const std::string& factory);
	private:
		implement * impl_;
	};
}//end namespace nana
#include <nana/pop_ignore_diagnostic>

#endif //#ifndef NANA_GUI_PLACE_HPP
