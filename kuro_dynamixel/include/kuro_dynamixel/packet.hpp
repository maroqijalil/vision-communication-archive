#ifndef KURO_DYNAMIXEL_PACKET_HPP_
#define KURO_DYNAMIXEL_PACKET_HPP_

#include <cstdint>
#include <vector>

namespace kuro_dynamixel
{
  class Packet
  {
  public:

    static const int IdBroadcast;
    static const uint16_t DefaultHeader;

    enum class ByteIndex : int
    {
      Header = 0,
      Id = 2,
      Length = 3,
      Instruction = 4,
      Error = 4,
      Parameter = 5
    };

    enum class Instruction : int
    {
      Ping = 0x01,
      Read = 0x02,
      Write = 0x03,
      RegWrite = 0x04,
      Action = 0x05,
      FactoryReset = 0x06,
      Reboot = 0x08,
      SyncWrite = 0x83,
      BlukRead = 0x92
    };

    Packet(int id, Instruction instruction, std::vector<uint8_t> parameters);
    Packet();

    static Packet makeFromBytes(std::vector<uint8_t> bytes);

    std::vector<uint8_t> getBytes();

    void setup();

  private:

    uint8_t calculateChecksum();

    uint16_t _header;
    uint8_t _id;
    uint8_t _length;
    uint8_t _instruction;
    std::vector<uint8_t> _parameters;
    uint8_t _checksum;
  };
}

#endif