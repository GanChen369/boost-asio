#include<iostream>
#include<boost/array.hpp>
#include<boost/asio.hpp>

using boost::asio::ip::udp;

int main(int argc, char* argv[]){
    try {
        if (argc != 2) {
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }

        boost::asio::io_service io_service;

        udp::resolver resolver(io_service);

        /**
         * 我们使用ip :: udp :: resolver对象根据主机名和服务名找到要使用的正确远程端点。
         * ip :: udp :: v4（）参数将查询限制为仅返回IPv4端点。
         */
        udp::resolver::query query(udp::v4(), argv[1], "9369");

        /**
         * 如果没有失败，则ip :: udp :: resolver :: resolve（）函数可确保返回列表中的至少一个端点。
         * 这意味着直接取消引用返回值是安全的。
         */
        udp::endpoint receiver_endpoint = *resolver.resolve(query);

        udp::socket socket(io_service);
        socket.open(udp::v4());

        boost::array<char, 1>send_buf = {{0}};
        // 这个函数同步地把缓冲区数据发送到一个指定的端点。在所有数据发送成功或者出现错误之前，这个函数都是阻塞的
        // send_to(buffer, endpoint [, flags])
        socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

        boost::array<char, 128> recv_buf;
        udp::endpoint sender_endpoint;
        // 这个函数同步地从一个指定的端点获取数据并写入到给定的缓冲区。在读完所有数据或者错误出现之前，这个函数都是阻塞的。
        size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
        std::cout.write(recv_buf.data(), len);
    }
    catch(std::exception &e){
        std::cerr<<e.what()<<std::endl;
    }

    return 0;
}