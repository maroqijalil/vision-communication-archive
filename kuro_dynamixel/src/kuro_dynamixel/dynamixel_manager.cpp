#include "kuro_dynamixel/dynamixel_manager.hpp"

#include "kuro_utility/binary.hpp"
#include "kuro_utility/exception.hpp"

#include <map>

using namespace kuro_dynamixel;
using namespace std::chrono_literals;
using namespace std::placeholders;

using Binary = kuro_utility::Binary;
using Exception = kuro_utility::Exception;

DynamixelManager::DynamixelManager(rclcpp::Node::SharedPtr rcl_node) :
  Shareable<DynamixelManager>(),
  Processable(),
  _rcl_node(rcl_node),
  _motor_read_positions_rcl_publisher(
    _rcl_node->create_publisher<MotorPositionsMsg>("/motor_read_positions", 10)
  ),
  _motor_target_positions_rcl_subscription(
    _rcl_node->create_subscription<MotorPositionsMsg>("/motor_target_positions", 10,
      std::bind(&DynamixelManager::motorTargetPositionsCallback, this, _1)
    )
  ),
  _request_motor_read_positions_rcl_service(
    _rcl_node->create_service<RequestMotorPositionsSrv>("/request_motor_read_positions",
      std::bind(&DynamixelManager::requestMotorReadPositionsCallback, this, _1, _2)
    )
  ),
  _serial(Serial::makeShared()),
  _mx_servos({ })
{
}

DynamixelManager::~DynamixelManager()
{
  stopUntilStopped();
}

void DynamixelManager::onStart()
{
  Processable::onStart();

  if (_serial->start() == false)
    throw Exception("cannot start the Serial");

  Packet packet(200, Packet::Instruction::Write, { 24, 1 });

  std::array<Packet *, 22> packet_on;

  for (int i = 0; i < 22; ++i)
  {
    packet_on[i] = new Packet(i+1, Packet::Instruction::Write, { 24, 1 });
  }

  if (_serial->restart() == false)
    throw Exception("cannot restart the Serial");

  _serial->process(packet);
  usleep(100000); // bisa sampai 78 ms
  for (int i = 0; i < 20; ++i)
  {
    _serial->process(*packet_on[i]);
  }
  // usleep(100000);
  // for (int i = 5; i < 10; ++i)
  // {
  //   _serial->process(*packetx[i]);
  // }
  // usleep(100000);
  // for (int i = 10; i < 15; ++i)
  // {
  //   _serial->process(*packetx[i]);
  // }
  // usleep(100000);
  // for (int i = 15; i < 20; ++i)
  // {
  //   _serial->process(*packetx[i]);
  // }
}

void DynamixelManager::onStop()
{
  Processable::onStop();

  if (_serial->stop() == false)
    throw Exception("cannot stop the Serial");
}

void DynamixelManager::onUpdate()
{
  Processable::onUpdate();

  motorReadPositionsPublish();
}

void DynamixelManager::motorReadPositionsPublish()
{
  readServoReadPositions();

   MotorPositionsMsg message;
  message.data.clear();

  for (auto mx_servo : _mx_servos)
  {
    MotorPositionMsg datum;
    datum.id = mx_servo->getId();
    datum.position = MxServo::ValueToAngle(mx_servo->getReadPosition());

    message.data.push_back(datum);
  }

  _motor_read_positions_rcl_publisher->publish(message);
}

void DynamixelManager::requestMotorReadPositionsCallback(const RequestMotorPositionsSrv::Request::SharedPtr request,
  RequestMotorPositionsSrv::Response::SharedPtr response)
{
  (void)request;

  response->data.clear();
  for (auto mx_servo : _mx_servos)
  {
    MotorPositionMsg datum;
    datum.id = mx_servo->getId();
    datum.position = MxServo::ValueToAngle(mx_servo->getReadPosition());

    response->data.push_back(datum);
  }
}

void DynamixelManager::motorTargetPositionsCallback(const MotorPositionsMsg::SharedPtr message)
{
  std::map<int, MxServo::SharedPtr> mx_servo_map;
  mx_servo_map.clear();

  for (auto mx_servo : _mx_servos)
  {
    mx_servo_map.emplace(mx_servo->getId(), mx_servo);
  }

  for (auto &datum : message->data)
  {
    if (mx_servo_map.find(datum.id) == mx_servo_map.end())
    {
      auto mx_servo = std::make_shared<MxServo>(datum.id);
      _mx_servos.push_back(mx_servo);

      mx_servo_map.emplace(mx_servo->getId(), mx_servo);
    }

    mx_servo_map[datum.id]->setTargetPosition(MxServo::AngleToValue(datum.position));
  }

  writeServoTargetPositions();
}

bool DynamixelManager::readServoReadPositions()
{
  // temporary, read to actual motor for later
  for (auto mx_servo : _mx_servos)
  {
    mx_servo->setReadPosition(mx_servo->getTargetPosition());
  }

  return true;
}

bool DynamixelManager::writeServoTargetPositions()
{
  std::vector<uint8_t> parameters;
  parameters.resize(2 + (3 * _mx_servos.size()), 0x00);

  parameters[0] = (uint8_t)MxServo::Address::GoalPosition;
  parameters[1] = 0x02;

  for (unsigned int i = 0; i < _mx_servos.size(); ++i)
  {
    parameters[(i * 3) + 2] = _mx_servos[i]->getId();

    auto target_position = _mx_servos[i]->getTargetPosition();
    parameters[(i * 3) + 3] = Binary::getLowByte(target_position);
    parameters[(i * 3) + 4] = Binary::getHighByte(target_position);
  }

  Packet packet(Packet::IdBroadcast, Packet::Instruction::SyncWrite, parameters);
  _serial->process(packet);

  return true;
}