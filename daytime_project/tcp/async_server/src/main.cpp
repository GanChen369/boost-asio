#include<ctime>
#include<iostream>
#include<string>
#include<boost/asio.hpp>
#include<boost/bind.hpp>
#include<boost/enable_shared_from_this.hpp>
#include<boost/shared_ptr.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
    using namespace std;   // For time_t, time and ctime
    time_t now = time(0);
    return ctime(&now);
}

/*
 * 当类A被share_ptr管理，且在类A的成员函数里需要把当前类对象作为参数传给其他函数时，就需要传递一个指向自身的share_ptr。
 *
 * 因为在异步调用中，存在一个保活机制，异步函数执行的时间点我们是无法确定的，然而异步函数可能会使用到异步调用之前就存在的变量。
 * 为了保证该变量在异步函数执期间一直有效，我们可以传递一个指向自身的share_ptr给异步函数，
 * 这样在异步函数执行期间share_ptr所管理的对象就不会析构，所使用的变量也会一直有效了（保活）
 * */

class tcp_connection:
        public boost::enable_shared_from_this<tcp_connection>
{
public:
    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_service& io_service)
    {
        return pointer(new tcp_connection(io_service));
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        message_ = make_daytime_string();

        /*
         * 这个方法异步地向一个流写入数据。结束时其处理方法被调用。处理方法的格式是
         * void handler(const boost::system::error_ code & err, size_t bytes)
         * */
        boost::asio::async_write(
                socket_,
                boost::asio::buffer(message_),
                boost::bind(&tcp_connection::handle_write, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

    }
private:

    tcp_connection(boost::asio::io_service& io_service)
        :socket_(io_service)
    {

    }

    /*
     * 如果执行内容不需要下面2个参数，可以忽略
     * void handle_write()
     *
     * */

    void handle_write(const boost::system::error_code& error /*error*/, size_t  len /* bytes_transferred*/)
    {
        std::cout<<"connection handle_write error_code="<<error<<" bytes_transferred="<<len<<std::endl;
    }

    tcp::socket socket_;
    std::string message_;

};

class tcp_server{
public:
    tcp_server(boost::asio::io_service &io_service)
        :acceptor_(io_service, tcp::endpoint(tcp::v4(), 9369))
    {
        start_accept();
    }

private:

    tcp::acceptor acceptor_;

    void start_accept()
    {
        tcp_connection::pointer new_connection = tcp_connection::create(acceptor_.get_io_service());
        acceptor_.async_accept(new_connection->socket(), boost::bind(&tcp_server::handle_accept, this, new_connection, boost::asio::placeholders::error));
    }

    void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error)
    {
        if(!error)
        {
            new_connection->start();
        }
        start_accept();
    }

};

int main(){

    try{
        boost::asio::io_service io_service;
        tcp_server server(io_service);

        io_service.run();
    }
    catch(std::exception &e)
    {
        std::cerr<< e.what()<<std::endl;
    }
    return 0;

}