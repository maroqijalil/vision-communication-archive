#ifndef KURO_DYNAMIXEL_JOINT_LEG_JOINTS_HPP_
#define KURO_DYNAMIXEL_JOINT_LEG_JOINTS_HPP_

#include "kuro_dynamixel/joint/joint.hpp"

#include "kuro_dynamixel/packet.hpp"

#include "kuro_motion_interfaces/msg/leg_joints_message.hpp"
#include "kuro_utility/configuration.hpp"

#include <vector>

namespace kuro_dynamixel
{
  namespace joint
  {
    class LegJoints
    {
    public:

      using SyncWritePacket = kuro_dynamixel::packet::SyncWritePacket;

      using LegJointsMessage = kuro_motion_interfaces::msg::LegJointsMessage;
      using Configuration = kuro_utility::configuration::Configuration;

      LegJoints();

      operator std::vector<Joint>() const;
      operator Configuration() const;
      operator LegJointsMessage() const;
      operator SyncWritePacket() const;

      LegJoints &operator=(const Configuration &configuration);
      LegJoints &operator=(const LegJointsMessage &message);

      Joint hipYaw;
      Joint hipRoll;
      Joint hipPitch;
      Joint knee;
      Joint anklePitch;
      Joint ankleRoll;
    };
  }
}

#endif