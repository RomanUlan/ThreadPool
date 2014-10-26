#include "Socket.hpp"

#include <stdexcept>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>

Socket::Socket(int p_domain, int p_type, int p_protocol)
  : m_fd(-1)
{
  m_fd = ::socket(p_domain, p_type, p_protocol);
  if (m_fd < 0)
  {
    throw std::runtime_error("Cannot create acceptor socket");
  }
}

Socket::Socket(int p_fd)
  : m_fd(p_fd)
{
}

Socket::~Socket()
{
  ::close(m_fd);
}

Socket::Descriptor Socket::getDescriptor() const
{
  return m_fd;
}

void Socket::setNonBlocking() const
{
  int flags = fcntl(m_fd, F_GETFL, 0);
  ::fcntl(m_fd, F_SETFL, flags | O_NONBLOCK);
}

void Socket::read(std::string& p_data) const
{
  char msg[2048];
  ::memset(msg, 0, 2048);
  int r = ::read(m_fd, msg, 2048);
  if (r < 0)
  {
    throw ::std::runtime_error("Cannot read");
  }

  p_data.append(msg, r);
}

void Socket::write(const std::string& p_data) const
{
  size_t len = p_data.length();
  ssize_t w = ::write(m_fd, p_data.c_str(), len);
  if (w < static_cast<ssize_t>(len))
  {
    throw ::std::runtime_error("Cannot write");
  }
}
