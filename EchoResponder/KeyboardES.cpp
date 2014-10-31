#include "KeyboardES.hpp"
#include "../IO/Epoll.hpp"

KeyboardES::KeyboardES(KeyboardSocket::Ptr p_keybSocket)
  : SocketES(p_keybSocket)
{
  m_eventTypes.insert(Epoll::EventType::Error);
  m_eventTypes.insert(Epoll::EventType::In);
  m_eventTypes.insert(Epoll::EventType::OneShot);

  m_socket->setNonBlocking();
}

KeyboardES::~KeyboardES()
{
}

void KeyboardES::read(std::string& p_data)
{
  m_socket->read(p_data);
}
