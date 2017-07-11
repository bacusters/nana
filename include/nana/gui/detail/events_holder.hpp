#ifndef NANA_DETAIL_EVENTS_HOLDER_HPP
#define NANA_DETAIL_EVENTS_HOLDER_HPP
#include <memory>

namespace nana
{
	//Forward declare
	struct general_events;

	namespace detail
	{
		/**
		 * \brief Abstract class that holds events
		 */
		class events_holder
		{
		public:
			virtual ~events_holder(){}
			/**
			 * \brief Set the specified events on this object
			 * \param events The events
			 * \returns 
			 */
			virtual bool set_events(const std::shared_ptr<general_events>& events) = 0;
			/**
			 * \brief Returns the events this holder holds.
			 * \returns Pointer to events, given as base class nana::general_events*.
			 */
			virtual general_events* get_events() const = 0;
		};
	}//end namespace detail
}//end namespace nana
#endif
