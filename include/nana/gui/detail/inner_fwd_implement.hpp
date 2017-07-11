/*
 *	Implementations of Inner Forward Declaration
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2016 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/detail/inner_fwd_implement.hpp
 *
 */

#ifndef NANA_GUI_INNER_FWD_IMPLEMENT_HPP
#define NANA_GUI_INNER_FWD_IMPLEMENT_HPP

#include <nana/push_ignore_diagnostic>
#include "inner_fwd.hpp"
#include "basic_window.hpp"
#include "../../paint/graphics.hpp"

#include <map>

namespace nana{
	namespace detail
	{
		/**
		 * \brief Container for shortkeys
		 */
		class shortkey_container
		{
			///Type of the container item
			struct item_type;

			//Noncopyable
			shortkey_container(const shortkey_container&) = delete;
			shortkey_container& operator=(const shortkey_container&) = delete;

			shortkey_container& operator=(shortkey_container&&) = delete;
		public:
			shortkey_container();

			shortkey_container(shortkey_container&&);

			~shortkey_container();

			/**
			 * \brief Clears the container
			 */
			void clear();

			/**
			 * \brief Creates 
			 */
			bool make(window wd, unsigned long key);

			/**
			 * \brief Removes all shortkey definitions for the specified window
			 * \param wd The window
			 */
			void umake(window wd);

			std::vector<unsigned long> keys(window wd) const;

			window find(unsigned long key) const;
		private:
			struct implementation;
			implementation * impl_;
		};

		/**
		 *
		 */
		struct root_misc
		{
			///Pointer to the window
			basic_window * window;
			///The graphics object
			nana::paint::graphics	root_graph;
			///Container for shortkeys
			shortkey_container		shortkeys;

			///
			struct condition_rep
			{
				bool			ignore_tab;			//ignore tab when the focus is changed by TAB key.
				basic_window*	pressed;			//The handle to a window which has been pressed by mouse left button.
				basic_window*	pressed_by_space;	//The handle to a window which has been pressed by SPACEBAR key.
				basic_window*	hovered;			//the latest window that mouse moved
			}condition;

			root_misc(root_misc&& other);
			root_misc(basic_window * wd, unsigned width, unsigned height);
		};//end struct root_misc


		/**
		 * \brief 
		 */
		class root_register
		{
			//Noncopyable
			root_register(const root_register&) = delete;
			root_register& operator=(const root_register&) = delete;

			//Nonmovable
			root_register(root_register&&) = delete;
			root_register& operator=(root_register&&) = delete;
		public:
			root_register();
			~root_register();

			/**
			 * \brief Inserts the window in the register, with the given extra data
			 * \param window The window to insert
			 * \param miscPointer Pointer to miscelaneous data pointer
			 * \returns The pointer to the miscelaneous data, now present associated with the window in the register. 
			 */
			root_misc* insert(native_window_type window, root_misc&& miscPointer);

			/**
			 *
			 */
			root_misc * find(native_window_type);

			/**
			 * \brief Removes the given window from the register
			 * \param window The window to remove
			 */
			void erase(native_window_type window);
		private:
			struct implementation;
			implementation * const impl_;
		};
	}
}//end namespace nana

#include <nana/pop_ignore_diagnostic>

#endif	//NANA_GUI_INNER_FWD_IMPLEMENT_HPP
