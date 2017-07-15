#ifndef NANA_PAINT_PTDEFS_INCLUDED
#define NANA_PAINT_PTDEFS_INCLUDED

namespace nana
{
	namespace detail
	{
		struct native_font_signature;

		/**
		 * \brief Style of a font
		 */
		struct font_style
		{
			///The font weight. 400 is normal
			unsigned weight{ 400 };
			///Whether the font is italic
			bool italic{ false };
			///Whether the font is underlined
			bool underline{ false };
			///Whether the font is strikeout
			bool strike_out{ false };

			font_style() = default;
			/**
			 * \brief Constructs a fontstyle with the given parameters
			 * \param weight The weight
			 * \param italic Whether the font is italice
			 * \param underline Whether the font is underline
			 * \param strike_out Whether the font is strikeout
			 */
			font_style(unsigned weight, bool italic = false, bool underline = false, bool strike_out = false);
		};
	}//end namespace detail

	namespace paint
	{
		using native_font_type = ::nana::detail::native_font_signature*;
	}
}

#endif