#include <memory>

#include "app_server.hpp"
#include "config_streams.hpp"
#include "agent_creator_factory.hpp"

int main()
{
    std::string serverConfigPath{"server.config"};
    std::string sensorAgentsConfig{"sensor_agents.ini"};    
    
    std::ifstream serverConfig(serverConfigPath);

    auto agentsFactory = std::make_shared<agentsFactory::AgentsFactory>(sensorAgentsConfig);
    server::AppServer server{serverConfig, agentsFactory};

    server.Run();
    
    return 0;
}