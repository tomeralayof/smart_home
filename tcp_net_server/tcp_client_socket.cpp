#include <vector>
#include <iostream>
#include <cstddef>
#include <bitset>

#include "tcp_client_socket.hpp"

namespace net {

void TcpClientSocket::Check(int readBytes, int fd)
{
    if (readBytes <= 0) {
        //printf("Oh dear, something went wrong with read()! %s\n", strerror(errno));
        throw TcpClientSocketErr("recive()","connection no longer exist",errno);
    }
}

TcpClientSocket::TcpClientSocket(const char * a_ipAddress, uint16_t a_port)
: m_socketManager()
{
    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(a_ipAddress);
    address.sin_port = htons(a_port);

    if (connect(m_socketManager.GetFD(), (struct sockaddr *)&address, sizeof(address)) == -1) {
        throw TcpClientSocketErr("connect()",
                                    "connectin error: connection failed, server is not running",errno);
    }
}

TcpClientSocket::TcpClientSocket(int a_fd)
: m_socketManager(a_fd)
{
}

void TcpClientSocket::Send(std::vector<unsigned char>& a_container)
{
    size_t numOfBytes = a_container.size();
    size_t counter = 0;

    
    int sendBytes = send(m_socketManager.GetFD(),a_container.data(),
        a_container.size() * sizeof(std::vector<uint8_t>::value_type),0);
    Check(sendBytes,m_socketManager.GetFD());

    counter += sendBytes;
    
    while (counter < numOfBytes) {
        sendBytes = send(m_socketManager.GetFD(),a_container.data(), a_container.size() - counter  ,0);
        Check(sendBytes,m_socketManager.GetFD());
        counter+=sendBytes;
    }
}

ssize_t TcpClientSocket::Receive(unsigned char *a_containerPtr,int a_bytesToRead)
{
    ssize_t readBytes = recv(m_socketManager.GetFD(), a_containerPtr, a_bytesToRead ,0);
    Check(readBytes,m_socketManager.GetFD());
    return readBytes;
}

} //net