/**
 *	A Form Implementation
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2015 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0. 
 *	(See accompanying file LICENSE_1_0.txt or copy at 
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/widgets/form.hpp
 */

#ifndef NANA_GUI_WIDGET_FORM_HPP
#define NANA_GUI_WIDGET_FORM_HPP

#include <nana/gui/widgets/widget.hpp>
#include <nana/gui/layout/place.hpp>

namespace nana
{
	//Forward declare
	class place;

	namespace drawerbase
	{
		namespace form
		{
			/**
			 * \brief Drawer trigger for nana::form class
			 */
			class trigger: public drawer_trigger
			{
			public:
				/**
				 * \brief Handles the attaching of the s
				 * \param widget Reference to the widget
				 * \param graph Reference to the graphcis object
				 */
				void attached(widget_reference widget, graph_reference graph)	override;
				void refresh(graph_reference)	override;
			private:
				widget*	wd_{nullptr};
			};

			/**
			 * \brief Base inplementation of the form class.
			 */
			class form_base
				: public widget_object<category::root_tag, drawerbase::form::trigger, detail::events_root_extension>
			{
			public:
				/**
				 * \brief Constructor for the base form
				 * \param owner Owner of this form, if present
				 * \param nested Whether this window is nested in the owner
				 * \param area Target area to occupy
				 * \param appearance Appearance for the window
				 */
				form_base(window owner, bool nested, const rectangle& area, const appearance& appearance);
				
				//place methods
				/**
				 * \brief Returns the associated place object
				 * \returns nana::place object.
				 */
				place & get_place();
				/**
				 * \brief Sets up the layout for the window
				 * \param div_text Text describing the window layout
				 */
				void div(const char* div_text);
				/**
				 * \brief Retrieves a field from the layout by name
				 * \param field_name The name of the field
				 * \returns Reference to the field
				 */
				place::field_reference operator[](const char* field_name);
				/**
				 * \brief Applies layouting on the window, placing children at the appropriate location
				 */
				void collocate() noexcept;
			private:
				///Reference to place object
				std::unique_ptr<place> place_;
			};
		}//end namespace form
	}//end namespace drawerbase

	/**
	 * \brief Pop-up window. Is different from other window widgets: its default constructor creates the window.
	 * \see nana::appearance
	 */
	class form
		: public drawerbase::form::form_base
	{
	public:
		using appear = ::nana::appear;

		/**
		 * \brief Creates a window using the specified rect for locationand dimensions., and with the specified appearance. Creates a form owned by the desktop.
		 */
		form(const rectangle& = API::make_center(300, 200), const appearance& = {});	//Default constructor
		form(const form&, const ::nana::size& = { 300, 200 }, const appearance& = {});	//Copy constructor
		form(window, const ::nana::size& = { 300, 200 }, const appearance& = {});
        /// Creates a window at the point and size specified by rect, with the specified appearance. This window is always floating above its owner.
		form(window, const rectangle&, const appearance& = {});

		/**
		 * \brief Makes the form a modal window, blocking other interactions until the window is closed.
		 */
		void modality() const;


		void wait_for_this();
	};

	/**
	 * \brief Implementation of a form, nested in another form.
	 */
	class nested_form
		: public drawerbase::form::form_base
	{
	public:
		using appear = ::nana::appear;

		/**
		 * Constructs a form within another form
		 * \param owner The owner form
		 * \param rect The target area to occupy
		 * \param appear The appearance for the form
		 */
		nested_form(const form& owner, const rectangle& rect = {}, const appearance& appear = {});
		
		/**
		* Constructs a form within another nested form
		* \param owner The owner form
		* \param rect The target area to occupy
		* \param appear The appearance for the form
		*/
		nested_form(const nested_form& owner, const rectangle& rect = {}, const appearance& appear = {});

		nested_form(window, const appearance&);
		nested_form(window, const rectangle& = {}, const appearance& = {});
	};
}//end namespace nana
#endif
