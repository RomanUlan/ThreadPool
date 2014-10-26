#include "TcpSocket.hpp"

#include <stdexcept>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>

TcpSocket::TcpSocket()
  : Socket(AF_INET, SOCK_STREAM, 0), m_address()
  , m_addressLength(sizeof(m_address))
{
  ::memset(&m_address, 0, m_addressLength);
}

TcpSocket::TcpSocket(int p_fd, const struct sockaddr& p_address, socklen_t p_addrLenngth)
  : Socket(p_fd), m_address(), m_addressLength(p_addrLenngth)
{
  ::memcpy(&m_address, &p_address, p_addrLenngth);
}

TcpSocket::~TcpSocket()
{
  ::close(m_fd);
}

void TcpSocket::bind(int p_port)
{
  struct sockaddr_in addr;
  ::memset(&addr, 0, sizeof(addr));

  addr.sin_family = AF_INET;
  addr.sin_port = htons(p_port);
  addr.sin_addr.s_addr = INADDR_ANY;

  ::memcpy(&m_address, &addr, sizeof(addr));
  m_addressLength = sizeof addr;

  if (::bind(m_fd, &m_address, m_addressLength) < 0) {
    throw std::runtime_error("Cannot bind acceptor socket");
  }
}

void TcpSocket::listen() const
{
  if (::listen(m_fd, 1) != 0) {
    throw std::runtime_error("Cannot listen on acceptor socket");
  }
}

TcpSocket::Ptr TcpSocket::accept() const
{
  struct sockaddr addr;
  ::memset(&addr, 0, sizeof(addr));
  socklen_t addrLen = sizeof(addr);
  int fd = ::accept(m_fd, &addr, &addrLen);
  if (fd < 0)
  {
    throw ::std::runtime_error("Cannot accept incoming connection");
  }

  return TcpSocket::Ptr(new TcpSocket(fd, addr, addrLen));
}
