#include "EventHandler.hpp"

EventHandler::EventHandler (EventSource::Ptr p_eventSource)
  : m_eventSource(p_eventSource)
{
}

EventHandler::~EventHandler()
{
}

EventSource::Ptr EventHandler::getEventSource() const
{
  return m_eventSource;
}

EventSource::EventTypes EventHandler::getEventTypes() const
{
  return m_eventSource->getEventTypes();
}
