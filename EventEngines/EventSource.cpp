#include "EventSource.hpp"

EventSource::EventSource(const EventTypes& p_eventTypes)
  : m_eventTypes(p_eventTypes)
{
}

EventSource::~EventSource()
{
}

void EventSource::setEventTypes(const EventTypes& p_eventTypes)
{
  m_eventTypes = p_eventTypes;
}

const EventSource::EventTypes& EventSource::getEventTypes() const
{
  return m_eventTypes;
}
