#ifndef ACCEPTOR_EH_HPP
#define ACCEPTOR_EH_HPP

#include "../EventEngines/ThreadPool.hpp"
#include "ListenerES.hpp"

class AcceptorEH : public EventHandler
{
public:
  AcceptorEH(ListenerES::Ptr, EventEngines::ThreadPool&);
  virtual ~AcceptorEH();

public:
  virtual void handle(const EventSource::EventTypes&);

private:
  EventEngines::ThreadPool& m_threadPool;
};

#endif
