#include "kuro_dynamixel/joint/leg_joints.hpp"

using namespace kuro_dynamixel::joint;

LegJoints::LegJoints()
  : hipYaw(),
    hipRoll(),
    hipPitch(),
    knee(),
    anklePitch(),
    ankleRoll()
{
}

LegJoints::operator std::vector<Joint>() const
{
  std::vector<Joint> joints = { hipYaw, hipRoll, hipPitch, knee, anklePitch, ankleRoll };
  return joints;
}

LegJoints::operator Configuration() const
{
  Configuration configuration;
  configuration["hip_yaw"] = hipYaw;
  configuration["hip_roll"] = hipRoll;
  configuration["hip_pitch"] = hipPitch;
  configuration["knee"] = knee;
  configuration["ankle_pitch"] = anklePitch;
  configuration["ankle_roll"] = ankleRoll;

  return configuration;
}

LegJoints::operator LegJointsMessage() const
{
  LegJointsMessage message;
  message.hip_yaw = hipYaw;
  message.hip_roll = hipRoll;
  message.hip_pitch = hipPitch;
  message.knee = knee;
  message.ankle_pitch = anklePitch;
  message.ankle_roll = ankleRoll;

  return message;
}

LegJoints::operator SyncWritePacket() const
{
  SyncWritePacket packet;
  packet.packets.push_back(hipYaw);
  packet.packets.push_back(hipRoll);
  packet.packets.push_back(hipPitch);
  packet.packets.push_back(knee);
  packet.packets.push_back(anklePitch);
  packet.packets.push_back(ankleRoll);

  return packet;
}

LegJoints &LegJoints::operator=(const Configuration &configuration)
{
  if (configuration.isDictionary())
  {
    hipYaw = configuration["hip_yaw"];
    hipRoll = configuration["hip_roll"];
    hipPitch = configuration["hip_pitch"];
    knee = configuration["knee"];
    anklePitch = configuration["ankle_pitch"];
    ankleRoll = configuration["ankle_roll"];
  }

  return *this;
}

LegJoints &LegJoints::operator=(const LegJointsMessage &message)
{
  hipYaw = message.hip_yaw;
  hipRoll = message.hip_roll;
  hipPitch = message.hip_pitch;
  knee = message.knee;
  anklePitch = message.ankle_pitch;
  ankleRoll = message.ankle_roll;

  return *this;
}