#ifndef THREADING_THREAD_POOL_HPP
#define THREADING_THREAD_POOL_HPP

#include "TwoEndQueue.hpp"

#include <thread>
#include <vector>

namespace Threading
{

class ThreadPool
{
private:
  struct Job
  {
  public:
    typedef std::function<void (void)> Task;
    enum Type {
      STOP,
      REGULAR
    };
    Job(Type, const Task&);
    Type m_Type;
    Task m_Task;

  private:
    Job();
  }; //struct Job

public:
  typedef Job::Task Task;
  explicit ThreadPool(size_t);
  ~ThreadPool();

public:
  void start();
  void stop(bool);
  void add(const Task&);

protected:
  static void threadFunc(TwoEndQueue<Job>&);

private:
  ThreadPool(const ThreadPool&);
  ThreadPool& operator=(const ThreadPool&);

private:
  typedef std::shared_ptr<std::thread> threadH;
  typedef std::vector<threadH> threads;

private:
  TwoEndQueue<Job> m_queue;
  size_t m_size;
  threads m_threads;
}; //class ThreadPool

} //namespace Threading

#endif //THREADING_THREAD_POOL_HPP
