/*
 *	A platform API implementation
 *	Copyright(C) 2003-2013 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/system/platform.hpp
 *	@description:
 *		this implements some API for platform-independent programming
 */

#ifndef NANA_SYSTEM_PLATFORM_HPP
#define NANA_SYSTEM_PLATFORM_HPP
#include <nana/deploy.hpp>

namespace nana
{
namespace system
{
	/**
	 * \brief Suspend current thread for a specified milliseconds.
	 * The precision of the time slept is depended on hardware.
	 * \param milliseconds The amount of seconds to susped the thread
	 */
	void sleep(unsigned milliseconds);

	/**
	 * \brief Returns the identifier of the calling thread
	 * \returns The thread ID
	 */
	unsigned long this_thread_id();

	/**
	 * \brief Retrieves the timestamp of the time the function is called.
	 * \returns The timestamp
	 */
	unsigned long timestamp();

	/**
	 * \brief Determines whether the given mouse button was pressed at the time the function is called.
	 * \param button The mouse button to check
	 * \returns Whether the mouse button was pressed 
	*/
	bool get_async_mouse_state(int button);

	/**
	 * \brief Opens a URL using the default browser on the system
	 * \param url The URL
	 */
	void open_url(const std::string& url);

}//end namespace system
}//end namespace nana

#endif
