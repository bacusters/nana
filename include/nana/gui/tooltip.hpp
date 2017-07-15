/*
 *	A Tooltip Implementation
 *	Copyright(C) 2003-2016 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/widgets/tooltip.hpp
 */

#ifndef NANA_GUI_WIDGETS_TOOLTIP_HPP
#define NANA_GUI_WIDGETS_TOOLTIP_HPP
#include <nana/gui/widgets/widget.hpp>

namespace nana
{
	/**
	 * \interface tooltip_interface
	 * \brief An interface for user-defined tooltip windows
	 */
	class tooltip_interface
	{
	public:
		virtual ~tooltip_interface(){}
		/**
		 * \brief Checks whether the tooltip is empty
		 * \returns Whether the tooltip is empty or not
		 */
		virtual bool tooltip_empty() const = 0;
		/**
		 * \brief Retrieves the tooltip size
		 * \return Size of the tooltip
		 */
		virtual nana::size tooltip_size() const = 0;
		/**
		 * \brief Sets the tooltip text
		 * \param text The tooltip text
		 */
		virtual void tooltip_text(const ::std::string& text)	= 0;
		/**
		 * \brief Moves the tooltip to the designeated screen position
		 * \param screen_pos
		 * \param ignore_pos 
		 */
		virtual void tooltip_move(const nana::point& screen_pos, bool ignore_pos)	= 0;
		/**
		 * \brief Sets how long the tooltip is shown
		 * \param duration The duration
		 */
		virtual void duration(std::size_t duration) = 0;
	};

	/**
	 * \class tooltip
	 * \brief The tooltip class
	 */
	class tooltip
	{
		/**
		 * \brief Interface for generating tooltips
		 */
		class factory_interface
		{
		public:
			virtual ~factory_interface(){}
			/**
			 * \brief Creates a new tooltip
			 */
			virtual tooltip_interface* create() = 0;
			/**
			 * \brief Destroys the given tooltip
			 * \param tooltip Pointer to the tooltip
			 */
			virtual void destroy(tooltip_interface* tooltip) = 0;
		};

		template<typename TooltipWindow>
		class factory
			: public factory_interface
		{
			tooltip_interface * create() override
			{
				return new TooltipWindow;
			}

			void destroy(tooltip_interface* p) override
			{
				delete p;
			}
		};
	public:
		typedef factory_interface factory_if_type;

		/**
		 * \brief Creates a factory for the given tooltip window type
		 */
		template<typename TooltipWindow>
		static void make_factory()
		{
			_m_hold_factory(new factory<TooltipWindow>);
		}

		tooltip() = default;
		tooltip(window w, const ::std::string &tip){set(w,tip);}


		static void set(window, const ::std::string&);
		static void show(window, point pos, const ::std::string&, std::size_t duration);
		static void close();
	private:
		static void _m_hold_factory(factory_interface*);
	};//class tooltip
}//namespace nana
#endif
