#ifndef NANA_PAINT_TEXT_RENDERER_HPP
#define NANA_PAINT_TEXT_RENDERER_HPP
#include <nana/paint/graphics.hpp>

namespace nana
{
	namespace paint
	{
		/**
		 * \brief Text renderer
		 */
		class text_renderer
		{
		public:
			using graph_reference = graphics &;
			
			text_renderer(graph_reference graph, align = align::left);

			/**
			 * \brief Retrieves the extents of the specified text
			 * \param x The x coordinate for the text
			 * \param y The y coordinate for the text
			 * \param text The text 
			 * \param len The length of the text?
			 * \param restricted_pixels The restriction on the number of pixels in the horizontal direction
			 */
			nana::size extent_size(int x, int y, const wchar_t* text, std::size_t len, unsigned restricted_pixels) const;

			/**
			 * 
			 */
			void render(const point& location, const wchar_t* string, std::size_t len);
			void render(const point&, const wchar_t*, std::size_t len, unsigned restricted_pixels, bool omitted);
			void render(const point&, const wchar_t*, std::size_t len, unsigned restricted_pixels);
		private:
			graph_reference graph_;
			align text_align_;
		};

		/**
		 * \brief Class for drawing aligned text.
		 */
		class aligner
		{
		public:
			using graph_reference = graphics&;

			/**
			 * \param graph Reference to a graphics object
			 * \param text_align Alignment of text
			 */
			aligner(graph_reference graph, align text_align = align::left);
			/**
			 * \brief Constructor
			* \param graph Reference to a graphics object
			* \param text_align Alignment of text
			* \param text_align_if_too_long Alignment of text if the rendered string would be larger than the text area
			*/
			aligner(graph_reference graph, align text_align, align text_align_if_too_long);

			/**
			 * \brief Draws a text with specified text alignment.
			 * \param text	Text to draw
			 * \param pos	Postion where the text to draw
			 * \param width The width of text area. If the pixels of text is larger than this parameter, it draws ellipsis  
			 */
			void draw(const std::string& text, point pos, unsigned width);
			/**
			 * \brief Draws a text with specified text alignment.
			 * \param text	Text to draw
			 * \param pos	Postion where the text to draw
			 * \param width The width of text area. If the pixels of text is larger than this parameter, it draws ellipsis
			*/
			void draw(const std::wstring& text, point pos, unsigned width);
		private:
			///Reference to graphics object
			graph_reference graph_;
			///Alignment for text
			align text_align_;
			///Alignment for text that is too long
			align text_align_ex_;
		};
	}
}

#endif
