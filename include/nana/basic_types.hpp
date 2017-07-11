/*
 *	Basic Types definition
 *	Nana C++ Library(http://www.nanapro.org)
 *	Copyright(C) 2003-2017 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0. 
 *	(See accompanying file LICENSE_1_0.txt or copy at 
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/basic_types.hpp
 */

#ifndef NANA_BASIC_TYPES_HPP
#define NANA_BASIC_TYPES_HPP

#include <nana/deploy.hpp>
#include <cctype>
#include <nana/push_ignore_diagnostic>

namespace nana
{
	/// A constant value for the invalid position.
	const std::size_t npos = static_cast<std::size_t>(-1);


	template<typename CharT>
	struct casei_char_traits
		: public std::char_traits<CharT>
	{
		typedef CharT char_type;

		
		//static constexpr bool eq(char_type c1, char_type c2) noexcept
		//VC2012 does not support constexpr and noexcept keywords
		static bool eq(char_type c1, char_type c2)
		{
			return std::toupper(c1) == std::toupper(c2);
		}

		//static constexpr bool lt(char_type c1, char_type c2) noexcept
		//VC2012 does not support constexpr and noexcept keywords
		static bool lt(char_type c1, char_type c2)
		{
			return std::toupper(c1) < std::toupper(c2);
		}

		static int compare(const char_type* s1, const char_type* s2, std::size_t n)
		{
			while(n--)
			{
				char_type c1 = std::toupper(*s1);
				char_type c2 = std::toupper(*s2);
				if(c1 < c2) return -1;
				if(c1 > c2) return 1;
				++s1;
				++s2;
			}
			return 0;
		}

		static const char_type* find(const char_type* s, std::size_t n, const char_type& a)
		{
			char_type ua = std::toupper(a);
			const char_type * end = s + n;
			while((s != end) && (std::toupper(*s) != ua))
				++s;
			return (s == end ? nullptr : s);
		}
	};

	using cistring = std::basic_string<char, casei_char_traits<char>>;
	using ciwstring = std::basic_string<wchar_t, casei_char_traits<wchar_t>>;
	

	namespace detail
	{
		struct drawable_impl_type;	//declaration, defined in platform_spec.hpp
	}

	namespace paint
	{
		typedef nana::detail::drawable_impl_type*	drawable_type;
	}

	/**
	 * \brief Enum for possible mouse actions
	 */
	enum class mouse_action
	{
		begin, normal = begin, normal_captured, hovered, pressed, end
	};

	/**
	 * \brief Enum for possible states
	 */
	enum class element_state
	{
		normal,
		hovered,
		focus_normal,
		focus_hovered,
		pressed,
		disabled
	};

	/**
	 * \brief Union for ARGB pixels
	 */
	union pixel_argb_t
	{
		struct element_tag
		{
			unsigned char blue;
			unsigned char green;
			unsigned char red;
			unsigned char alpha_channel;
		}element;
		unsigned value;
	};

	/**
	 * \brief Union for RGBA pixels
	 */
	union pixel_rgba_t
	{
		struct element_tag
		{
			unsigned char alpha_channel;
			unsigned char blue;
			unsigned char green;
			unsigned char red;
		}element;
		unsigned value;
	};

	using pixel_color_t = pixel_argb_t;

