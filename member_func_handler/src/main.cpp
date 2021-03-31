#include<iostream>
#include<boost/asio.hpp>
#include<boost/bind.hpp>
#include<boost/date_time/posix_time/posix_time.hpp>

class printer{

public:
    printer(boost::asio::io_service& io): timer_(io, boost::posix_time::seconds(1)),  count_(0){
        timer_.async_wait(boost::bind(&printer::print, this));
    }

    ~printer(){
        std::cout<<"Final count is "<< count_<<std::endl;
    }

    /*
     * The boost::bind returns a function object which refers to method print for class printer for the object referenced by this.
     * This function object is called by the async_wait method with the error parameter (since that is the signature it expects).
     * But the error parameter is silently ignored because it is not referenced by the bind.
     * */
    void print(){
        if(count_ < 5){
            std::cout<<count_<<std::endl;
            ++count_;

            timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(1));
            timer_.async_wait(boost::bind(&printer::print, this));
        }

    }

private:
    boost::asio::deadline_timer timer_;
    int count_;
};

int main(){
    boost::asio::io_service io;
    printer p(io);
    io.run();

    return 0;

}