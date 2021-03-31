#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>


using boost::asio::ip::udp;

std::string make_daytime_string()
{
    using namespace std; // For time_t, time and ctime;
    time_t now = time(0);
    return ctime(&now);
}


class udp_server
{
public:
    udp_server(boost::asio::io_service& io_service)
        :socket_(io_service, udp::endpoint(udp::v4(), 9369))
    {
        start_receive();
    }

private:
    void start_receive()
    {
        socket_.async_receive_from(
                boost::asio::buffer(recv_buffer_),
                remote_endpoint_,
                boost::bind(
                        &udp_server::handle_receive,
                        this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive(const boost::system::error_code& error, std::size_t transfer_len)
    {
        std::cout<<"handle_receive error="<<error<<" transfer_len="<<transfer_len<<std::endl;
        std::cout<<"recv_buffer_ start"<<std::endl;
        boost::array<char , 1>::iterator itr;
        for(itr=recv_buffer_.begin(); itr!=recv_buffer_.end(); itr++)
        {
            std::cout<<*itr<<"\n";
        }
        std::cout<<"recv_buffer_ end"<<std::endl;

        if(!error || error == boost::asio::error::message_size)
        {
            boost::shared_ptr<std::string> message(
                    new std::string(make_daytime_string()));

            socket_.async_send_to(
                    boost::asio::buffer(*message),
                    remote_endpoint_,
                    boost::bind(
                            &udp_server::handle_send,
                            this,
                            message,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));

            start_receive();
        }

    }

    void handle_send(boost::shared_ptr<std::string> message,
            const boost::system::error_code& error,
            std::size_t bytes_transferred)
    {
        std::cout<<"handle_send message="<<message<<" error="<<error<<" bytes_transferred="<<bytes_transferred<<std::endl;
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array<char, 1> recv_buffer_;
};

int main()
{
    try
    {
        boost::asio::io_service io_service;
        udp_server server(io_service);
        io_service.run();
    }
    catch(std::exception& e)
    {
        std::cerr<< e.what()<<std::endl;
    }
    return 0;


}