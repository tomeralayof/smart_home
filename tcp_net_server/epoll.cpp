#include <fcntl.h>
#include <fstream>
#include <ostream>

#include "epoll.hpp"

int global = 1;

namespace net {

Epoll::Epoll(NetServer a_serverSocket, AppHandler a_handler, AcceptorHandler a_acceptor)
:   EventDetector(a_serverSocket, a_handler ,a_acceptor),
    m_ev(),
    m_events(),
    m_fd(epoll_create1(0))
{
    if (m_fd == -1) {
        throw EpollExeption("Epoll()","fail to create async server");
    }

    m_ev.events = EPOLLIN;
    m_ev.data.fd = m_serverSocket->GetFD();
    if (epoll_ctl(m_fd, EPOLL_CTL_ADD, m_serverSocket->GetFD() , &m_ev) == -1) {
        throw EpollExeption("Epoll()","epoll control fail");
    }
    
    m_serverSocket->Listen();
}

Epoll::~Epoll()
{
    close(m_fd);
}

void Epoll::Run()
{
    while (true) {
        int nfds = epoll_wait(m_fd, m_events, 100, -1);
        if (nfds == -1) {
            throw EpollExeption("Run()","fail to wait for epoll");
        }

        HandleIncomingEvents(nfds);
    }
}

bool epollEventErr(uint32_t a_event)
{
    return a_event & EPOLLERR || a_event & EPOLLHUP || !(a_event & EPOLLIN);
}

void Epoll::DeleteConnection(int a_fd)
{
    epoll_ctl(m_fd, EPOLL_CTL_DEL, a_fd, NULL);
	close(a_fd);
    m_clients.erase(a_fd);
}

void Epoll::AcceptConnections()
{
    Clientptr clientSocket;
    try {
        clientSocket = m_serverSocket->Accept();
    }
    catch(TcpClientSocketErr& e) {
        std::cerr << e.what() << "\n";
        return;
    }

    int clientFd = clientSocket->GetFD();

    if (!m_acceptor->DoAccept(&clientSocket)) {
        close(clientFd);
        return;
    }

    m_ev.data.fd = clientFd;
    m_ev.events = EPOLLIN;
    if (epoll_ctl(m_fd, EPOLL_CTL_ADD, clientFd, &m_ev) == -1) {
        std::cerr << "[E] epoll_ctl failed\n";
        close(clientFd);
        return;
    }

    m_clients[clientFd] = std::move(clientSocket);
}

void Epoll::HandleIncomingEvents(int a_nfds)
{
    for (int i = 0; i < a_nfds; i++) {
        if(epollEventErr(m_events[i].events)) {
            printf("[+] connection closed\n");
            DeleteConnection(m_events[i].events);
        }
        
        else if (m_serverSocket->GetFD() == m_events[i].data.fd) {
            AcceptConnections();
        }
        else {
            try {
                m_handler->HandlerIncoming(&m_clients[m_events[i].data.fd]);                
            }
            catch(const TcpClientSocketErr& e) {
                DeleteConnection(m_events[i].data.fd);
            }
        }
    }
}

} // net