	/// See extended CSS color keywords (4.3) in http://www.w3.org/TR/2011/REC-css3-color-20110607/
	enum class colors
	{
		alice_blue = 0xf0f8ff,
		antique_white = 0xfaebd7,
		aqua	= 0xFFFF,
		aquamarine = 0x7fffd4,
		azure	= 0xf0ffff,
		beige	= 0xf5f5dc,
		bisque	= 0xffe4ce,
		black	= 0x0,
		blanched_almond = 0xffebcd,
		blue	= 0x0000FF,
		blue_violet = 0x8a2be2,
		brown	= 0xa52a2a,
		burly_wood = 0xdeb887,
		cadet_blue = 0x5f9ea0,
		chartreuse = 0x7fff00,
		chocolate = 0xd2691e,
		coral = 0xff7f50,
		cornflower_blue = 0x6495ed,
		cornsilk = 0xfff8dc,
		crimson	= 0xdc143c,
		cyan	= 0xffff,
		dark_blue = 0x8b,
		dark_cyan = 0x8b8b,
		dark_goldenrod = 0xb8860b,
		dark_gray = 0xa9a9a9,
		dark_green = 0x6400,
		dark_grey = dark_gray,
		dark_khaki = 0xbdb76b,
		dark_magenta = 0x8b008b,
		dark_olive_green = 0x556b2f,
		dark_orange = 0xff8c00,
		dark_orchid = 0x9932cc,
		dark_red = 0x8b0000,
		dark_salmon = 0xe9976a,
		dark_sea_green = 0x8fbc8f,
		dark_slate_blue = 0x483d8b,
		dark_slate_gray = 0x2f4f4f,
		dark_slate_grey = 0x2f4f4f,
		dark_turquoise = 0xced1,
		dark_violet = 0x9400d3,
		deep_pink = 0xff1493,
		deep_sky_blue = 0xbfff,
		dim_gray = 0x696969,
		dim_grey = dim_gray,
		dodger_blue = 0x1e90ff,
		firebrick = 0xb22222,
		floral_white = 0xfffaf0,
		forest_green = 0x228b22,
		fuchsia	= 0xFF00FF,
		gainsboro = 0xdcdcdc,
		ghost_white = 0xf8f8ff,
		gold = 0xffd700,
		goldenrod = 0xdaa520,
		gray = 0x808080,
		green = 0x008000,
		green_yellow = 0xadff2f,
		grey = gray,
		honeydew = 0xf0fff0,
		hot_pink = 0xff69b4,
		indian_red = 0xcd5c5c,
		indigo	= 0x4b0082,
		ivory = 0xfffff0,
		khaki = 0xf0e68c,
		lavendar = 0xe6e6fa,
		lavender_blush = 0xfff0f5,
		lawn_green = 0x7cfc00,
		lemon_chiffon = 0xfffacd,
		light_blue = 0xadd8e6,
		light_coral = 0xf08080,
		light_cyan = 0xe0ffff,
		light_goldenrod_yellow = 0xfafad2,
		light_gray = 0xd3d3d3,
		light_green = 0x90ee90,
		light_grey = light_gray,
		light_pink = 0xffb6c1,
		light_salmon = 0xffa07a,
		light_sea_green = 0x20b2aa,
		light_sky_blue = 0x87cefa,
		light_slate_gray = 0x778899,
		light_slate_grey = light_slate_gray,
		light_steel_blue = 0xb0c4de,
		light_yellow = 0xffffe0,
		lime	= 0x00FF00,
		lime_green = 0x32cd32,
		linen = 0xfaf0e6,
		magenta = 0xff00ff,
		maroon	= 0x800000,
		medium_aquamarine = 0x66cdaa,
		medium_blue = 0xcd,
		medium_orchid = 0xba55d3,
		medium_purple = 0x9370db,
		medium_sea_green = 0x3cb371,
		medium_slate_blue = 0x7b68ee,
		medium_spring_green = 0xfa9a,
		medium_turquoise = 0x48d1cc,
		medium_violet_red = 0xc71585,
		midnight_blue = 0x191970,
		mint_cream = 0xf5fffa,

		misty_rose = 0xffe4e1,
		moccasin = 0xffe4b5,
		navajo_white = 0xffdead,
		navy	= 0x000080,
		old_lace = 0xfdf5e6,
		olive	= 0x808000,
		olive_drab = 0x6b8e23,
		orange	= 0xffa500,
		orange_red = 0xff4500,
		orchid	= 0xda70d6,
		pale_goldenrod = 0xeee8aa,
		pale_green	= 0x98fb98,
		pale_turquoise = 0xafeeee,
		pale_violet_red = 0xdb7093,
		papaya_whip = 0xffefd5,
		peach_puff = 0xffdab9,
		peru	= 0xcd853f,
		pink	= 0xffc0cb,
		plum	= 0xdda0dd,
		powder_blue = 0xb0e0e6,
		purple	= 0x800080,
		red		= 0xFF0000,
		rosy_brown = 0xbc8f8f,
		royal_blue = 0x4169e1,
		saddle_brown = 0x8b4513,
		salmon = 0xfa8072,
		sandy_brown = 0xf4a460,
		sea_green = 0x2e8b57,
		sea_shell = 0xfff5ee,
		sienna	= 0xa0522d,
		silver	= 0xc0c0c0,
		sky_blue = 0x87ceeb,
		slate_blue = 0x6a5acd,
		slate_gray = 0x708090,
		slate_grey = 0x708090,
		snow	= 0xfffafa,
		spring_green = 0xff7f,
		steel_blue = 0x4682b4,
		tan		= 0xd2b48c,
		teal	= 0x008080,
		thistle	= 0xd8bfd8,
		tomato	= 0xff6347,
		turquoise = 0x40e0d0,
		violet	= 0xee82ee,
		wheat	= 0xf5deb3,
		white	= 0xFFFFFF,
		white_smoke = 0xf5f5f5,
		yellow	= 0xFFFF00,
		yellow_green = 0x9acd32,

