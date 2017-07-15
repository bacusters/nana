#ifndef NANA_DETAIL_PLATFORM_ABSTRACTION_TYPES_HEADER_INCLUDED
#define NANA_DETAIL_PLATFORM_ABSTRACTION_TYPES_HEADER_INCLUDED
#include <nana/config.hpp>
#include <nana/paint/detail/ptdefs.hpp>

#include <string>

#ifdef NANA_X11
#	define NANA_USE_XFT
#endif

namespace nana
{
	/**
	 * \brief Abstract platform independent representation of font
	 */
	class font_interface
	{
	public:
		using font_style = detail::font_style;
		using native_font_type = paint::native_font_type;

		virtual ~font_interface() = default;

		/**
		 * \brief Returns the font family name
		 * \returns The font family name
		 */
		virtual const std::string& family() const = 0;
		/**
		 * \brief Returns the font size
		 * \returns The font size
		 */
		virtual double size() const = 0;
		/**
		 * \brief Returns the font style
		 * \returns The font style
		 */
		virtual const font_style & style() const = 0;
		/**
		 * \brief Returns the native handle to the font
		 * \returns The native handle
		 */
		virtual native_font_type native_handle() const = 0;
	};
}

#endif