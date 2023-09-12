#pragma once
#include "net_common.h";
#include <deque>
#include <mutex>

using namespace std;

namespace olc
{
	namespace net
	{
		template <typename T> 
		class tsqueue
		{
			public:
				tsqueue() = default;
				tsqueue(const tsqueue<T>&) = delete;
				virtual ~tsqueue() { clear(); } // distructor

			public:
				// return and maintain item at the front of queue
				const T& front()
				{

					lock_guard lock(muxQueue);
					return deqQueue.front();
				}

				// returns and maintains item at back of queue
				const T& back()
				{
					lock_guard lock(muxQueue);
					return deqQueue.back();
				}

				// add item to the back
				void push_back(const T& item)
				{
					lock_guard lock(muxQueue);
					deqQueue.emplace_back(move(item));
				}

				// add item to the front
				void push_front(const T& item)
				{
					lock_guard lock(muxQueue);
					deqQueue.emplace_front(move(item));
				}

				// empty
				bool empty()
				{
					lock_guard lock(muxQueue);
					return deqQueue.empty();
				}

				// count
				size_t count()
				{
					lock_guard lock(muxQueue);
					return deqQueue.size();
				}

				// empty it
				void clear()
				{
					lock_guard lock(muxQueue);
					deqQueue.clear();
				}

				// remove and return front item
				T pop_front()
				{
					lock_guard lock(muxQueue);
					auto t = move(deqQueue.front());
					deqQueue.pop_front();
					return t;
				}

			protected:
				mutex muxQueue;
				deque<T> deqQueue;

		};
	}
}