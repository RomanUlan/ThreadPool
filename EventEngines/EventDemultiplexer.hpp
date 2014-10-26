#ifndef EVENT_DEMULTIPLEXER_HPP
#define EVENT_DEMULTIPLEXER_HPP

#include "EventSource.hpp"
#include <vector>

class EventDemultiplexer
{
public:
  typedef std::shared_ptr<EventDemultiplexer> Ptr;
  struct Event
  {
    EventSource::Descriptor descriptor;
    EventSource::EventTypes eventTypes;
  };
  typedef std::vector<Event> Events;

public:
  virtual ~EventDemultiplexer();

  virtual void add(EventSource::Ptr) = 0;
  virtual void modify(EventSource::Ptr) = 0;
  virtual void remove(EventSource::Descriptor) = 0;
  virtual void wait(Events&) = 0;
  virtual void interrupt() = 0;

protected:
  EventDemultiplexer();
  EventDemultiplexer(const EventDemultiplexer&);
  EventDemultiplexer& operator=(const EventDemultiplexer&);
};

#endif
