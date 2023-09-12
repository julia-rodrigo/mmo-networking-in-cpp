#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include <chrono>

using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace std::chrono_literals;

// control how much data read
vector <char> vBuffer(20 * 1024);

void GrabSomeData(tcp::socket& socket)
{
    socket.async_read_some(buffer(vBuffer.data(), vBuffer.size()),
        [&](error_code ec, size_t length)
        {
            if (!ec)
            {
                // display the bytes
                cout << "\n\nRead " << length << " bytes\n\n";

                // display the bytes from the buffer
                for (int i = 0; i < length; i++)
                    cout << vBuffer[i];

                // grab more data until there is no data to 
                // read and then the function will wait
                GrabSomeData(socket);
            }
        }
    );
}

int main()
{

    // connect to prebuild website! 93.184.216.34:80
    //const char* address = "127.0.0.1"; // descirptive error message
    
    const char*address = "93.184.216.34";
    unsigned short port_num = 80;
    boost::system::error_code ec;

    // create a context 
    io_context context;

    // give some fake tasks to asio so that the context doesnt finish
    io_context::work idleWord(context);

    // Start the context
    thread thrContext = thread([&]() { context.run(); });

    // construct address we want to connect to
    tcp::endpoint endpoint(make_address(address, ec), port_num);

    // create a socket to deliver the context implementation
    tcp::socket socket(context);

    // tell the socket to connect
    socket.connect(endpoint, ec);

    if (!ec)
    {
        cout << "Connected!" << endl;
    }
    else {
        cout << "Failed to connect to address:\n" << ec.message() << endl;
    }

    if (socket.is_open())
    {
        GrabSomeData(socket);

        string sRequest =
            "GET /index.html/1.1\r\n"
            "Host: example.com\r\n"
            "Connection: close\r\n\r\n";

        socket.write_some(buffer(sRequest.data(), sRequest.size()), ec);

        this_thread::sleep_for(2000ms);

        context.stop();
        if (thrContext.joinable()) thrContext.join();

    }

    system("pause");

    return 0;
}