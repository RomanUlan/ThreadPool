#include "ThreadPool.hpp"

#include <sstream>

using namespace Threading;

ThreadPool::Job::Job(Type p_type, const Task& p_task)
  : m_Type(p_type), m_Task(p_task)
{
}

ThreadPool::ThreadPool(size_t p_size)
  : m_queue(), m_size(p_size), m_threads()
{
}

ThreadPool::~ThreadPool()
{
  //TODO check if kill all threads before if needed
}

void ThreadPool::start()
{
  if (!m_threads.empty())
  {
    throw std::runtime_error("ThreadPool started already");
  }

  std::string threadCreationError;
  for (size_t i = 0; (i < m_size) && threadCreationError.empty(); ++i)
  {
    try
    {
      m_threads.push_back(threadH(new std::thread(std::bind(ThreadPool::threadFunc, std::ref(m_queue)))));
    }
    catch (const std::exception e)
    {
      threadCreationError = e.what();
      break;
    }
  }

  if (!threadCreationError.empty())
  {
    if (m_threads.size())
      stop(true);

    std::stringstream ss;
    ss << "ThreadPool thread creation error: " << threadCreationError << ", ThreadPool not started at all";
    throw std::runtime_error(ss.str());
  }
}

void ThreadPool::stop(bool p_immediately)
{
  if (p_immediately)
    for (size_t i = 0; i < m_size; ++i)
      m_queue.pushFront(Job(Job::STOP, Task()));
  else
    for (size_t i = 0; i < m_size; ++i)
      m_queue.pushBack(Job(Job::STOP, Task()));

  for (auto i = m_threads.begin(); i != m_threads.end(); ++i)
    (*i)->join();

  m_threads.clear();
}

void ThreadPool::add(const Task& p_task)
{
  m_queue.pushBack(Job(Job::REGULAR, p_task));
}

void ThreadPool::threadFunc(TwoEndQueue<Job>& p_queue)
{
  while (1)
  {
    Job job = p_queue.pop();
    if (job.m_Type == Job::REGULAR)
      job.m_Task();
    else
      break;
  }
}
