#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include <boost/noncopyable.hpp>
#include "connection.hpp"
#include "connection_manager.hpp"
#include "request_handler.hpp"

namespace http
{
    namespace server
    {
        /// The top-level class of the HTTP server
        class server
                : private boost::noncopyable
        {
        public:
            /**
             * Constructor the server to listen on the specified TCP address and port,
             * and serve up files from the given directory.
             */
             explicit server(const std::string& address, const std::string& port, const std::string& doc_root);

             /// run the server's io_service loop
             void run();

        private:
            /// Initiate an asynchronous accept operation
            void start_accept();

            /// handle completion of an asynchronous accept operation
            void handle_accept(const boost::system::error_code& e);

            /// handle a request to stop the server
            void handle_stop();

            /// the io_service used to perform asynchronous operations
            boost::asio::io_service io_service_;

            /// the signal_set is used to register for process termination notifications
            boost::asio::signal_set signals_;

            /// Acceptor used to listen for incoming connections
            boost::asio::ip::tcp::acceptor acceptor_;

            /// the connection manager which owns all live connections.
            connection_manager  connection_manager_;

            /// The next connection to be accepted.
            connection_ptr new_connection_;

            /// The handler for all incoming requests.
            request_handler request_handler_;
        };
    }   //  namespace server
}   // namespace http







#endif // HTTP_SERVER_HPP