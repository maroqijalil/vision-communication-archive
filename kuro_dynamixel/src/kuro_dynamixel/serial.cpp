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

    _timeout(_io_service),
    // _timer(_io_service)
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
  std::vector<unsigned char> bytes(4);

  while (bytes.size() > 0)
  {
    auto read = boost::asio::read(_serial_port, boost::asio::buffer(bytes));

    read_bytes.resize(read_bytes.size() + read);
    for (unsigned int i = 0; i < (unsigned)read; ++i)
      read_bytes[read_bytes.size() - read + i] = bytes[i];
    bytes.resize(bytes.size() - read);

    if (read_bytes.size() >= 4)
    {
      if (read_bytes[3] + (unsigned)4 != read_bytes.size() + bytes.size())
        bytes.resize(read_bytes[3] + 4 - read_bytes.size());
    }
  }

  _is_packet_read = true;
  _read_packet = bytes;

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

// nonblocking read
bool Serial::asyncRead()
{
  std::vector<unsigned char> read_bytes;
  std::vector<unsigned char> bytes(1);

  _io_service.reset();

  data_available = false;

  _serial_port.async_read_some(boost::asio::buffer(bytes),
    boost::bind(&Serial::read_callback, boost::ref(data_available),
    boost::ref(_timeout), boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred));
  _timeout.expires_from_now(boost::posix_time::milliseconds(1000));
  _timeout.async_wait(boost::bind(&Serial::wait_callback,
    boost::ref(_serial_port), boost::asio::placeholders::error));

  _io_service.run();

  if (!data_available)
  {
    // kick_start_the_device();
    return false;
  }
  else
    return true;  
}

void Serial::read_callback(bool& data_available, boost::asio::deadline_timer& timeout, const boost::system::error_code& error, std::size_t bytes_transferred)
{
  if (error || !bytes_transferred)
  {
    data_available = false;
    return;
  }

  timeout.cancel();
  data_available = true;
}

void Serial::wait_callback(boost::asio::serial_port& ser_port, const boost::system::error_code& error)
{
  if (error)
    return;

  ser_port.cancel();
}


// // blocking read
// void Serial::read_complete(const boost::system::error_code& error, size_t bytes_transferred)
// {
//   read_error = (error || bytes_transferred == 0);
  
//   _timer.cancel();
// }

// void Serial::time_out(const boost::system::error_code& error)
// {
//   if (error)
//     return;

//   _serial_port.cancel();
// }

// bool Serial::read_char(char& val)
// {
//   char c;

//   val = c = '\0';

//   _io_service.reset();

//   boost::asio::async_read(_serial_port, boost::asio::buffer(&c, 1), 
//   boost::bind(&Serial::read_complete, 
//   this, 
//   boost::asio::placeholders::error, 
//   boost::asio::placeholders::bytes_transferred)); 

//   _timer.expires_from_now(boost::posix_time::milliseconds(timeout));
//   _timer.async_wait(boost::bind(&Serial::time_out,
//   this, boost::asio::placeholders::error));

//   _io_service.run();

//   if (!read_error)
//     val = c;

//   return !read_error;
// }