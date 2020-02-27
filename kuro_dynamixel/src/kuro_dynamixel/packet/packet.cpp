#include "kuro_dynamixel/packet/packet.hpp"

using namespace kuro_dynamixel::packet;

const Packet::Word Packet::Header = 0xFFFF;
const Packet::Byte Packet::BroadcastId = 0xFE;

Packet::Packet()
  : id(),
    instruction(),
    parameters(),
    _correct(true)
{
}

Packet::operator std::vector<unsigned char>() const
{
  std::vector<unsigned char> bytes;
  bytes.resize(6 + parameters.size());

  int index = 0;

  bytes[index++] = Header.toLowByte();
  bytes[index++] = Header.toHighByte();

  bytes[index++] = id;
  bytes[index++] = parameters.size() + 2;
  bytes[index++] = instruction;

  for (unsigned int i = 0; i < parameters.size(); ++i)
    bytes[index++] = parameters[i];

  unsigned char checksum = 0x00;
  for (unsigned int i = 2; i < bytes.size() - 1; ++i)
    checksum += bytes[i];

  bytes[index++] = ~checksum;

  return bytes;
}

Packet &Packet::operator=(const std::vector<unsigned char> &bytes)
{
  _correct = true;

  // check minimum size
  if (bytes.size() < 6)
    _correct = false;

  // check header
  if ((bytes.size() >= 2) && (Word::makeFromBytes(bytes[0], bytes[1]) != 0xFFFF))
    _correct = false;

  if (bytes.size() >= 3)
    id = bytes[2];

  if (bytes.size() >= 5)
    instruction = bytes[4];

  // check parameters
  parameters.clear();
  if (bytes.size() >= 6)
  {
    parameters.resize(bytes.size() - 6);
    for (unsigned int i = 0; i < parameters.size(); ++i)
      parameters[i] = bytes[5 + i];
  }

  if ((bytes.size() >= 4) && (bytes[3] != parameters.size() + 2))
    _correct = false;

  // check checksum
  if (bytes.size() >= 2)
  {
    unsigned char checksum = 0x00;
    for (unsigned int i = 2; i < bytes.size() - 1; ++i)
      checksum += bytes[i];
    checksum = ~checksum;

    if (bytes[bytes.size() - 1] != checksum)
      _correct = false;
  }

  return *this;
}

bool Packet::isCorrect() const
{
  return _correct;
}