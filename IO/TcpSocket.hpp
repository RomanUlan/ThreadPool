#ifndef TCP_SOCKET_ES_HPP
#define TCP_SOCKET_ES_HPP

#include "Socket.hpp"
#include <sys/socket.h>

class TcpSocket: public Socket
{
public:
  typedef std::shared_ptr<TcpSocket> Ptr;

public:
  TcpSocket();
  virtual ~TcpSocket();

public:
  void bind(int);
  void listen() const;
  TcpSocket::Ptr accept() const;


private:
  TcpSocket(int, const struct sockaddr&, socklen_t);

private:
  struct sockaddr m_address;
  socklen_t m_addressLength;
};

#endif
