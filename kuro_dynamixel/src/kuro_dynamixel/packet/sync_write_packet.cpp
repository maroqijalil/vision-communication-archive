#include "kuro_dynamixel/packet/sync_write_packet.hpp"

#include <set>

using namespace kuro_dynamixel::packet;

const SyncWritePacket::Byte SyncWritePacket::Instruction = 0x83;

SyncWritePacket::SyncWritePacket()
  : packets()
{
}

SyncWritePacket::operator Packet() const
{
  Packet result_packet;

  result_packet.id = Packet::BroadcastId;
  result_packet.instruction = Instruction;

  unsigned char lowest_address = 0xFF;
  unsigned char highest_address = 0x00;
  for (auto &packet : packets)
  {
    for (auto &datum : packet.data)
    {
      unsigned char address = datum.first;
      if (address < lowest_address)
        lowest_address = address;

      if (address > highest_address)
        highest_address = address;
    }
  }

  int address_size = (highest_address - lowest_address) + 1;
  if (address_size < 0)
    return result_packet;

  result_packet.parameters.resize(2 + ((address_size + 1) * packets.size()));

  int index = 0;

  result_packet.parameters[index++] = lowest_address;
  result_packet.parameters[index++] = address_size;

  for (auto &packet : packets)
  {
    result_packet.parameters[index++] = packet.id;
    for (int i = 0; i < address_size; ++i)
    {
      auto datum = packet.data.find(lowest_address + i);
      if (datum != packet.data.end())
        result_packet.parameters[index++] = datum->second;
      else
        result_packet.parameters[index++] = 0x00;
    }
  }

  return result_packet;
}