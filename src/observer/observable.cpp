#include <iostream>
#include <algorithm> //find
#include "observable.hpp"

namespace observer {
	
void Observable::Notify(std::shared_ptr<event::Event> a_event)
{
	auto key = a_event->GetLog() + "_" + a_event->GetLocation();	
	auto vec = m_observableMap.getValue(key);
	for (const auto &e : vec) {
		e->GetEvent(a_event);
	}
}

void Observable::Subscribe(const std::string& a_eventID, AgentPtr a_controller)
{
	std::string k(a_eventID);
	if (m_observableMap.IsContained(a_eventID)) {
		auto vec = m_observableMap.getValue(a_eventID);
		vec.push_back(a_controller);
		m_observableMap.Update(k, vec);
	}
	else {
		std::vector<std::shared_ptr<device::AbstractAgent>> vec;
		vec.push_back(a_controller);
		m_observableMap.Insert(k, vec);
	}
}

void Observable::UnSubscribe(const std::string& a_eventID, AgentPtr a_controller)
{
	auto vec = m_observableMap.getValue(a_eventID);
	vec.erase(std::find(vec.begin(), vec.end(), a_controller));
	m_observableMap.Update(a_eventID, vec);
}

} //observer
