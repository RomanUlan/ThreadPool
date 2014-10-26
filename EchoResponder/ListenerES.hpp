#ifndef LISTENER_ES_HPP
#define LISTENER_ES_HPP

#include "MessageES.hpp"

class ListenerES: public SocketES
{
public:
  typedef std::shared_ptr<ListenerES> Ptr;

public:
  ListenerES(TcpSocket::Ptr, int);
  virtual ~ListenerES();

  MessageES::Ptr accept() const;
};

#endif
