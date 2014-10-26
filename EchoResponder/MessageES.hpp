#ifndef ECHO_ES_HPP
#define ECHO_ES_HPP

#include "SocketES.hpp"
#include "../IO/TcpSocket.hpp"
#include "../EventEngines/EventSource.hpp"

class MessageES: public SocketES
{
public:
  typedef std::shared_ptr<MessageES> Ptr;

public:
  explicit MessageES(TcpSocket::Ptr);
  virtual ~MessageES();

  void read(std::string&) const;
};

#endif
