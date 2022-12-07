#include "recv_messages.hpp"
#include "tcp_client_socket.hpp"

namespace net {

std::vector<unsigned char> Recv(std::unique_ptr<net::TcpClientSocket>* a_client)
{
    std::vector<unsigned char>response(sizeof(uint64_t));
    auto responsePtr = response.data();

    unsigned long total = 0;

    while (total < sizeof(double)) {
        int readBytes;

        try {
            readBytes = a_client->get()->Receive(responsePtr ,sizeof(double) - total);
            responsePtr += readBytes;
            total += readBytes;
        }
        catch(...) {
            throw;
        }    
    }

    uint64_t *temp = reinterpret_cast<uint64_t*>(&response[0]);

    //*temp = be64toh(*temp);    

    uint64_t bytes = *temp;

    response.resize(sizeof(double) + *temp);
    
    auto res = response.data();
    res += sizeof(double);

    size_t counter = 0;

    while (counter < bytes) {
        ssize_t readBytes;
        try {
            readBytes = a_client->get()->Receive(res ,bytes - counter);
            counter+=readBytes;
            res+=readBytes;
        }
        catch(const TcpClientSocketErr& e) {           
           throw;
        }
    }
        
    return response;
}

} //net