#pragma once
#include <map>

namespace utils
{
	template<typename T>
	class nullable
	{
		T value;
		T defaultVal;
		bool isNull = true;
	public:
		nullable(T defaultVal) : defaultVal(defaultVal), value(defaultVal), isNull(true){}
		void set(T value)
		{
			this->value = value;
			isNull = false;
		}
		void unset()
		{
			isNull = true;
		}
		T get()
		{
			if (isNull) return defaultVal;
			return value;
		}
	};

	/**
	 * \brief Map that caches the last result for repeated accessing.
	 */
	template<typename Key, typename Value>
	class cached_map
	{
		std::map<Key, Value> localMap;
		Key lastVisited;
		nullable<Value> lastValue;
	public:
		cached_map(Value defaultValue) : lastValue(nullable<Value>(defaultValue))
		{
		}

		// Noncopyable
		cached_map(const cached_map&) = delete;
		cached_map& operator=(const cached_map&) = delete;

		//Nonmovable
		cached_map(cached_map&&) = delete;
		cached_map& operator=(cached_map&&) = delete;

		/**
		 * \brief Insert a key value pair
		 */
		void insert(Key k, Value v)
		{
			lastVisited = k;
			lastValue.set(v);
			localMap.emplace(k, v);
		}

		void insert(Key k, Value&& v)
		{
			lastVisited = k;
			auto ret = localMap.emplace(k, std::move(v));
			lastValue.set(ret.first->second);
		}

		void erase(Key k)
		{
			localMap.erase(k);
			lastVisited = k;
			lastValue.unset();
		}

		Value find(Key k)
		{
			if(k == lastVisited)
			{
				return lastValue.get();
			}
			auto elem = localMap.find(k);

			if(elem == localMap.end())
			{
				lastValue.unset();
				lastVisited = k;
				return lastValue.get();
			}
			return elem->second;
		}

		Value& findRef(Key k )
		{
			if (k == lastVisited)
			{
				return lastValue.get();
			}
			auto elem = localMap.find(k);

			if (elem == localMap.end())
			{
				lastValue.unset();
				lastVisited = k;
				return lastValue.get();
			}
			return elem->second;
		}
	};
}