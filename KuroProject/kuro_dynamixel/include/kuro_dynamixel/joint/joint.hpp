#ifndef KURO_DYNAMIXEL_JOINT_JOINT_HPP_
#define KURO_DYNAMIXEL_JOINT_JOINT_HPP_

#include "kuro_dynamixel/packet.hpp"

#include "kuro_motion_interfaces/msg/joint_message.hpp"
#include "kuro_utility/configuration.hpp"
#include "kuro_utility/type.hpp"

namespace kuro_dynamixel
{
  namespace joint
  {
    class Joint
    {
    public:

      using WritePacket = kuro_dynamixel::packet::WritePacket;

      using JointMessage = kuro_motion_interfaces::msg::JointMessage;
      using Configuration = kuro_utility::configuration::Configuration;
      using Byte = kuro_utility::type::Byte;
      using Word = kuro_utility::type::Word;

      enum class Address : int
      {
        TorqueEnable = 24,
        DGain = 26,
        IGain = 27,
        PGain = 28,
        GoalPosition = 30
      };

      Joint();

      operator Configuration() const;
      operator JointMessage() const;
      operator WritePacket() const;

      Joint &operator=(const Configuration &configuration);
      Joint &operator=(const JointMessage &message);

      Byte id;

      Word position;
      Byte pGain;
      Byte iGain;
      Byte dGain;

    private:

      bool _inverted;
    };
  }
}

#endif
