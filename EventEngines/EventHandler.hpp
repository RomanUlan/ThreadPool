#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include "EventSource.hpp"

class EventHandler
{
public:
  typedef std::shared_ptr<EventHandler> Ptr;

public:
  virtual ~EventHandler();

public:
  virtual void handle(const EventSource::EventTypes&) = 0;
  EventSource::Ptr getEventSource() const;
  EventSource::EventTypes getEventTypes() const;

protected:
  explicit EventHandler(EventSource::Ptr);

private:
  EventHandler(const EventHandler&);
  EventHandler& operator=(const EventHandler&);

protected:
  EventSource::Ptr m_eventSource;
};//class EventHandler

#endif
