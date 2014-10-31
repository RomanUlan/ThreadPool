#include "AcceptorEH.hpp"
#include "EchoResponderEH.hpp"
#include "../IO/Epoll.hpp"

#include <stdexcept>

AcceptorEH::AcceptorEH(ListenerES::Ptr p_listenerES, EventEngines::ThreadPool& p_threadPool)
  : EventHandler(p_listenerES)
  , m_threadPool(p_threadPool)
{
}

AcceptorEH::~AcceptorEH()
{
}

void AcceptorEH::handle(const EventSource::EventTypes& p_eventTypes)
{
  EventSource::EventTypes::const_iterator iIn = p_eventTypes.find(Epoll::EventType::In);
  if ( (iIn != p_eventTypes.end()) && (p_eventTypes.size() == 1) )
  {
    ListenerES::Ptr listenerES = std::dynamic_pointer_cast<ListenerES>(m_eventSource);
    MessageES::Ptr msgES = listenerES->accept();
    EchoResponderEH::Ptr erEH(new EchoResponderEH(msgES, m_threadPool));
    m_threadPool.add(erEH);
    m_threadPool.modify(m_eventSource);
  }
  else
  {
    throw std::runtime_error("Bad event for for acceptor");
  }
}
