#include "../EventEngines/ThreadPool.hpp"
#include "../IO/Epoll.hpp"
#include "EpollED.hpp"
#include "AcceptorEH.hpp"
#include "KeyboardEH.hpp"

#include <iostream>
#include <stdexcept>

int main(int, char**)
{
  try
  {
    Epoll::Ptr epoll(new Epoll());
    EpollED::Ptr epollED(new EpollED(epoll));
    EventEngines::ThreadPool tp(epollED, 2);

    KeyboardSocket::Ptr keybSocket(new KeyboardSocket());
    KeyboardES::Ptr keybES(new KeyboardES(keybSocket));
    EventHandler::Ptr keybEH(new KeyboardEH("log.txt", keybES, tp));

    TcpSocket::Ptr listenerSokcet(new TcpSocket());
    ListenerES::Ptr listenerES(new ListenerES(listenerSokcet, 5050));
    EventHandler::Ptr acceptorEH(new AcceptorEH(listenerES, tp));

    tp.add(keybEH);
    tp.add(acceptorEH);

    tp.eventLoop();
  }
  catch (const std::runtime_error& rte)
  {
    std::cout << "Runtime exception: " << rte.what() << std::endl;
  }
  catch (const std::exception& e)
  {
    std::cout << "STD exception: " << e.what() << std::endl;
  }

  return 0;
}
