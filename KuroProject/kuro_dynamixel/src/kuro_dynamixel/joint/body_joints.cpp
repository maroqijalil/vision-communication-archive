#include "kuro_dynamixel/joint/body_joints.hpp"

#include <list>

using namespace kuro_dynamixel::joint;

BodyJoints::BodyJoints()
  : head(),
    leftArm(),
    rightArm(),
    leftLeg(),
    rightLeg()
{
}

BodyJoints::operator std::vector<Joint>() const
{
  std::list<std::vector<Joint>> joints_list = { head, leftArm, rightArm, leftLeg, rightLeg };

  std::vector<Joint> merged_joints;
  for (auto &joints : joints_list)
    merged_joints.insert(merged_joints.end(), joints.begin(), joints.end());

  return merged_joints;
}

BodyJoints::operator Configuration() const
{
  Configuration configuration;
  configuration["head"] = head;
  configuration["left_arm"] = leftArm;
  configuration["right_arm"] = rightArm;
  configuration["left_leg"] = leftLeg;
  configuration["right_arm"] = rightArm;

  return configuration;
}

BodyJoints::operator BodyJointsMessage() const
{
  BodyJointsMessage message;
  message.head = head;
  message.left_arm = leftArm;
  message.right_arm = rightArm;
  message.left_leg = leftLeg;
  message.right_leg = rightLeg;

  return message;
}

BodyJoints::operator SyncWritePacket() const
{
  std::list<SyncWritePacket> packet_list = { head, leftArm, rightArm, leftLeg, rightLeg };

  SyncWritePacket merged_packet;
  for (auto &packet : packet_list)
  {
    merged_packet.packets.insert(
      merged_packet.packets.end(), packet.packets.begin(), packet.packets.end()
    );
  }

  return merged_packet;
}

BodyJoints &BodyJoints::operator=(const Configuration &configuration)
{
  if (configuration.isDictionary())
  {
    head = configuration["head"];
    leftArm = configuration["left_arm"];
    rightArm = configuration["right_arm"];
    leftLeg = configuration["left_leg"];
    rightLeg = configuration["right_leg"];
  }

  return *this;
}

BodyJoints &BodyJoints::operator=(const BodyJointsMessage &message)
{
  head = message.head;
  leftArm = message.left_arm;
  rightArm = message.right_arm;
  leftLeg = message.left_leg;
  rightLeg = message.right_leg;

  return *this;
}