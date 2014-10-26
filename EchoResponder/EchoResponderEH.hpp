#ifndef ECHO_RESPONDER_EH_HPP
#define ECHO_RESPONDER_EH_HPP

#include "../EventEngines/ThreadPool.hpp"
#include "MessageES.hpp"

class EchoResponderEH : public EventHandler
{
public:
  EchoResponderEH(MessageES::Ptr, EventEngines::ThreadPool&);
  virtual ~EchoResponderEH();

public:
  virtual void handle(const EventSource::EventTypes&);

private:
  EventEngines::ThreadPool& m_threadPool;
};

#endif
