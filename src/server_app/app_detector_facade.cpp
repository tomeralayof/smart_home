#include <map>
#include <memory>

#include "app_detector_facade.hpp"
#include "tcp_server_socket.hpp"
#include "iacceptor.hpp"
#include "thread_pool.hpp"
#include "event_handler.hpp"
#include "app_handler.hpp"
#include "epoll.hpp"
#include "server_config_parser.hpp"
#include "agent_creator_factory.hpp"
#include "observable.hpp"
#include "priority_queue_adapter.hpp"
#include "app_db.hpp"
#include "http_client.hpp"
#include "connect_agents.hpp"

namespace server {

AppDetectorFacade::AppDetectorFacade(std::ifstream& a_serverConfig, AgentsFactoryPtr a_agentsFactory)
: m_serverConfig(a_serverConfig)
, m_agentsFactory(a_agentsFactory)
{
}

class DefaultAcceptor : public net::Iacceptor {
    bool DoAccept(std::unique_ptr<net::TcpClientSocket>* a_client)
    {
        return true;
    }
};

net::EventDetector* AppDetectorFacade::GetAppHandler() {
    //data base
    device::AppDB appTable;
    appTable.MakeDeviceTable();

    //setup server configuration
    config::ServerConfigParser serverParser{m_serverConfig};
    uint16_t port = serverParser.Port();
    unsigned int queueSize = serverParser.QueueSize();
    unsigned int numOfThreads = serverParser.ThreadsNum();
    std::string uri = serverParser.Uri();
    std::string contentType = serverParser.ContentType();

    //setup listener server
    auto netServer = std::make_shared<net::TcpServerSocket>(port);

    //setup acceptor
    auto acceptor = std::make_unique<DefaultAcceptor>();

    //setup event handler classes
    auto queueAdapter = std::make_shared<ds::priorityQueueAdapter>(queueSize);
    auto threadPoolPtr = std::make_shared<threads::ThreadPool>(numOfThreads,queueAdapter);
    auto observablePtr = std::make_shared<observer::Observable>();

    //create event handler
    auto httpClientPtr = std::make_shared<http::HttpCLient>(uri,contentType);
    auto eventHandler = std::make_shared<event::EventsHandler>
                                (threadPoolPtr,observablePtr,httpClientPtr);
    //create agents
    std::shared_ptr<observer::AbstractSubscribers> subscriber = observablePtr;

    //fix seg fault    
    auto agentsMap = m_agentsFactory->GenerateAgentsMap(appTable);
    ConnectAgents(agentsMap, subscriber, eventHandler);
    
    //create app handler
    auto appHandler = std::make_shared<net::AppHandler>(agentsMap);

    //create detector
    net::EventDetector* detector = new net::Epoll(netServer, appHandler, std::move(acceptor));

    return detector;
}

} //server