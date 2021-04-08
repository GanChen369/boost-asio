#ifndef HTTP_SERVER4_SERVER_HPP
#define HTTP_SERVER4_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include "request_parser.hpp"


namespace http {
    namespace server4 {

        struct request;
        struct reply;

        class server : boost::asio::coroutine{
        public:
            explicit server(boost::asio::io_service &io_service,
                    const std::string &address,
                    const std::string &port,
                    boost::function<void(const request&, reply&)> request_handler);

            void operator()(boost::system::error_code ec = boost::system::error_code(), std::size_t length = 0);

        private:

            typedef boost::asio::ip::tcp tcp;

            /// The user-supplied handler for all incoming requests.
            boost::function<void(const request &, reply &)> request_handler_;

            /// Acceptor used to listen for incoming connection
            boost::shared_ptr <tcp::acceptor> acceptor_;

            /// The current connection from a client
            boost::shared_ptr <tcp::socket> socket_;

            /// buffer for incoming data
            boost::shared_ptr <boost::array<char, 8192> > buffer_;

            /// The incoming request
            boost::shared_ptr <request> request_;

            /// Whether the request is valid or not.
            boost::tribool valid_request_;

            /// The parser for the incoming request
            request_parser request_parser_;

            /// The reply to be sent back to the client
            boost::shared_ptr <reply> reply_;
        };


    }


}


#endif // HTTP_SERVER4_SERVER_HPP