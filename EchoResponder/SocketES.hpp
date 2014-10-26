#ifndef SOCKET_ES_HPP
#define SOCKET_ES_HPP

#include "../EventEngines/EventSource.hpp"
#include "../IO/Socket.hpp"

class SocketES: public EventSource
{
public:
  typedef std::shared_ptr<SocketES> Ptr;

public:
  virtual ~SocketES();

  Descriptor getDescriptor() const;
  Socket::Ptr getSocket() const;

protected:
  explicit SocketES(Socket::Ptr);

protected:
  Socket::Ptr m_socket;
};

#endif
