#ifndef KURO_DYNAMIXEL_SERIAL_HPP_
#define KURO_DYNAMIXEL_SERIAL_HPP_

#include "kuro_dynamixel/packet.hpp"

#include "boost/asio.hpp"
#include "kuro_utility/process.hpp"
#include "kuro_utility/shareable.hpp"

#include <string>

namespace kuro_dynamixel
{
  class Serial :
    public kuro_utility::Shareable<Serial>,
    public kuro_utility::process::Processable
  {
  public:

    Serial();
    ~Serial();

    bool process(Packet packet);

  protected:

    void onStart() override;
    void onStop() override;

  private:

    void send(Packet packet);

    std::string _port_name;
    int _baudrate;

    boost::asio::io_service _io_service;
    boost::asio::serial_port _serial_port;
  };
}

#endif