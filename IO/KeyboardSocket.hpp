#ifndef KEYBOARD_SOCKET_HPP
#define KEYBOARD_SOCKET_HPP

#include "Socket.hpp"

class KeyboardSocket: public Socket
{
public:
  typedef std::shared_ptr<KeyboardSocket> Ptr;

public:
  KeyboardSocket();
  virtual ~KeyboardSocket();
};

#endif
