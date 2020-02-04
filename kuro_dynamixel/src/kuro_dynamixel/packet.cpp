#include "kuro_dynamixel/packet.hpp"

#include "kuro_utility/binary.hpp"

using namespace kuro_dynamixel;

using Binary = kuro_utility::Binary;

const int Packet::IdBroadcast = 254;
const uint16_t Packet::DefaultHeader = 0xFFFF;

Packet::Packet(int id, Packet::Instruction instruction, std::vector<uint8_t> parameters) :
  _header(0x0000),
  _id((uint8_t)id),
  _length(0x00),
  _instruction((uint8_t)instruction),
  _parameters(parameters),
  _checksum(0x00)
{
  setup();
}

Packet::Packet() :
  Packet(0, Instruction::Ping, { })
{
}

Packet Packet::makeFromBytes(std::vector<uint8_t> bytes)
{
  Packet packet;

  if (bytes.size() > (int)ByteIndex::Header + 1)
    packet._header = Binary::makeWord(bytes[(int)ByteIndex::Header], bytes[(int)ByteIndex::Header + 1]);
  else if (bytes.size() > (int)ByteIndex::Header)
    packet._header = Binary::makeWord(bytes[(int)ByteIndex::Header], 0x00);

  if (bytes.size() > (int)ByteIndex::Id)
    packet._id = bytes[(int)ByteIndex::Id];

  if (bytes.size() > (int)ByteIndex::Length)
    packet._length = bytes[(int)ByteIndex::Length];

  if (bytes.size() > (int)ByteIndex::Instruction)
    packet._instruction = bytes[(int)ByteIndex::Instruction];

  // 1 last byte for checksum
  if (bytes.size() > (int)ByteIndex::Parameter + 1)
  {
    packet._parameters.resize(bytes.size() - ((int)ByteIndex::Parameter + 1), 0x00);
    for (std::size_t i = 0; i < packet._parameters.size(); ++i)
      packet._parameters[i] = bytes[(int)ByteIndex::Parameter + i];
  }

  if (bytes.size() > 0)
    packet._checksum = bytes[bytes.size() - 1];

  return packet;
}

std::vector<uint8_t> Packet::getBytes()
{
  std::vector<uint8_t> bytes;
  bytes.resize((int)ByteIndex::Parameter + _parameters.size() + 1);

  bytes[(int)ByteIndex::Header] = Binary::getLowByte(_header);
  bytes[(int)ByteIndex::Header + 1] = Binary::getHighByte(_header);

  bytes[(int)ByteIndex::Id] = _id;
  bytes[(int)ByteIndex::Length] = _length;
  bytes[(int)ByteIndex::Instruction] = _instruction;

  for (std::size_t i = 0; i < _parameters.size(); ++i)
    bytes[(int)ByteIndex::Parameter + i] = _parameters[i];

  bytes[bytes.size() - 1] = _checksum;

  return bytes;
}

void Packet::setup()
{
  // default headers
  _header = DefaultHeader;

  // 2 from instruction and checksum
  _length = _parameters.size() + 2;

  _checksum = calculateChecksum();
}

uint8_t Packet::calculateChecksum()
{
  uint8_t checksum = 0x00;

  auto bytes = getBytes();
  for (std::size_t i = (int)Packet::ByteIndex::Id; i < bytes.size() - 1; ++i)
    checksum += bytes[i];

  return ~checksum;
}