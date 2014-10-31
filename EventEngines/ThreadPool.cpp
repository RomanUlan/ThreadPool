#include "ThreadPool.hpp"
#include <stdexcept>

using namespace EventEngines;

ThreadPool::ThreadPool(EventDemultiplexer::Ptr p_eventDemultiplexer, size_t p_tpSize)
  : m_run(false), m_eventDemultiplexer(p_eventDemultiplexer), m_threadPool(p_tpSize)
{
  m_threadPool.start();
}

ThreadPool::~ThreadPool()
{
  m_threadPool.stop(true);
}

void ThreadPool::add(EventHandler::Ptr p_eventHandler)
{
  std::unique_lock<std::mutex> lock(m_mutex);
  m_eventDemultiplexer->add(p_eventHandler->getEventSource());
  m_handlers.insert(std::make_pair(p_eventHandler->getEventSource()->getDescriptor(), p_eventHandler));
}

void ThreadPool::modify(EventSource::Ptr p_eventSource)
{
  std::unique_lock<std::mutex> lock(m_mutex);
  m_eventDemultiplexer->modify(p_eventSource);
}

void ThreadPool::remove(EventSource::Descriptor p_descriptor)
{
  std::unique_lock<std::mutex> lock(m_mutex);
  t_handlers::iterator i = m_handlers.find(p_descriptor);
  if (i != m_handlers.end())
  {
    m_handlers.erase(i);
    m_eventDemultiplexer->remove(p_descriptor);
  }
}

void ThreadPool::eventLoop()
{
  m_run = true;
  while (m_run)
  {
    EventDemultiplexer::Events events;
    m_eventDemultiplexer->wait(events);

    t_toHandles toHandles;
    {//scope of m_mutex lock begin
      std::unique_lock<std::mutex> lock(m_mutex);
      for (auto i = events.begin(); i < events.end(); ++i)
      {
        auto ih = m_handlers.find(i->descriptor);
        if (ih != m_handlers.end())
        {
          toHandles.push_back(t_toHandle(ih->second, i->eventTypes));
        }
        else
        {
          throw std::runtime_error("EventDemultiplexer returned event for \
												unfounded handler");
        }
      }
    }//scope of m_mutex lock end

    for (auto i = toHandles.begin(); i != toHandles.end(); ++i)
    {
      t_toHandle th = *i;
      Threading::ThreadPool::Task t = [th](void)->void { th.first->handle(th.second); };
      m_threadPool.add(t);
    }
  } //while (1)
}

void ThreadPool::interrupt(bool p_immediately)
{
  m_run = false;
  m_eventDemultiplexer->interrupt();
}
