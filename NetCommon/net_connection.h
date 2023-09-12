#pragma once

#include "net_common.h";
#include "net_tsqueue.h";
#include "net_message.h";

#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;

namespace olc
{
	namespace net
	{
		template <typename T>
		class connection : public  enable_shared_from_this<connection <T>>
		{
		public:
			connection()
			{}

			virtual ~connection()
			{}

		public:
			bool ConnectToServer();
			bool Disconnect();
			bool IsConnect();

		public:
			bool Send(const message <T>& msg);

		protected:
			// each connection has a socket
			ip::tcp::socket m_socket;

			// this context is shared with the asio instance
			io_context& m_asioContext;

			// thread safe queue
			tsqueue<message<T>>& m_qMessagesOut;
			tsqueue<owned_message<T>>& m_qMessagesIn;
		};
	}
}