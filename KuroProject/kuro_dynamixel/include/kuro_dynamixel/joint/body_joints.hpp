#ifndef KURO_DYNAMIXEL_JOINT_BODY_JOINTS_HPP_
#define KURO_DYNAMIXEL_JOINT_BODY_JOINTS_HPP_

#include "kuro_dynamixel/joint/arm_joints.hpp"
#include "kuro_dynamixel/joint/head_joints.hpp"
#include "kuro_dynamixel/joint/joint.hpp"
#include "kuro_dynamixel/joint/leg_joints.hpp"

#include "kuro_dynamixel/packet.hpp"

#include "kuro_motion_interfaces/msg/body_joints_message.hpp"
#include "kuro_utility/configuration.hpp"

namespace kuro_dynamixel
{
  namespace joint
  {
    class BodyJoints
    {
    public:

      using ArmJoints = kuro_dynamixel::joint::ArmJoints;
      using HeadJoints = kuro_dynamixel::joint::HeadJoints;
      using Joint = kuro_dynamixel::joint::Joint;
      using LegJoints = kuro_dynamixel::joint::LegJoints;

      using SyncWritePacket = kuro_dynamixel::packet::SyncWritePacket;

      using BodyJointsMessage = kuro_motion_interfaces::msg::BodyJointsMessage;
      using Configuration = kuro_utility::configuration::Configuration;

      BodyJoints();

      operator std::vector<Joint>() const;
      operator Configuration() const;
      operator BodyJointsMessage() const;
      operator SyncWritePacket() const;

      BodyJoints &operator=(const Configuration &configuration);
      BodyJoints &operator=(const BodyJointsMessage &message);

      HeadJoints head;
      ArmJoints leftArm;
      ArmJoints rightArm;
      LegJoints leftLeg;
      LegJoints rightLeg;
    };
  }
}

#endif