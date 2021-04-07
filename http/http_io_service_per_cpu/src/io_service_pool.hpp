#ifndef HTTP_SERVER2_IO_SERVICE_POOL_HPP
#define HTTP_SERVER2_IO_SERVICE_POOL_HPP

#include <boost/asio.hpp>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace http
{
    namespace server2
    {
        /// a pool of io_service objects
        class io_service_pool
                :private boost::noncopyable
        {
        public:
            explicit io_service_pool(std::size_t pool_size);

            void run();

            void stop();

            boost::asio::io_service& get_io_service();

        private:
            typedef boost::shared_ptr<boost::asio::io_service> io_service_ptr;
            typedef boost::shared_ptr<boost::asio::io_service::work> work_ptr;

            /// The pool of io_services.
            std::vector<io_service_ptr> io_services_;

            /// The work that keeps the io_services running.
            std::vector<work_ptr> work_;

            /// The next io_service to use for a connection.
            std::size_t next_io_service_;
        };
    }

}

#endif // HTTP_SERVER2_IO_SERVICE_POOL_HPP