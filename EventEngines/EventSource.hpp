#ifndef EVENT_SOURCE_HPP
#define EVENT_SOURCE_HPP

#include <set>
#include <memory>

class EventSource
{
public:
  typedef std::shared_ptr<EventSource> Ptr;
  typedef int Descriptor;
  typedef int EventType;
  typedef std::set<EventType> EventTypes;

public:
  virtual ~EventSource();

  virtual Descriptor getDescriptor() const = 0;
  void setEventTypes(const EventTypes&);
  const EventTypes& getEventTypes() const;

protected:
  explicit EventSource(const EventTypes&);
  EventSource(const EventSource&);
  EventSource& operator=(const EventSource&);

protected:
  EventTypes m_eventTypes;
}; //class EventSource

#endif
