#ifndef KURO_DYNAMIXEL_SERIAL_HPP_
#define KURO_DYNAMIXEL_SERIAL_HPP_

#include "kuro_dynamixel/packet.hpp"

#include "kuro_utility/configuration.hpp"
#include "boost/asio.hpp"
#include "boost/bind.hpp"

#include <string>
#include <vector>

namespace kuro_dynamixel
{
  class Serial
  {
  public:

    using Packet = kuro_dynamixel::packet::Packet;

    using Configuration = kuro_utility::configuration::Configuration;

    Serial();
    ~Serial();

    operator Configuration() const;

    Serial &operator=(const Configuration &configuration);

    bool open();
    bool close();

    bool restart();

    bool write(const Packet &packet);
    bool read();
    
    void read_complete(const boost::system::error_code& error, size_t bytes_transferred);
    void time_out(const boost::system::error_code& error);
    bool read_char(unsigned char& byte);

    bool isPacketRead();

    const Packet &getReadPacket() const;

  private:

    std::string _port_name;
    int _baud_rate;

    boost::asio::io_service _io_service;
    boost::asio::serial_port _serial_port;

    std::size_t timeout;
    boost::asio::deadline_timer _timer;
    bool read_error = true;

    bool _is_packet_read;

    Packet _read_packet;
  };
}

#endif