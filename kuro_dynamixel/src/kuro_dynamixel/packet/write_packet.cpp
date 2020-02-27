#include "kuro_dynamixel/packet/write_packet.hpp"

using namespace kuro_dynamixel::packet;

const WritePacket::Byte WritePacket::Instruction = 0x03;

WritePacket::WritePacket(Byte id)
  : id(id),
    data()
{
}

WritePacket::operator Packet() const
{
  Packet packet;

  packet.id = id;
  packet.instruction = Instruction;

  unsigned char lowest_address = 0xFF;
  unsigned char highest_address = 0x00;
  for (auto &datum : data)
  {
    unsigned char address = datum.first;
    if (address < lowest_address)
      lowest_address = address;

    if (address > highest_address)
      highest_address = address;
  }

  int address_size = (highest_address - lowest_address) + 1;
  if (address_size < 0)
    return packet;

  packet.parameters.resize(address_size + 1);
  packet.parameters[0] = lowest_address;

  for (int i = 0; i < address_size; ++i)
  {
    auto datum = data.find(lowest_address + i);
    if (datum != data.end())
      packet.parameters[i + 1] = datum->second;
    else
      packet.parameters[i + 1] = 0x00;
  }

  return packet;
}