#ifndef KURO_DYNAMIXEL_PACKET_WRITE_PACKET_HPP_
#define KURO_DYNAMIXEL_PACKET_WRITE_PACKET_HPP_

#include "kuro_dynamixel/packet/packet.hpp"

#include "kuro_utility/type.hpp"

#include <map>

namespace kuro_dynamixel
{
  namespace packet
  {
    class WritePacket
    {
    public:

      using Byte = kuro_utility::type::Byte;

      static const Byte Instruction;

      WritePacket(const Byte id);

      operator Packet() const;

      Byte id;
      std::map<Byte, Byte> data;
    };
  }
}

#endif