		//temporary defintions, these will be replaced by color schema
		button_face_shadow_start = 0xF5F4F2,
		button_face_shadow_end = 0xD5D2CA,
		button_face = 0xD4D0C8 , //,light_cyan
		dark_border = 0x404040,
		gray_border = 0x808080,
		highlight = 0x1CC4F7
	};

	//Some helper types to identify an integer as color.
	enum class color_rgb :	unsigned{};
	enum class color_argb:	unsigned{};
	enum class color_rgba : unsigned{};

	/**
	 * \brief Class for representing colors with high precision
	 */
	class color
	{
	public:
		/**
		 * \brief Initialize color with black value
		 */
		color() = default;
		
		/**
		* \brief Intialize color from a predefined color
		* \param color The predefined color
		*/
		color(colors color);

		/**
		* \brief Intialize color from a predefined color
		* \param color The predefined color
		* \param alpha Alpha value
		*/
		color(colors color, double alpha);

		/**
		* \brief Intialize color from RGB element
		* \param color RGB color
		*/
		color(color_rgb color);
		
		/**
		* \brief Intialize color from ABGR element
		* \param color ABGR color
		*/
		color(color_argb color);
		
		/**
		 * \brief Intialize color from RGBA element
		 * \param color RGBA color
		 */
		color(color_rgba color);

		/**
		 * \brief Initialize color from RGBA values
		 * \param red Red component
		 * \param green Green component
		 * \param blue Blue component
		 * \param alpha Alpha component. Defaults to 1.0
		 */
		color(unsigned red, unsigned green, unsigned blue, double alpha = 1.0);

		/**
		 * \brief Initializes the color with a CSS-like rgb string.
		 * \param css_rgb String representing rgb values
		 */
		explicit color(std::string css_rgb);

		/**
		 * \brief Sets alpha channel
		 * \param alpha The alpha value
		 * \returns The modified color.
		 */
		color& alpha(double alpha);

		/**
		 * \brief Initializes the color from RGB values
		 * \param red Red value
		 * \param green Green value
		 * \param blue Blue value
		 * \returns The modified color
		 */
		color& from_rgb(unsigned red, unsigned green, unsigned blue);

		/// Sets color with a HSL value.
		/// @param hue in range of [0, 360]
		/// @param saturation in range of [0, 1]
		/// @param lightness  in range of [0, 1]
		color& from_hsl(double hue, double saturation, double lightness);	///< immutable alpha channel

		/**
		 * \brief Linearly interpolates this color with the given color, where alpha specifies the amount of new colour.
		 * \param blending_color The color to blend this color with.
		 * \param alpha Amount of the new colour to use.
		 * \returns The blended color.
		 */
		color blend(const color& blending_color, double alpha) const;

		/**
		 * \brief Determines whether the color is completely transparent.
		 * \returns Whether this color is transparent
		 */
		bool invisible() const;
		
		/**
		 * \brief Returns the color as a pixel color.
		 * \returns The pixel color
		 */
		pixel_color_t px_color() const;
		
		/**
		* \brief Returns the color as an ARGB pixel color.
		* \returns The pixel color
		*/
		pixel_argb_t argb() const;
		
		/**
		* \brief Returns the color as an RGBA pixel color.
		* \returns The pixel color
		*/
		pixel_rgba_t rgba() const;

		/**
		 * \brief Returns a reference to the Red component
		 * \returns Reference to the component
		 */
		const double& r() const;
		
