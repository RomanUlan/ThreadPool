#include "KeyboardEH.hpp"
#include "../IO/Epoll.hpp"

#include <stdexcept>
#include <boost/algorithm/string.hpp>

KeyboardEH::KeyboardEH(const std::string& p_fileName, KeyboardES::Ptr p_kES, EventEngines::ThreadPool& p_tp)
  : EventHandler(p_kES)
  , m_file(p_fileName.c_str(), std::ofstream::out)
  , m_threadPool(p_tp)
{
  if (!m_file.is_open())
  {
    throw std::runtime_error("Cannot open file");
  }
}

KeyboardEH::~KeyboardEH()
{
  m_file.close();
}

void KeyboardEH::handle(const EventSource::EventTypes& p_eventTypes)
{
  EventSource::EventTypes::const_iterator iIn = p_eventTypes.find(Epoll::EventType::In);
  if ( (iIn != p_eventTypes.end()) && (p_eventTypes.size() == 1) )
  {
    SocketES::Ptr socketES = std::dynamic_pointer_cast<SocketES>(m_eventSource);
    KeyboardES::Ptr keyboardES = std::dynamic_pointer_cast<KeyboardES>(socketES);
    std::string data;
    keyboardES->read(data);
    m_file << data;

    if (boost::istarts_with(data, "exit"))
    {
      m_threadPool.interrupt(true);
    }
  }
  else
  {
    throw std::runtime_error("Bad event for for acceptor");
  }
}
