#ifndef NANA_DETAIL_BEDROCK_PI_DATA_HPP
#define NANA_DETAIL_BEDROCK_PI_DATA_HPP

#include <nana/push_ignore_diagnostic>

#include <nana/gui/detail/bedrock.hpp>
#include "color_schemes.hpp"
#include <nana/gui/events/events_operation.hpp>
#include <nana/gui/windows/window_manager.hpp>
#include <set>

namespace nana
{
	namespace detail
	{
		/**
		 * \brief Platform independent bedrock data
		 */
		struct bedrock::pi_data
		{
			///The color scheme
			color_schemes				scheme;
			///Event operations
			events_operation			evt_operation;
			///Window manager
			window_manager				wd_manager;
			///Set of form windows
			std::set<core_window_t*>	auto_form_set;
			///Whether a shortkey occurred
			bool shortkey_occurred{ false };

			///Representation for menu
			struct menu_rep
			{
				core_window_t*	taken_window{ nullptr };
				bool			delay_restore{ false };
				native_window_type window{ nullptr };
				native_window_type owner{ nullptr };
				bool	has_keyboard{ false };
			}menu;
		};
	}
}

#include <nana/pop_ignore_diagnostic>

#endif
