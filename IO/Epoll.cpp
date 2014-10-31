#include "Epoll.hpp"

#include <stdexcept>
#include <sys/epoll.h>
#include <string.h>
#include <fcntl.h>
#include <boost/scoped_array.hpp>

namespace
{
uint32_t nativeEvents(const Epoll::EventTypes& p_ets)
{
  uint32_t result = 0;
  for (Epoll::EventTypes::const_iterator i = p_ets.begin(); i != p_ets.end(); ++i)
  {
    if (*i == Epoll::EventType::Error)
      result |= EPOLLERR;
    else if (*i == Epoll::EventType::In)
      result |= EPOLLIN;
    else if (*i == Epoll::EventType::Out)
      result |= EPOLLOUT;
    else if (*i == Epoll::EventType::Hup)
      result |= EPOLLHUP;
    else if (*i == Epoll::EventType::RdHup)
      result |= EPOLLRDHUP;
    else if (*i == Epoll::EventType::EdgeTriggered)
      result |= EPOLLET;
    else if (*i == Epoll::EventType::OneShot)
      result |= EPOLLONESHOT;
    else
      throw std::runtime_error("Unknown event");
  }

  return result;
}

void nativeEvents(uint32_t p_nativeEvents, Epoll::EventTypes& p_ets)
{
  if (p_nativeEvents & EPOLLERR)
    p_ets.insert(Epoll::EventType::Error);

  if (p_nativeEvents & EPOLLIN)
    p_ets.insert(Epoll::EventType::In);

  if (p_nativeEvents & EPOLLOUT)
    p_ets.insert(Epoll::EventType::Out);

  if (p_nativeEvents & EPOLLHUP)
    p_ets.insert(Epoll::EventType::Hup);

  if (p_nativeEvents & EPOLLRDHUP)
    p_ets.insert(Epoll::EventType::RdHup);

  if (p_nativeEvents & EPOLLET)
    p_ets.insert(Epoll::EventType::EdgeTriggered);

  if (p_nativeEvents & EPOLLONESHOT)
    p_ets.insert(Epoll::EventType::OneShot);
}
}

int Epoll::EventType::Error = 0;
int Epoll::EventType::In = 1;
int Epoll::EventType::Out = 2;
int Epoll::EventType::Hup = 3;
int Epoll::EventType::RdHup = 4;
int Epoll::EventType::EdgeTriggered = 5;
int Epoll::EventType::OneShot = 6;

Epoll::Event::Event(const EventTypes& p_ets, Socket::Descriptor p_fd)
  : eventTypes(p_ets), descriptor(p_fd)
{
}

Epoll::Epoll(int p_size)
  : m_size(p_size), m_epollFd(::epoll_create(m_size))
{
  if (m_epollFd < 0)
  {
    throw std::runtime_error("Epoll create failed");
  }

  int pipeResult = pipe(m_selfStopPipes);
  if (pipeResult < 0)
  {
    ::close(m_epollFd);
    throw std::runtime_error("Pipe create failed");
  }

  int readPipeFlags = fcntl(m_selfStopPipes[0], F_GETFL, 0);
  fcntl(m_selfStopPipes[1], F_SETFL, readPipeFlags | O_NONBLOCK);
  EventTypes eventTypesReadPipe;
  eventTypesReadPipe.insert(Epoll::EventType::In);
  try
  {
    epollCtl(EPOLL_CTL_ADD, m_selfStopPipes[0], eventTypesReadPipe);
  }
  catch (const std::runtime_error& e)
  {
    ::close(m_selfStopPipes[0]);
    ::close(m_selfStopPipes[1]);
    ::close(m_epollFd);
    throw e;
  }
}

Epoll::~Epoll()
{
  ::close(m_epollFd);
}

void Epoll::add(Socket::Ptr p_socket, const EventTypes& p_ets) const
{
  int option = EPOLL_CTL_ADD;
  epollCtl(option, p_socket->getDescriptor(), p_ets);
}

void Epoll::modify(Socket::Ptr p_socket, const EventTypes& p_ets) const
{
  int option = EPOLL_CTL_MOD;
  epollCtl(option, p_socket->getDescriptor(), p_ets);
}

void Epoll::remove(Socket::Descriptor p_fd) const
{
  if (::epoll_ctl(m_epollFd, EPOLL_CTL_DEL, p_fd, 0) < 0)
  {
    throw ::std::runtime_error("Cannot remove fd from epoll");
  }
}

void Epoll::wait(Events& p_events) const
{
  boost::scoped_array<epoll_event> events(new epoll_event[m_size]);
  ::memset(events.get(), 0, m_size * sizeof (epoll_event));

  int eventsLen = ::epoll_wait(m_epollFd, events.get(), m_size, -1);
  if (eventsLen < 0)
  {
    throw ::std::runtime_error("Epoll wait failed");
  }

  for (int i = 0; i < eventsLen; ++i)
  {
    if (events[i].data.fd == m_selfStopPipes[0])
    {
      p_events.clear();
      break;
    }
    else
    {
      EventTypes ets;
      nativeEvents(events[i].events, ets);
      p_events.push_back(Event(ets, events[i].data.fd));
    }
  }
}

void Epoll::interrupt() const
{
  char c = 'x';
  if (write(m_selfStopPipes[1], &c, 1) < 1)
  {
    throw std::runtime_error("Interrupt failed since write to pipe failed");
  }
}

void Epoll::epollCtl(int p_option, int p_fd, const EventTypes& p_ets) const
{
  epoll_event e;
  ::memset(&e, 0, sizeof(e));
  e.data.fd = p_fd;
  e.events = nativeEvents(p_ets);

  if (::epoll_ctl(m_epollFd, p_option, p_fd, &e) < 0)
  {
    throw std::runtime_error("Epoll ctl failed");
  }
}
