#ifndef TWO_END_QUEUE_HPP
#define TWO_END_QUEUE_HPP

#include <list>
#include <mutex>
#include <condition_variable>

namespace Threading
{

template<class T>
class TwoEndQueue
{
public:
  TwoEndQueue();
  ~TwoEndQueue();

public:
  void pushFront(const T&);
  void pushBack(const T&);
  T pop();

private:
  typedef std::list<T> t_queue;
  std::mutex m_mutex;
  std::condition_variable m_condition;
  t_queue m_queue;
};
//class TwoEndQueue

template<class T>
TwoEndQueue<T>::TwoEndQueue() :
    m_mutex(), m_condition(), m_queue()
{
}

template<class T>
TwoEndQueue<T>::~TwoEndQueue()
{
}

template<class T>
void TwoEndQueue<T>::pushFront(const T& p_t)
{
  std::unique_lock < std::mutex > lock(m_mutex);
  m_queue.push_front(p_t);
  m_condition.notify_one();
}

template<class T>
void TwoEndQueue<T>::pushBack(const T& p_t)
{
  std::unique_lock < std::mutex > lock(m_mutex);
  m_queue.push_back(p_t);
  m_condition.notify_one();
}

template<class T>
T TwoEndQueue<T>::pop()
{
  std::unique_ptr<T> result;
  {
    std::unique_lock < std::mutex > lock(m_mutex);
    while (m_queue.empty())
      m_condition.wait(lock);

    result.reset(new T(m_queue.front()));
    m_queue.pop_front();
  }

  return *result;
}

} //namespace Threading

#endif //TWO_END_QUEUE_HPP

