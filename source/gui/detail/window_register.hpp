#ifndef NANA_WINDOW_REGISTER_HEADER_INCLUDED
#define NANA_WINDOW_REGISTER_HEADER_INCLUDED

#include <nana/gui/windows/basic_window.hpp>
#include <set>
#include <vector>
#include <algorithm> //std::find

namespace nana
{
	namespace detail
	{
		/**
		 * \brief Cache for key-value pairs. 
		 */
		template<typename Key, typename Value, std::size_t CacheSize>
		class cache
			: noncopyable
		{
		public:
			typedef std::pair<Key, Value> pair_type;
			typedef std::size_t size_type;

			cache()
				:addr_(reinterpret_cast<pair_type*>(::operator new(sizeof(pair_type) * CacheSize)))
			{
				for (std::size_t i = 0; i < CacheSize; ++i)
				{
					bitmap_[i] = 0;
					seq_[i] = nana::npos;
				}
			}

			~cache()
			{
				for (std::size_t i = 0; i < CacheSize; ++i)
				{
					if (bitmap_[i])
						addr_[i].~pair_type(); //Destruct element
				}

				::operator delete(addr_);
			}

			bool insert(Key k, Value v)
			{
				size_type pos = _m_find_key(k);
				if (pos != nana::npos)
				{
					addr_[pos].second = v;
				}
				else
				{
					//No key exists
					pos = _m_find_pos();

					if (pos == nana::npos)
					{	//No room, and remove the last pair
						pos = seq_[CacheSize - 1];
						(addr_ + pos)->~pair_type();
					}

					if (seq_[0] != nana::npos)
					{//Need to move
						for (int i = CacheSize - 1; i > 0; --i)
							seq_[i] = seq_[i - 1];
					}

					seq_[0] = pos;

					new (addr_ + pos) pair_type(k, v);
					bitmap_[pos] = 1;
				}
				return v;
			}

			Value * get(Key k)
			{
				size_type pos = _m_find_key(k);
				if (pos != nana::npos)
					return &(addr_[pos].second);
				return 0;
			}
		private:
			size_type _m_find_key(Key k) const
			{
				for (std::size_t i = 0; i < CacheSize; ++i)
				{
					if (bitmap_[i] && (addr_[i].first == k))
						return i;
				}
				return nana::npos;
			}

			size_type _m_find_pos() const
			{
				for (std::size_t i = 0; i < CacheSize; ++i)
				{
					if (bitmap_[i] == 0)
						return i;
				}
				return nana::npos;
			}
		private:
			char bitmap_[CacheSize];
			size_type seq_[CacheSize];
			pair_type * addr_;
		};

		/**
		 * \brief Container for windows
		 */
		class window_register
		{
		public:
			using window_handle_type = basic_window*;

			~window_register()
			{
				//Deleting a basic_window if thread never called exec(), the basic_window object
				//will always stay in trash.
				//
				//Empty the trash before destructs window register
				delete_trash(0);
			}

			/**
			 * \brief Inserts a new window handle in this container.
			 * 
			 * \param wd The window handle
			 */
			void insert(window_handle_type wd)
			{
				if (wd)
				{
					base_.insert(wd);
					wdcache_.insert(wd, true);

					if (category::flags::root == wd->other.category)
						queue_.push_back(wd);
				}
			}

			/**
			 * \brief Convenience overload for window_register::remove
			 * \param wd The window handle to remove
			 */
			void operator()(window_handle_type wd)
			{
				remove(wd);
			}

			/**
			 * \brief Removes the given window handle from the register.
			 * \param wd The window handle
			 */
			void remove(window_handle_type wd)
			{
				if (base_.erase(wd))
				{
					wdcache_.insert(wd, false);
					trash_.push_back(wd);

					if (category::flags::root == wd->other.category)
					{
						auto i = std::find(queue_.begin(), queue_.end(), wd);
						if (i != queue_.end())
							queue_.erase(i);
					}
				}
			}

			/**
			 * \brief Deletes all thrashed window handles for the given thread ID.
			 * Removes all thrash if the thread id is zero.
			 * \param thread_id The thread id.
			 */
			void delete_trash(unsigned thread_id)
			{
				if (0 == thread_id)
				{
					for (auto wd : trash_)
						delete wd;

					trash_.clear();
				}
				else
				{
					for (auto i = trash_.begin(); i != trash_.end();)
					{
						if (thread_id == (*i)->thread_id)
						{
							delete (*i);
							i = trash_.erase(i);
						}
						else
							++i;
					}
				}
			}

			/**
			 * \brief Returns the queue of window handles
			 * The queue contains handles with root category.
			 * \returns Collection of window handles.
			 */
			const std::vector<window_handle_type>& queue() const
			{
				return queue_;
			}

			/**
			 * \brief Returns the number of registered window handles
			 * \returns The number of handles
			 */
			std::size_t size() const
			{
				return base_.size();
			}

			/**
			 * \brief Returns whether the given handle is available
			 * Checks 
			 * \param wd The window handle
			 * \returns Whether the given handle is available
			 */
			bool available(window_handle_type wd) const
			{
				if (nullptr == wd)
					return false;

				auto exists = wdcache_.get(wd);
				if (exists)
					return *exists;

				return wdcache_.insert(wd, (base_.count(wd) != 0));
			}
		private:
			///Cache for windows
			mutable cache<window_handle_type, bool, 5> wdcache_;
			///Set of all window handles
			std::set<window_handle_type> base_;
			///Thrash handles
			std::vector<window_handle_type> trash_;
			///Queue of window with root category
			std::vector<window_handle_type> queue_;
		};
	}
}

#endif