		/**
		* \brief Returns a reference to the Green component
		* \returns Reference to the component
		*/
		const double& g() const;
		
		/**
		* \brief Returns a reference to the Blue component
		* \returns Reference to the component
		*/
		const double& b() const;
		
		/**
		* \brief Returns a reference to the Apha component
		* \returns Reference to the component
		*/
		const double& a() const;

		bool operator==(const color& other) const;
		bool operator!=(const color& other) const;

		friend color operator+(const color&, const color&);
	private:
		double r_{ 0.0 };
		double g_{ 0.0 };
		double b_{ 0.0 };
		double a_{ 0.0 };	//invisible
	};

	template<typename T>
	struct basic_point
	{
		//typedef-names
		using value_type = T;

		///X member
		value_type x{};
		///Y member
		value_type y{};

		//member functions
		basic_point() = default;

		basic_point(value_type x, value_type y)
			: x{ x }, y{y}
		{}

		bool operator==(const basic_point& other) const noexcept
		{
			return (x == other.x && y == other.y);
		}

		bool operator!=(const basic_point& other) const noexcept
		{
			return (x != other.x || y != other.y);
		}

		bool operator<(const basic_point& other) const noexcept
		{
			return ((y < other.y) || (y == other.y && x < other.x));
		}

		bool operator<=(const basic_point& other) const noexcept
		{
			return ((y < other.y) || (y == other.y && x <= other.x));
		}

		bool operator>(const basic_point& other) const noexcept
		{
			return ((y > other.y) || (y == other.y && x > other.x));
		}

		bool operator>=(const basic_point& other) const noexcept
		{
			return ((y > other.y) || (y == other.y && x >= other.x));
		}

		basic_point operator-(const basic_point& other) const noexcept
		{
			return{ x - other.x, y - other.y };
		}

		basic_point operator+(const basic_point& other) const noexcept
		{
			return{ x + other.x, y + other.y };
		}

		basic_point& operator-=(const basic_point& other) noexcept
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		basic_point& operator+=(const basic_point& other) noexcept
		{
			x += other.x;
			y += other.y;
			return *this;
		}
	};

	using point = basic_point<int>;
	using upoint = basic_point<unsigned>;

	/**
	 * \brief Struct representing dimensions
	 */
	struct size
	{
		using value_type = unsigned;
		/**
		 * \brief Creates an empty size with width = height = 0.
		 */
		size();

		/**
		 * \brief Creates a size struct of the given dimensions
		 * \param width The width
		 * \param height The height
		 */
		size(value_type width, value_type height);

		/**
		 * \brief Checks whether the size is empty
		 * Emptyness is defined as width * height == 0
		 * \returns Whether the size is empty
		 */
		bool empty() const;

		/**
		 * \brief Checks whether the given point lies within the dimensions.
		 * It is assumed that the size struct represent a rectangle at location (0,0). Then, the check is
		 * made whether the x- and y-coordinates of the point lie within [0, width) and [0, height).
		 * \param point The point to test
		 * \returns Whether the point lies within this size struct.
		 */
		bool is_hit(const point& point) const;

		/**
		 * \brief Swaps the width and height of the size struct
		 * \returns The modified size struct
		 */
		size& shift();

		/**
		 * \brief Checks whether this size equals another size struct
		 * \param rhs The other size struct.
		 * \returns Whether the sizes are equal.
		 */
		bool operator==(const size& rhs) const;
		
		/**
		* \brief Checks whether this size does not equal another size struct
		* \param rhs The other size struct.
		* \returns Whether the sizes are not equal.
		*/
		bool operator!=(const size& rhs) const;

		/**
		 * \brief Operator for adding another size to this size.
		 * \param other The other size
		 * \returns A new size struct with the combined width and height of this size and the other size.
		 */
		size operator+(const size& other) const;

		///The width member
		value_type width;
		///The height member
		value_type height;
	};

	/**
	 * \brief Rectangle representation
	 */
	struct rectangle
	{
		/**
		 * \brief Creates a rectangle of dimensions (0, 0) at (0, 0).
		 */
		rectangle();
		/**
		 * \brief Creates a rectangle at the specified location with the given dimensions
		 * \param x X coordinate
		 * \param y Y coordinate
		 * \param width The width of the rectangle
		 * \param height The height of the rectangle
		 */
		rectangle(int x, int y, unsigned width, unsigned height);
		
