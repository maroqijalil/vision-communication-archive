#include "kuro_dynamixel/joint/arm_joints.hpp"

using namespace kuro_dynamixel::joint;

ArmJoints::ArmJoints()
  : shoulderPitch(),
    shoulderRoll(),
    elbow()
{
}

ArmJoints::operator std::vector<Joint>() const
{
  std::vector<Joint> joints = { shoulderPitch, shoulderRoll, elbow };
  return joints;
}

ArmJoints::operator Configuration() const
{
  Configuration configuration;
  configuration["shoulder_pitch"] = shoulderPitch;
  configuration["shoulder_roll"] = shoulderRoll;
  configuration["elbow"] = elbow;

  return configuration;
}

ArmJoints::operator ArmJointsMessage() const
{
  ArmJointsMessage message;
  message.shoulder_pitch = shoulderPitch;
  message.shoulder_roll = shoulderRoll;
  message.elbow = elbow;

  return message;
}

ArmJoints::operator SyncWritePacket() const
{
  SyncWritePacket packet;
  packet.packets.push_back(shoulderPitch);
  packet.packets.push_back(shoulderRoll);

  return packet;
}

ArmJoints &ArmJoints::operator=(const Configuration &configuration)
{
  if (configuration.isDictionary())
  {
    shoulderPitch = configuration["shoulder_pitch"];
    shoulderRoll = configuration["shoulder_roll"];
    elbow = configuration["elbow"];
  }

  return *this;
}

ArmJoints &ArmJoints::operator=(const ArmJointsMessage &message)
{
  shoulderPitch = message.shoulder_pitch;
  shoulderRoll = message.shoulder_roll;
  elbow = message.elbow;

  return *this;
}