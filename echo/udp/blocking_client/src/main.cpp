#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

enum { max_length = 1024 };

int main(int argc, char* argv[])
{
    try
    {
        if(argc != 3)
        {
            std::cerr<<"Usage: blocking_udp_echo_server <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;
        udp::socket s(io_service, udp::endpoint(udp::v4(), 0));

        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), argv[1], argv[2]);
        udp::resolver::iterator iterator = resolver.resolve(query);

        using namespace std;
        std::cout<<"Enter message: ";
        char request[max_length];
        std::cin.getline(request, max_length);
        size_t request_length = strlen(request);
        s.send_to(boost::asio::buffer(request, request_length), *iterator);

        char reply[max_length];
        udp::endpoint sender_endpoint;
        size_t reply_length = s.receive_from(boost::asio::buffer(reply, max_length), sender_endpoint);
        std::cout<<"Reply is: ";
        std::cout.write(reply, reply_length);
        std::cout<<"\n";
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}