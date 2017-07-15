/*
 *	A Timer Implementation
 *	Copyright(C) 2003-2015 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0. 
 *	(See accompanying file LICENSE_1_0.txt or copy at 
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *	@file: nana/gui/timer.hpp
 *	@description:
 *		A timer can repeatedly call a piece of code. The duration between 
 *	calls is specified in milliseconds. Timer is defferent from other graphics
 *	controls, it has no graphics interface.
 */

#ifndef NANA_GUI_TIMER_HPP
#define NANA_GUI_TIMER_HPP
#include <nana/gui/events/general_events.hpp>
#include <nana/push_ignore_diagnostic>

namespace nana
{  
    /**
     * \brief Event for timer elapse.s
     */
	struct arg_elapse
		: public event_arg
	{
		///Timer identifier
		long long id;	
	};

	/**
	 * \brief Timer class
	 * The nana::timer class makes it possible to execute a specified function at regular intervals. The following example
	 * executes the captured lambda every half second. 
	 * \code
	 * timer t;
	 * t.interval(500); //Every 500 ms
	 * t.elapsed([](elapseArgs){ std::cout << "Interval elapsed! Timer id was " << elapseArgs.id << std::endl;});
	 * t.start();
	 * \endcode
	 * Exact precision of the intervals is hardware dependent.
	 */
	class timer
	{
		///The implementation
		struct implement;

		///Delete copy constructor
		timer(const timer&) = delete;
		///Delete copy assignment
		timer& operator=(const timer&) = delete;
		///Delete move constructor
		timer(timer&&) = delete;
		///Delete move assignment
		timer& operator=(timer&&) = delete;
	public:
		/**
		 * \brief Constructs an empty timer
		 */
		timer();

		/**
		 * \brief Destructor
		 */
		~timer();

		/**
		 * \brief Connects an elapse listener to the events of the timer
		 * \param fn The elapse listener
		 */
		template<typename Function>
		void elapse(Function && fn)
		{
			elapse_.connect(std::forward<Function>(fn));
		}

		/**
		 * \brief Resets the timer
		 */
		void reset();
		/**
		 * \brief Starts the timer
		 */
		void start();
		/**
		 * \brief Returns whether the timer was started
		 * \returns Whether the timer was started
		 */
		bool started() const;
		/**
		 * \brief Stops the timer
		 */
		void stop();

		/**
		 * \brief Set the duration between calls of the elapse listeners
		 * \param milliseconds The interval
		 */
		void interval(unsigned milliseconds);

		/**
		 * \brief Returns the interval in milliseconds between elapse listener calls.
		 * \returns The interval
		 */
		unsigned interval() const;
	private:
		///The elapse event emitter
		nana::basic_event<arg_elapse> elapse_;
		///The implementation
		implement * const impl_;
	};
}//end namespace nana
#include <nana/pop_ignore_diagnostic>
#endif
