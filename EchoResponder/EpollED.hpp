#ifndef EPOLL_REACTOR_HPP
#define EPOLL_REACTOR_HPP

#include "../EventEngines/EventDemultiplexer.hpp"
#include "../IO/Epoll.hpp"

class EpollED: public EventDemultiplexer
{
public:
  explicit EpollED(Epoll::Ptr);
  ~EpollED();

public:
  virtual void add(EventSource::Ptr);
  virtual void modify(EventSource::Ptr);
  virtual void remove(EventSource::Descriptor);
  virtual void wait(Events&);
  virtual void interrupt();

private:
  Epoll::Ptr m_epoll;
};

#endif //EPOLL_REACTOR_HPP
