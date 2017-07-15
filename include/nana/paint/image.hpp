/*
 *	Paint Image Implementation
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2016 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0. 
 *	(See accompanying file LICENSE_1_0.txt or copy at 
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/paint/image.hpp
 *	@description:	class image is used for load an image file into memory.
 */

#ifndef NANA_PAINT_IMAGE_HPP
#define NANA_PAINT_IMAGE_HPP

#include "graphics.hpp"

namespace nana
{
namespace paint
{
    /// load a picture file
	class image
	{
		//Friend accessor
		friend class image_accessor;

		//Some function?
		typedef bool (image::* unspecified_bool_t)() const; 
	public:
		//Implementation of image types
		class image_impl_interface;

		image() noexcept;
		image(const image&);
		image(image&&);
		explicit image(const ::std::string& file);
		explicit image(const ::std::wstring& file);

		~image();
		image& operator=(const image& rhs);
		image& operator=(image&&);
		
		/**
		* \brief Creates an image from the specified file
		* \param file The file
		* \returns Whether the image was loaded succesfully
		*/
		bool open(const ::std::string& file);

		/**
		 * \brief Creates an image from the specified file
		 * \param file The file
		 * \returns Whether the image was loaded succesfully
		 */
		bool open(const ::std::wstring& file);
		
		/**
		 * \brief Interprets the specified data as an image.
		 * Checks what type of image the data represents
		 * \param data The image data
		 * \param bytes The number of bytes in the data
		 * \returns Whether the interpretation was succesful.
		 */
		bool open(const void* data, std::size_t bytes);
		
		/**
		 * \brief Returns whether no image is loaded into the class
		 * \returns Whether no image is loaded.
		 */
		bool empty() const noexcept;
		
		/**
		 * \brief Converts the image to a bool returning member function which checks emptiness.
		 */
		operator unspecified_bool_t() const;

		/**
		 * \brief Destroys the current image
		 */
		void close() noexcept;

		bool alpha() const noexcept;
		nana::size size() const noexcept;
		void paste(graphics& dst, const point& p_dst) const;
		void paste(const nana::rectangle& r_src, graphics& dst, const point& p_dst) const;///< Paste the area of picture specified by r_src into the destination graphics specified by dst at position p_dst.
		void stretch(const nana::rectangle& r_src, graphics& dst, const nana::rectangle& r_dst) const;///<Paste the picture into the dst, stretching or compressing the picture to fit the given area.
	private:
		std::shared_ptr<image_impl_interface> image_ptr_;
	};//end class image

}//end namespace paint
}//end namespace nana

#endif

