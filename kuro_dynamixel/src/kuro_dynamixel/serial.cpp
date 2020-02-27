#include "kuro_dynamixel/serial.hpp"

#include <chrono>
#include <future>

using namespace kuro_dynamixel;
using namespace std::chrono_literals;

Serial::Serial()
  : _port_name("/dev/ttyUSB0"),
    _baud_rate(1000000),
    _io_service(),
    _serial_port(_io_service),
    _is_packet_read(false),
    _read_packet(),

    _timer(_io_service),
    timeout(50)
{
}

Serial::~Serial()
{
  close();
}

Serial::operator Configuration() const
{
  Configuration configuration;
  configuration["port_name"] = _port_name;
  configuration["baud_rate"] = _baud_rate;

  return configuration;
}

Serial &Serial::operator=(const Configuration &configuration)
{
  if (configuration.isDictionary())
  {
    configuration["port_name"].setTo(_port_name);
    configuration["baud_rate"].setTo(_baud_rate);
  }

  return *this;
}
#include <iostream>
bool Serial::open()
{
  if (_serial_port.is_open())
    return false;

  try {
    _serial_port.open(_port_name);
    _serial_port.set_option(boost::asio::serial_port_base::baud_rate(_baud_rate));
  }
  catch (std::exception &error) {
    return false;
  }

  return true;
}

bool Serial::close()
{
  if (!_serial_port.is_open())
    return true;

  try {
    _serial_port.close();
  }
  catch (std::exception &error) {
    return false;
  }

  return true;
}

bool Serial::restart()
{
  return close() && open();
}

bool Serial::write(const Packet &packet)
{
  if (!_serial_port.is_open())
    return false;

  std::vector<unsigned char> bytes = packet;

  unsigned int written = 0;
  while (written < bytes.size())
  {
    bytes.resize(bytes.size() - written);
    written = boost::asio::write(_serial_port, boost::asio::buffer(bytes));
    for (unsigned int i = 0; written + i < bytes.size(); ++i)
    {
      bytes[i] = bytes[written + i];
    }
  }

  if (packet.id == Packet::BroadcastId)
    return true;

  return read();
}

bool Serial::read()
{
  if (!_serial_port.is_open())
    return false;

  std::vector<unsigned char> read_bytes;
  bool read_state = true;
  std::size_t i = 0;

  while (read_state)
  {
    read_bytes.resize(read_bytes.size() + 1);

    read_state = read_char(read_bytes[i++]);
  }

  unsigned char checksum = 0x00;
  for (i = 2; i < read_bytes.size() - 2; ++i)
    checksum += read_bytes[i];
  read_bytes[read_bytes.size() - 1] = ~checksum;

  if (read_bytes.size() >= 4)
  {
    for (auto byte : read_bytes)
      printf("%2X ", byte);
    printf("\n");
  }
  else
  {
    printf("failed!!!\n");

    return false;
  }

  _is_packet_read = true;
  _read_packet = read_bytes;

  return true;
}

bool Serial::isPacketRead()
{
  if (!_is_packet_read)
    return false;

  _is_packet_read = false;
  return true;
}

const Serial::Packet &Serial::getReadPacket() const
{
  return _read_packet;
}

void Serial::read_complete(const boost::system::error_code& error, size_t bytes_transferred)
{
  read_error = (error || bytes_transferred == 0);
  
  _timer.cancel();
}

void Serial::time_out(const boost::system::error_code& error)
{
  if (error)
    return;
  
  _serial_port.cancel();
}

bool Serial::read_char(unsigned char& byte)
{
  unsigned char c = '\0';

  byte = c;

  _io_service.reset();

  boost::asio::async_read(_serial_port, boost::asio::buffer(&c, 1), 
    boost::bind(&Serial::read_complete, this, 
    boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred)); 

  _timer.expires_from_now(boost::posix_time::milliseconds(timeout));
  _timer.async_wait(boost::bind(&Serial::time_out,
    this, boost::asio::placeholders::error));

  _io_service.run();

  if (!read_error)
    byte = c;

  return !read_error;
}