		/**
		 * \brief Creates a rectangle of the specified dimensions at (0,0).
		 * \param size The dimensions of the rectangle
		 */
		explicit rectangle(const size & size);
		/**
		 * \brief Creates a rectangle of the specified dimensions at the specified location
		 * \param point The location of the rectangle
		 * \param dims The dimensions of the rectangle. Defaults to empty size
		 */
		explicit rectangle(const point& point, const size& dims = size());

		/**
		 * \brief Checks equality between this rectangle and the other
		 * \param rhs The other rectangle
		 * \returns Whether the rectangles are equal
		 */
		bool operator==(const rectangle& rhs) const;
		
		/**
		* \brief Checks nonequality between this rectangle and the other
		* \param rhs The other rectangle
		* \returns Whether the rectangles are not equal
		*/
		bool operator!=(const rectangle& rhs) const;

		/**
		 * \brief Returns the position of the rectangle
		 * \returns Position of the rectangle as nana::point struct.
		 */
		point position() const noexcept;

		/**
		* \brief Sets the position of the rectangle and returns this modified rectangle
		* \param point New position for the rectangle.
		* \returns The modified rectangle
		*/
		rectangle& position(const point& point) noexcept;

		/**
		* \brief Returns the dimensions of the rectangle
		* \returns Dimensions of the rectangle as nana::size struct.
		*/
		size dimension() const noexcept;

		/**
		* \brief Sets the dimensions of the rectangle and returns this modified rectangle
		* \param size New dimensions for the rectangle.
		* \returns The modified rectangle
		*/
		rectangle& dimension(const size& size) noexcept;

		/**
		 * \brief Shrinks the rectangle towards the center by the given amount of pixels.
		 * \param pixels The number of pixels to shrink from each side of the rectangle
		 * \returns The modified rectangle
		 */
		rectangle& pare_off(int pixels);

		/**
		 * \brief Returns the right side of the rectangle
		 * This is calculated as x + width
		 * \returns The right side location
		 */
		int right() const noexcept;
		
		/**
		* \brief Returns the bottom side of the rectangle
		* This is calculated as y + height
		* \returns The bottom side location
		*/
		int bottom() const noexcept;

		/**
		 * \brief Returns whether the given x and y coordinate lie within the rectangle.
		 * Checks that x is within [this->x, this->x + width) and y is within [this->y, this->y + height)
		 * \param x The x coordinate
		 * \param y The y coordinate
		 * \returns Whether the location is contained in the rectangle.
		 */
		bool is_hit(int x, int y) const;

		/**
		 * \brief Convenience overload for is_hit(int x, int y), accepting a point instead.
		 * \param pos The location
		 * \returns Whether the location is contained in the rectangle.
		 */
		bool is_hit(const point& pos) const;

		/**
		 * \brief Checks whether the rectangle is empty
		 * Empty is defined as width * height == 0.
		 * \returns Whether the rectangle is empty
		 */
		bool empty() const;

		/**
		 * \brief Transposes the rectangle, swapping x and y, and width and height
		 * \returns The modified rectangle.
		 */
		rectangle& shift();

		int x;
		int y;
		unsigned width;
		unsigned height;
	};

	class rectangle_rotator
	{
	public:
		rectangle_rotator(bool rotated, const ::nana::rectangle& area);

		int x() const;
		int & x_ref();
		int y() const;
		int & y_ref();
		unsigned w() const;
		unsigned & w_ref();
		unsigned h() const;
		unsigned & h_ref();

		int right() const;
		int bottom() const;
		const ::nana::rectangle& result() const;
	private:
		bool rotated_;
		::nana::rectangle area_;
	};//end class rectangle_rotator

	enum class arrange
	{
		unknown, horizontal, vertical, horizontal_vertical
	};

	///The definition of horizontal alignment
	enum class align
	{
		left, center, right
	};

	///The definition of vertical alignment
	enum class align_v
	{
		top, center, bottom
	};

	///The definition of the four corners of the world
	enum class direction
	{
		north,
		south,
		east,
		west,
		southeast
	};
}//end namespace nana
#include <nana/pop_ignore_diagnostic>
#endif


