#include "SocketES.hpp"

SocketES::SocketES(Socket::Ptr p_socket)
  : EventSource(EventTypes()), m_socket(p_socket)
{
}

SocketES::~SocketES()
{
}

EventSource::Descriptor SocketES::getDescriptor() const
{
  return m_socket->getDescriptor();
}

Socket::Ptr SocketES::getSocket() const
{
  return m_socket;
}
