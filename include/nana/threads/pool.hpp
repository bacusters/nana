/*
 *	A Thread Pool Implementation
 *	Copyright(C) 2003-2016 Jinhao(cnjinhao@hotmail.com)
 *
 *	Distributed under the Boost Software License, Version 1.0.
 *	(See accompanying file LICENSE_1_0.txt or copy at
 *	http://www.boost.org/LICENSE_1_0.txt)
 *
 *
 *	@file: nana/threads/pool.hpp
 */

#ifndef NANA_THREADS_POOL_HPP
#define NANA_THREADS_POOL_HPP

#include <nana/traits.hpp>
#include <functional>
#include <cstddef>


namespace nana{
   /// Some mutex classes for synchronizing.
namespace threads
{    /// A thread pool manages a group threads for a large number of tasks processing.
	class pool
	{
		/**
		 * \brief Task to append to the task queue
		 * Can either be a genuine task, or a signal.
		 */
		struct task
		{
			///Task type enumeration
			enum t{general, signal};

			///The task type
			const t kind;

			/**
			 * \brief Constructs a task of the given type
			 * \param type Task type, which is either 'general' or 'signal'
			 */
			task(t type);
			/**
			 * \brief Destroy the task
			 */
			virtual ~task() = 0;
			/**
			 * \brief Run the task
			 */
			virtual void run() = 0;
		};

		/**
		 * \brief Wrapper for a function as a task
		 */
		template<typename Function>
		struct task_wrapper
			: task
		{
			typedef Function function_type;
			///The task function
			function_type taskobj;

			/**
			 * \brief Create a new task
			 * \param f The function to apply
			 */
			task_wrapper(const function_type& f)
				: task(task::general), taskobj(f)
			{}

			/**
			 * \brief Run the wrapped function
			 */
			void run()
			{
				taskobj();
			}
		};

		//Task signal
		struct task_signal;
		//The local implementation
		class impl;

		//Don't allow copy construction
		pool(const pool&) = delete;
		//Don't allow copy assignment
		pool& operator=(const pool&) = delete;
	public:
		/**
		 * \brief Creates a default size thread pool.
		 * Default size is 4 threads.
		 */
		pool();

		/**
		 * \brief Move constructor
		 * \param other The other pool to initialize this pool with.
		 */
		pool(pool&& other);

		/**
		 * \brief Creates a pool with the specified number of threads
		 * \param thread_number Number of threads to instantiate
		 */
		pool(std::size_t thread_number);
		/**
		 * \brief Waits for all running tasks to complete and skips all queued tasks.
		 */
		~pool();

		/**
		 * \brief Moves implementation of the other pool to this pool.
		 * Destroys the local resources if available and connects them to the resources of the given pool
		 * \param other The pool to copy resources from
		 */
		pool& operator=(pool&& other);

		/**
		 * \brief Adds a new task to the task queue
		 * \param f Function to execute.
		 */
		template<typename Function>
		void push(const Function& f)
		{
			task * taskptr = nullptr;

			try
			{
				taskptr = new task_wrapper<typename std::conditional<std::is_function<Function>::value, Function*, Function>::type>(f);
				_m_push(taskptr);
			}
			catch(std::bad_alloc&)
			{
				//Bad alloc is thrown when the pool is addressed concurrently?
				delete taskptr;
			}
		}

		/**
		 * \brief Creates a signal that will be triggered when the tasks which are pushed before it are finished.
		 */
		void signal();
		/**
		 * \brief Waits for a signal created by signal()
		 */
		void wait_for_signal();
		/**
		 * \brief Waits for all thread tasks to complete
		 */
		void wait_for_finished();
	private:
		/**
		 * \brief Pushes the new task unto the task queue
		 * \param task_ptr Pointer to the task
		 */
		void _m_push(task* task_ptr);
	private:
		impl * impl_;
	};//end class pool

    /**
     * \brief Wrapper for pushing the same task on the specified pool multiple times
     */
	template<typename Function>
	class pool_pusher
	{
	public:
           /// same as Function if Function is not a function prototype, otherwise value_type is a pointer type of function
		typedef typename std::conditional<std::is_function<Function>::value, Function*, Function>::type value_type;

		/**
		 * \brief Creates a pusher for the given pool that can repeatedly push the specified function on the task queue.
		 * \param pobj Reference to the thread pool
		 * \param fn The function
		 */
		pool_pusher(pool& pobj, value_type fn)
			:pobj_(pobj), value_(fn)
		{}

		/**
		 * \brief Pushes the wrapped function on the queue
		 */
		void operator()() const
		{
			pobj_.push(value_);
		}
	private:
		///Local reference to pool
		pool & pobj_;
		///Function to push
		value_type value_;
	};

	template<typename Function>
	pool_pusher<Function> pool_push(pool& pobj, const Function& fn)
	{
		return pool_pusher<Function>(pobj, fn);
	}

	/**
	 * \brief Creates a pool pusher for a member function
	 * \param pobj The pool
	 * \param obj Target class object
	 * \param mf Member function pointer
	 * \returns A new pool pusher that pushes the member function, applied on the object, on the task queue
	 */
	template<typename Class, typename Concept>
	pool_pusher<std::function<void()> > pool_push(pool& pobj, Class& obj, void(Concept::*mf)())
	{
		return pool_pusher<std::function<void()> >(pobj, std::bind(mf, &obj));
	}

}//end namespace threads
}//end namespace nana
#endif

