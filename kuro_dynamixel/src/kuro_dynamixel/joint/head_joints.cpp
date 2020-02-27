#include "kuro_dynamixel/joint/head_joints.hpp"

using namespace kuro_dynamixel::joint;

HeadJoints::HeadJoints()
  : neckYaw(),
    neckPitch()
{
}

HeadJoints::operator std::vector<Joint>() const
{
  std::vector<Joint> joints = { neckYaw, neckPitch };
  return joints;
}

HeadJoints::operator Configuration() const
{
  Configuration configuration;
  configuration["neck_yaw"] = neckYaw;
  configuration["neck_pitch"] = neckPitch;

  return configuration;
}

HeadJoints::operator HeadJointsMessage() const
{
  HeadJointsMessage message;
  message.neck_yaw = neckYaw;
  message.neck_pitch = neckPitch;

  return message;
}

HeadJoints::operator SyncWritePacket() const
{
  SyncWritePacket packet;
  packet.packets.push_back(neckYaw);
  packet.packets.push_back(neckPitch);

  return packet;
}

HeadJoints &HeadJoints::operator=(const Configuration &configuration)
{
  if (configuration.isDictionary())
  {
    neckYaw = configuration["neck_yaw"];
    neckPitch = configuration["neck_pitch"];
  }

  return *this;
}

HeadJoints &HeadJoints::operator=(const HeadJointsMessage &message)
{
  neckYaw = message.neck_yaw;
  neckPitch = message.neck_pitch;

  return *this;
}