#include<iostream>
#include<boost/array.hpp>
#include<boost/asio.hpp>

#include<ctime>
#include<string>

using boost::asio::ip::udp;

std::string make_daytime_string()
{
    using namespace std;   // For time_t, time and ctime
    time_t now = time(0);
    return ctime(&now);
}

int main()
{
    try{
        boost::asio::io_service io_service;
        // create an ip::udp::socket object to receive requests on UDP port 13
        udp::socket socket(io_service, udp::endpoint(udp::v4(), 9369));

        for(;;)
        {
            boost::array<char, 1> recv_buf;
            udp::endpoint remote_endpoint;
            boost::system::error_code error;
            socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint, 0, error);

            if(error && error != boost::asio::error::message_size)
                throw boost::system::system_error(error);

            std::string message = make_daytime_string();

            boost::system::error_code ignored_error;
            socket.send_to(boost::asio::buffer(message), remote_endpoint, 0, ignored_error);
        }

    }
    catch(std::exception &e)
    {
        std::cerr<<e.what()<<std::endl;
    }
    return 0;
}