#include "connect_agents.hpp"

namespace server {

void ConnectAgents(AgentsMapPtr a_agentsMap, AbstractSubscriberPtr a_subscriber, EventHandlerPtr a_eventHandler) 
{
    auto agents = a_agentsMap.get();
    auto begin = agents->begin();
    auto end = agents->end();

    while(begin != end)  
    {
        a_agentsMap->at(begin->first)->Connect(a_eventHandler, a_subscriber);
        ++begin;
    }
}

} //server