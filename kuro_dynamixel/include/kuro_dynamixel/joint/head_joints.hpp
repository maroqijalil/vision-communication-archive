#ifndef KURO_DYNAMIXEL_JOINT_HEAD_JOINTS_HPP_
#define KURO_DYNAMIXEL_JOINT_HEAD_JOINTS_HPP_

#include "kuro_dynamixel/joint/joint.hpp"

#include "kuro_dynamixel/packet/sync_write_packet.hpp"

#include "kuro_motion_interfaces/msg/head_joints_message.hpp"
#include "kuro_utility/configuration.hpp"

#include <vector>

namespace kuro_dynamixel
{
  namespace joint
  {
    class HeadJoints
    {
    public:

      using SyncWritePacket = kuro_dynamixel::packet::SyncWritePacket;

      using HeadJointsMessage = kuro_motion_interfaces::msg::HeadJointsMessage;
      using Configuration = kuro_utility::configuration::Configuration;

      HeadJoints();

      operator std::vector<Joint>() const;
      operator Configuration() const;
      operator HeadJointsMessage() const;
      operator SyncWritePacket() const;

      HeadJoints &operator=(const Configuration &configuration);
      HeadJoints &operator=(const HeadJointsMessage &message);

      Joint neckYaw;
      Joint neckPitch;
    };
  }
}

#endif