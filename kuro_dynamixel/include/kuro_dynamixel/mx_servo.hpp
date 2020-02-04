#ifndef KURO_DYNAMIXEL_MX_SERVO_
#define KURO_DYNAMIXEL_MX_SERVO_

#include "kuro_utility/shareable.hpp"

namespace kuro_dynamixel
{
  class MxServo :
    public kuro_utility::Shareable<MxServo>
  {
  public:

    enum class Address : int
    {
      Id = 2,
      TorqueEnable = 24,
      GoalPosition = 30,
      PresentPosition = 36,
    };

    MxServo(int id);

    static int AngleToValue(float angle);
    static float ValueToAngle(int value);

    int getId();

    void setReadPosition(int id);
    int getReadPosition();

    void setTargetPosition(int id);
    int getTargetPosition();

  private:

    static const int PositionMinValue;
    static const int PositionMaxValue;

    int _id;

    int _read_position;
    int _target_position;
  };
}

#endif