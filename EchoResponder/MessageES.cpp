#include "MessageES.hpp"
#include "../IO/Epoll.hpp"

MessageES::MessageES(TcpSocket::Ptr p_tcpSocket)
  : SocketES(p_tcpSocket)
{
  m_eventTypes.insert(Epoll::EventType::Error);
  m_eventTypes.insert(Epoll::EventType::In);
  m_eventTypes.insert(Epoll::EventType::Hup);
  m_eventTypes.insert(Epoll::EventType::RdHup);
  m_eventTypes.insert(Epoll::EventType::OneShot);

  m_socket->setNonBlocking();
}

MessageES::~MessageES()
{
}

void MessageES::read(std::string& p_message) const
{
  m_socket->read(p_message);
}
