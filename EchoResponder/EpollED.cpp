#include "EpollED.hpp"
#include "SocketES.hpp"

#include <stdexcept>

EpollED::EpollED(Epoll::Ptr p_epoll)
  : m_epoll(p_epoll)
{
}

EpollED::~EpollED()
{
}

void EpollED::add(EventSource::Ptr p_es)
{
  SocketES::Ptr sES = std::dynamic_pointer_cast<SocketES>(p_es);
  m_epoll->add(sES->getSocket(), p_es->getEventTypes());
}

void EpollED::modify(EventSource::Ptr p_es)
{
  SocketES::Ptr sES = std::dynamic_pointer_cast<SocketES>(p_es);
  m_epoll->modify(sES->getSocket(), p_es->getEventTypes());
}

void EpollED::remove(EventSource::Descriptor p_descriptor)
{
  m_epoll->remove(p_descriptor);
}

void EpollED::wait(Events& p_events)
{
  Epoll::Events ees;
  m_epoll->wait(ees);

  for(Epoll::Events::iterator i = ees.begin(); i != ees.end(); ++i)
  {
    EventDemultiplexer::Event e;
    e.descriptor = i->descriptor;
    e.eventTypes = i->eventTypes;
    p_events.push_back(e);
  }
}

void EpollED::interrupt()
{
  m_epoll->interrupt();
}

