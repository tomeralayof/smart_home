#ifndef EPOLL_HPP
#define EPOLL_HPP

#include <sys/epoll.h>
#include "event_detector.hpp"
#include "abstract_exeption.hpp"
#include <memory>

namespace net {

class Epoll : public EventDetector {
public:
    Epoll(NetServer a_serverSocket, AppHandler a_handler,
                    AcceptorHandler a_acceptor);
    ~Epoll();
    void Run() override;

private:
    void HandleIncomingEvents(int a_nfds); //remove file after tests.
    void DeleteConnection(int a_fd);
    void AcceptConnections();

private:
    struct epoll_event m_ev;
    struct epoll_event m_events[100];
    int m_fd;
};

class EpollExeption : public SMError {
public:
    EpollExeption(const char* fn, const char* w) : SMError(fn , w){};
};

} //net

#endif //EPOLL_HPP