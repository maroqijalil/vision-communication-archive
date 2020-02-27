#ifndef KURO_DYNAMIXEL_PACKET_SYNC_WRITE_PACKET_HPP_
#define KURO_DYNAMIXEL_PACKET_SYNC_WRITE_PACKET_HPP_

#include "kuro_dynamixel/packet/packet.hpp"
#include "kuro_dynamixel/packet/write_packet.hpp"

#include "kuro_utility/type.hpp"

#include <vector>

namespace kuro_dynamixel
{
  namespace packet
  {
    class SyncWritePacket
    {
    public:

      using Byte = kuro_utility::type::Byte;

      static const Byte Instruction;

      SyncWritePacket();

      operator Packet() const;

      std::vector<WritePacket> packets;
    };
  }
}

#endif