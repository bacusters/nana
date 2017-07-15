#ifndef NANA_DETAIL_EVENTS_OPERATION_HPP
#define NANA_DETAIL_EVENTS_OPERATION_HPP

#include <nana/gui/events/general_events.hpp>
#include <unordered_set>

#if defined(STD_THREAD_NOT_SUPPORTED)
#include <nana/std_mutex.hpp>
#else
#include <mutex>
#endif

namespace nana
{
	namespace detail
	{
		/**
		 * \brief
		 */
		class events_operation
		{
		public:
			/**
			 * \brief Registers an event handle
			 */
			void register_evt(event_handle);
			void cancel(event_handle);
			void erase(event_handle);
		private:
			std::recursive_mutex mutex_;
			std::unordered_set<event_handle>	handles_;
		};
	}//end namespace detail
}//end namespace nana

#endif
