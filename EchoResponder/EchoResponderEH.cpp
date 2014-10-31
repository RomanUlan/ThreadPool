#include "EchoResponderEH.hpp"
#include "../IO/Epoll.hpp"

#include <boost/algorithm/string.hpp>

EchoResponderEH::EchoResponderEH(MessageES::Ptr p_messageES, EventEngines::ThreadPool& p_threadPool)
  : EventHandler(p_messageES), m_threadPool(p_threadPool)
{
}

EchoResponderEH::~EchoResponderEH()
{
}

void EchoResponderEH::handle(const EventSource::EventTypes& p_eventTypes)
{
  EventSource::EventTypes::const_iterator iIn = p_eventTypes.find(Epoll::EventType::In);
  if ( (iIn != p_eventTypes.end()) && (p_eventTypes.size() == 1) )
  {
    MessageES::Ptr mES = std::dynamic_pointer_cast<MessageES>(m_eventSource);
    std::string data;
    while (!boost::ends_with(data, "\n"))
    {
      std::string part;
      mES->read(part);
      data.append(part);
    }
    mES->getSocket()->write(data);
    m_threadPool.modify(m_eventSource);
  }
  else
  {
    m_threadPool.remove(m_eventSource->getDescriptor());
  }
}
