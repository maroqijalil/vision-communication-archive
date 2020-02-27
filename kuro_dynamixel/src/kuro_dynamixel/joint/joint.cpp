#include "kuro_dynamixel/joint/joint.hpp"

#include "kuro_utility/math.hpp"

using namespace kuro_dynamixel::joint;

using Math = kuro_utility::Math;

Joint::Joint()
  : id(),
    position(2048),
    pGain(8),
    iGain(0),
    dGain(0),
    _inverted(false)
{
}

Joint::operator Configuration() const
{
  Configuration configuration;
  configuration["id"] = id;
  configuration["inverted"] = _inverted;

  return configuration;
}

Joint::operator JointMessage() const
{
  JointMessage message;

  if (_inverted)
    message.position.value = Math::interpolate(180.0, -180.0, (float)position / 4095.0);
  else
    message.position.value = Math::interpolate(-180.0, 180.0, (float)position / 4095.0);

  message.pid_gain.p_gain.value = (float)pGain / 8.0;
  message.pid_gain.i_gain.value = (float)iGain * 1000.0 / 2048.0;
  message.pid_gain.d_gain.value = (float)dGain * 4.0 / 1000.0;

  return message;
}

Joint::operator WritePacket() const
{
  WritePacket packet(id);
  packet.data.emplace((int)Address::DGain, dGain);
  packet.data.emplace((int)Address::IGain, iGain);
  packet.data.emplace((int)Address::PGain, pGain);
  packet.data.emplace((int)Address::GoalPosition, position.toLowByte());
  packet.data.emplace((int)Address::GoalPosition + 1, position.toHighByte());

  return packet;
}

Joint &Joint::operator=(const Configuration &configuration)
{
  if (configuration.isDictionary())
  {
    id = configuration["id"];
    configuration["inverted"].setTo(_inverted);
  }

  return *this;
}

Joint &Joint::operator=(const JointMessage &message)
{
  if (_inverted)
    position = Math::progress(180.0, -180.0, message.position.value) * 4095.0;
  else
    position = Math::progress(-180.0, 180.0, message.position.value) * 4095.0;

  pGain = message.pid_gain.p_gain.value * 8.0;
  iGain = message.pid_gain.i_gain.value * 2048.0 / 1000.0;
  dGain = message.pid_gain.d_gain.value * 1000.0/ 4.0;

  return *this;
}