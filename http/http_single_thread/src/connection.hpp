#ifndef HTTP_CONNECTION_HPP
#define HTTP_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "reply.hpp"
#include "request.hpp"
#include "request_handler.hpp"
#include "request_parser.hpp"

namespace http
{
    namespace server
    {
        class connection_manager;


        class connection :public boost::enable_shared_from_this<connection>, private boost::noncopyable
        {
        public:
            /// construct a connection with the given io_service
            explicit connection(boost::asio::io_service& io_service, connection_manager& manger, request_handler& handler);

            /// Get the socket associated with the connection.
            boost::asio::ip::tcp::socket& socket();

            /// get the socket associated with the connection
            void start();

            /// stop all asyncronous operations associated with the connection.
            void stop();

        private:
            /// handle completion of a read operation
            void handle_read(const boost::system::error_code& e, std::size_t bytes_transferred);

            /// handle completion of a write operation
            void handle_write(const boost::system::error_code& e);

            /// Socket for the connection.
            boost::asio::ip::tcp::socket socket_;

            /// The manager for this connection.
            connection_manager& connection_manager_;

            /// The handler used to process the incoming request.
            request_handler& request_handler_;

            /// Buffer for incoming data.
            boost::array<char, 8192> buffer_;

            /// The incoming request.
            request request_;

            /// The parser for the incoming request.
            request_parser request_parser_;

            /// The reply to be sent back to the client.
            reply reply_;
        };

        typedef boost::shared_ptr<connection> connection_ptr;
    }
}

#endif // HTTP_CONNECTION_HPP