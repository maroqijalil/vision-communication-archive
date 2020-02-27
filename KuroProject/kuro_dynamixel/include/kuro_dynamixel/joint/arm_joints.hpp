#ifndef KURO_DYNAMIXEL_JOINT_ARM_JOINTS_HPP_
#define KURO_DYNAMIXEL_JOINT_ARM_JOINTS_HPP_

#include "kuro_dynamixel/joint/joint.hpp"

#include "kuro_dynamixel/packet/sync_write_packet.hpp"

#include "kuro_motion_interfaces/msg/arm_joints_message.hpp"
#include "kuro_utility/configuration.hpp"

#include <vector>

namespace kuro_dynamixel
{
  namespace joint
  {
    class ArmJoints
    {
    public:

      using SyncWritePacket = kuro_dynamixel::packet::SyncWritePacket;

      using ArmJointsMessage = kuro_motion_interfaces::msg::ArmJointsMessage;
      using Configuration = kuro_utility::configuration::Configuration;

      ArmJoints();

      operator std::vector<Joint>() const;
      operator Configuration() const;
      operator ArmJointsMessage() const;
      operator SyncWritePacket() const;

      ArmJoints &operator=(const Configuration &configuration);
      ArmJoints &operator=(const ArmJointsMessage &message);

      Joint shoulderPitch;
      Joint shoulderRoll;
      Joint elbow;
    };
  }
}

#endif