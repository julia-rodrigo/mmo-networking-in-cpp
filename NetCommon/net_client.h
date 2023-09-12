#pragma once

#include "net_common.h";
#include "net_tsqueue.h";
#include "net_message.h";
#include "net_connection.h";

#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;

namespace olc
{
	namespace net
	{
		template <typename T>
		class client_interface
		{
			public:
				// 
				client_interface()
				{
					// initiate
				}

				virtual ~client_interface()
				{
					// fi client is destroyed, always try and disconnect from the server
					Disconnect();
				}

			public:
				// Connect with hostname / ip-address and port
				bool Connect(const string& host, const uint16_t port)
				{
					try {

						// Resolve hostname/ip-address into tangiable physical address
						ip::tcp::resolver resolver(m_context);
						ip::tcp::resolver::results_type endpoints = resolver.resolve(host, to_string(port));

						// Create connection
						m_connection = make_unique<connection<T>>();

						// Tell the connection object to connect to server
						m_connection->ConnectToServer();

						// Start Context Thread
						thrContext = std::thread([this]() { m_context.run(); });

					}
					catch (exception& e)
					{
						cerr << "Client EXCEPTION: " << e.what() << endl;
						return false;
					}
					return false;
				}

				void Disconnect()
				{
					if (IsConnected())
					{
						m_connection->Disconnect();
					}

					m_context.stop();

					if (thrContext.joinable())
						thrContext.join;

					m_connection.release();

				}

				bool IsConnected()
				{
					if (m_connection)
						return m_connection->IsConnected();
					return false;
				}

				// get messages
				tsqueue<owned_message<T>>& Incoming()
				{
					return m_qMessagesIn;
				}

				// Send message to server
				void Send(const message<T>& msg)
				{
					if (IsConnected())
						m_connection->Send(msg);
				}

			protected:
				// asio context handles the data transfer
				io_context m_context;

				// but need a thread of its own to execute its work commands
				thread thrContext;

				// the client has a single instanc eo f a connetion opbject to handle data transfer
				unique_ptr <connection<T>> m_connection;

			private:
				//this is thread safe queue of incoming messages from the server
				tsqueue<owned_message<T>> m_qMessagesIn;


		};
	}
}