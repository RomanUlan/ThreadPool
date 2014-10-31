#ifndef EVENT_ENGINES_THREAD_POOL_HPP
#define EVENT_ENGINES_THREAD_POOL_HPP

#include "EventHandler.hpp"
#include "EventDemultiplexer.hpp"
#include "../Threading/ThreadPool.hpp"

#include <map>

namespace EventEngines
{

class ThreadPool
{
public:
  typedef std::shared_ptr<ThreadPool> Ptr;

public:
  explicit ThreadPool(EventDemultiplexer::Ptr, size_t);
  ~ThreadPool();

  void add(EventHandler::Ptr);
  void modify(EventSource::Ptr);
  void remove(EventSource::Descriptor);
  void eventLoop();
  void interrupt(bool);

private:
  ThreadPool(const ThreadPool&);
  ThreadPool& operator=(const ThreadPool&);

private:
  typedef std::map<EventSource::Descriptor, EventHandler::Ptr> t_handlers;
  typedef std::pair<EventHandler::Ptr, EventSource::EventTypes> t_toHandle;
  typedef std::vector<t_toHandle> t_toHandles;

  std::mutex m_mutex;
  bool m_run;
  t_handlers m_handlers;
  EventDemultiplexer::Ptr m_eventDemultiplexer;
  Threading::ThreadPool m_threadPool;
}; //class ThreadPool

} //namespace EventEngines

#endif //EVENT_ENGINES_THREAD_POOL_HPP
