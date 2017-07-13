/*
 *	Background Effects Implementation
 *	Copyright(C) 2003-2013 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/effects.hpp
 *
 */
#ifndef NANA_GUI_EFFECTS_HPP
#define NANA_GUI_EFFECTS_HPP
#include <cstddef>
#include <nana/paint/graphics.hpp>

namespace nana
{
	namespace effects
	{
		/**
		 * \enum edge_nimbus
		 * \brief Enum for nimbus types
		 */
		enum class edge_nimbus
		{
			none, active = 0x1, over = 0x2
		};

		/**
		 * \brief Interface for background rendering
		 */
		class bground_interface
		{
		public:
			typedef paint::graphics & graph_reference;

			virtual ~bground_interface() = 0;
			virtual void take_effect(window, graph_reference) const = 0;
		};

		/**
		 * \brief Factory for background
		 */
		class bground_factory_interface
		{
			//Define friend
			friend class effects_accessor;
		public:
			virtual ~bground_factory_interface() = 0;
		private:
			/**
			 * \brief Creates a new background interface strategy
			 * 
			 */
			virtual bground_interface * create() const = 0;
		};

		/**
		 * \brief Implementation for creating transparent background rendering object.
		 */
		class bground_transparent
			: public bground_factory_interface
		{
		public:
			/**
			 * \brief Constructor. Sets the percentage of transparency
			 * \param percent Transparency percentage
			 */
			bground_transparent(std::size_t percent);
		private:
			/**
			 * \brief Creates the background rendering object
			 * \returns Pointer to renderer.
			 */
			bground_interface* create() const override;
		private:
			std::size_t percent_;
		};

		/**
		 * \brief Factory for the blur background effect.
		 */
		class bground_blur
			: public bground_factory_interface
		{
		public:
			/**
			* \brief Constructor. Sets the radius of the blur
			* \param radius Blur radius
			*/
			bground_blur(std::size_t radius);
		private:
			/**
			* \brief Creates the background rendering object
			* \returns Pointer to renderer.
			*/
			bground_interface * create() const override;
		private:
			///The blur radius
			std::size_t radius_;
		};
	}
}//end namespace nana
#endif
