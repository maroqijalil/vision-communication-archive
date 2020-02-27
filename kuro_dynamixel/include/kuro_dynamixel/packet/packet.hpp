#ifndef KURO_DYNAMIXEL_PACKET_PACKET_HPP_
#define KURO_DYNAMIXEL_PACKET_PACKET_HPP_

#include "kuro_utility/type.hpp"

#include <vector>

namespace kuro_dynamixel
{
  namespace packet
  {
    class Packet
    {
    public:

      using Byte = kuro_utility::type::Byte;
      using Word = kuro_utility::type::Word;

      static const Word Header;
      static const Byte BroadcastId;

      Packet();

      operator std::vector<unsigned char>() const;

      Packet &operator=(const std::vector<unsigned char> &bytes);

      bool isCorrect() const;

      Byte id;
      Byte instruction;
      std::vector<Byte> parameters;

    private:

      bool _correct;
    };
  }
}

#endif