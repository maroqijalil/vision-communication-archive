#include "kuro_dynamixel/serial.hpp"

using namespace kuro_dynamixel;

Serial::Serial() :
  Shareable<Serial>(),
  Processable(),
  _port_name("cm740"),
  _baudrate(1000000),
  _io_service(),
  _serial_port(_io_service)
{
}

Serial::~Serial()
{
  stopUntilStopped();
}

bool Serial::process(Packet packet)
{
  if (!_serial_port.is_open())
    return false;

  send(packet);
  return true;
}

void Serial::onStart()
{
  Processable::onStart();

  try {
    _serial_port.open(_port_name);
    _serial_port.set_option(boost::asio::serial_port_base::baud_rate(_baudrate));
  }
  catch (std::exception &error) {
    throw Exception("cannot connect to the serial port");
  }
}

void Serial::onStop()
{
  Processable::onStop();

  try {
    _serial_port.close();
  }
  catch (std::exception &error) {
    throw Exception("cannot close the serial port");
  }
}
#include <iostream>
void Serial::send(Packet packet)
{
  auto bytes = packet.getBytes();
  
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

	std::vector<uint8_t> rxpacket;
  rxpacket.resize(6);
  int length = 0;

  while (length < rxpacket.size())
  {
    length = boost::asio::read(_serial_port, boost::asio::buffer(rxpacket));

    unsigned char checksum2 = 0x00;
    for(int i=2; i<rxpacket[(int)Packet::ByteIndex::Length]+3; i++)
      checksum2 += rxpacket[i];

    rxpacket.resize(rxpacket.size() - length);
  }
}