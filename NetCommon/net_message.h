#pragma once
#include "net_common.h"

using namespace std;

namespace olc
{
	namespace net
	{
		template <typename T> 
		struct message_header
		{
			T id{};
			uint32_t size = 0;
		};

		template <typename T>
		struct message
		{
			message_header <T> header{};
			vector<uint8_t> body;

			size_t size() const
			{
				return sizeof(message_header<T>) + body.size();
			}

			// Override for cout compatibility
			friend ostream& operator << (ostream& os, const message <T>& msg)
			{
				os << "ID: " << int(msg.header.id) << " Size: " << msg.header.size;
				return os;
			}

			// push any data into message buffer 
			template <typename DataType>
			friend message <T>& operator << (message<T>& msg, const DataType& data)
			{
				// check taht the type of the data neing pushed is copyable
				static_assert(is_standard_layout<DataType>::value, "Data is too complex to be pushed to vector");

				// cashe current size of the vetor
				size_t i = msg.body.size();

				// resize the vector by the size of the ddata being pushed
				msg.body.resize(msg.body.size() + sizeof(DataType));

				// [ysically copy the data to new vector space
				memcpy(msg.body.data() + i, &data, sizeof(DataType));

				// recalculate the message size
				msg.header.size = msg.size();

				// return the target message so it can be chained
				return msg;
			}

			template <typename DataType> 
			friend message<T>& operator >> (message<T>& msg, DataType& data)
			{
				// check taht the type of the data neing pushed is copyable
				static_assert(is_standard_layout<DataType>::value, "Data is too complex to be pushed to vector");

				// cashe current size of the vetor
				size_t i = msg.body.size();

				// resize the vector by the size of the ddata being pushed
				msg.body.resize(msg.body.size() + sizeof(DataType));

				// [ysically copy the data to new vector space
				memcpy(msg.body.data() + i, &data, sizeof(DataType));

				//Shrink the vector to remove read bytes, reset end position 
				msg.body.resize(i);

				// recalculate the message size
				msg.header.size = msg.size();

				// return the target message so it can be chained
				return msg;
			}
		};

		// forward declare the connection
		template <typename T>
		class connection;

		template <typename T>
		struct owned_message
		{
			shared_ptr<connection <T>> remote = nullptr;
			message<T> msg;

			friend ostream& operator<<(ostream& os, const owned_message<T>& msg)
			{
				os << msg.msg;
				return os;
			}
		};
	}
}