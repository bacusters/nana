/*
*	Color Schemes
*	Nana C++ Library(http://www.nanapro.org)
*	Copyright(C) 2003-2016 Jinhao(cnjinhao@hotmail.com)
*
*	Distributed under the Boost Software License, Version 1.0.
*	(See accompanying file LICENSE_1_0.txt or copy at
*	http://www.boost.org/LICENSE_1_0.txt)
*
*	@file: nana/gui/color_schemes.hpp
*	@description:
*/
#ifndef NANA_DETAIL_COLOR_SCHEMES_HPP
#define NANA_DETAIL_COLOR_SCHEMES_HPP

#include "widget_geometrics.hpp"

namespace nana
{
	namespace detail
	{
		/**
		 * \brief Interface for scheme factory
		 */
		class scheme_factory_interface
		{
		public:
			///Tag for identifying the factory
			struct factory_identifier{};
			virtual ~scheme_factory_interface() = default;

			/**
			 * \brief Returns the ID
			 * \returns The ID
			 */
			virtual factory_identifier* get_id() const = 0;
			/**
			 * \brief Returns a new scheme
			 * \returns The scheme
			 */
			virtual	widget_geometrics* create() = 0;

			/**
			 * \brief Returns a new scheme, based on another
			 * \param other The other scheme
			 * \returns The new scheme
			 */
			virtual widget_geometrics* create(widget_geometrics& other) = 0;
		};
		
		/**
		 * \brief Templated factory implementation for creating color schemes
		 */
		template<typename Scheme>
		class scheme_factory
			: public scheme_factory_interface
		{
		private:
			/**
			 * \brief Returns the factory ID
			 * \returns The ID
			 */
			factory_identifier* get_id() const override
			{
				return &fid_;
			}

			/**
			 * \brief Creates a new scheme
			 * \returns The new heap allocataed scheme
			 */
			widget_geometrics* create() override
			{
				return (new Scheme);
			}

			/**
			 * \brief Creates a new scheme by copying another
			 * \param other The scheme to copy
			 * \returns The new scheme
			 */
			widget_geometrics* create(widget_geometrics& other) override
			{
				return (new Scheme(static_cast<Scheme&>(other)));
			}
		private:
			///Identifier of this factory
			static factory_identifier fid_;
		};

		template<typename Scheme>
		scheme_factory_interface::factory_identifier scheme_factory<Scheme>::fid_;

		class color_schemes
		{
			struct implement;
			color_schemes(const color_schemes&) = delete;
			color_schemes(color_schemes&&) = delete;
			color_schemes& operator=(const color_schemes&) = delete;
			color_schemes& operator=(color_schemes&&) = delete;
		public:
			using scheme = widget_geometrics;

			color_schemes();
			~color_schemes();

			/**
			 * \brief Returns a template for the given scheme factory 
			 * \param factory The factory
			 * \returns A scheme template, to be copied when constructing a new scheme
			 */
			scheme&	scheme_template(scheme_factory_interface&& factory);

			/**
			 * \brief Creates a new scheme, using the template of the given factory as reference
			 * \param factory The factory
			 * \returns A new scheme, based on the template associated with the factory
			 */
			scheme* create(scheme_factory_interface&& factory);
		private:
			implement * impl_;
		};
	}//end namespace detail;
}//end namespace nana
#endif