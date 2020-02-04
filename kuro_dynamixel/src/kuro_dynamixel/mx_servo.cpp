#include "kuro_dynamixel/mx_servo.hpp"

#include "kuro_utility/angle.hpp"
#include "kuro_utility/math.hpp"

using namespace kuro_dynamixel;

using Angle = kuro_utility::Angle;
using Math = kuro_utility::Math;

const int MxServo::PositionMinValue(0);
const int MxServo::PositionMaxValue(4096);

MxServo::MxServo(int id) :
  Shareable<MxServo>(),
  _id(id),
  _read_position(0),
  _target_position(0)
{
}

int MxServo::AngleToValue(float angle)
{
  float transition = Math::progress(-180.0, 180.0, angle);
  return (int)Math::interpolate(PositionMinValue, PositionMaxValue, transition);
}

float MxServo::ValueToAngle(int value)
{
  float transition = Math::progress(PositionMinValue, PositionMaxValue, value);
  return Math::interpolate(-180.0, 180.0, transition);
}

int MxServo::getId()
{
  return _id;
}

void MxServo::setReadPosition(int value)
{
  _read_position = value;
}

int MxServo::getReadPosition()
{
  return _read_position;
}

void MxServo::setTargetPosition(int value)
{
  _target_position = value;
}

int MxServo::getTargetPosition()
{
  return _target_position;
}
