#ifndef KURO_DYNAMIXEL_DYNAMIXEL_MANAGER_HPP_
#define KURO_DYNAMIXEL_DYNAMIXEL_MANAGER_HPP_

#include "kuro_dynamixel/mx_servo.hpp"
#include "kuro_dynamixel/serial.hpp"

#include "kuro_interface/msg/motor_position_msg.hpp"
#include "kuro_interface/msg/motor_positions_msg.hpp"
#include "kuro_interface/srv/request_motor_positions_srv.hpp"
#include "kuro_utility/process.hpp"
#include "kuro_utility/shareable.hpp"
#include "rclcpp/rclcpp.hpp"

#include <string>

namespace kuro_dynamixel
{
  class DynamixelManager :
    public kuro_utility::Shareable<DynamixelManager>,
    public kuro_utility::process::Processable
  {
  public:

    using MotorPositionMsg = kuro_interface::msg::MotorPositionMsg;
    using MotorPositionsMsg = kuro_interface::msg::MotorPositionsMsg;
    using RequestMotorPositionsSrv = kuro_interface::srv::RequestMotorPositionsSrv;
    using MxServo = kuro_dynamixel::MxServo;
    using Serial = kuro_dynamixel::Serial;

    DynamixelManager(rclcpp::Node::SharedPtr rcl_node);
    ~DynamixelManager();

  protected:

    void onStart() override;
    void onStop() override;
    void onUpdate() override;

  private:

    void motorReadPositionsPublish();
    void motorTargetPositionsCallback(const MotorPositionsMsg::SharedPtr message);

    void requestMotorReadPositionsCallback(const RequestMotorPositionsSrv::Request::SharedPtr request,
      RequestMotorPositionsSrv::Response::SharedPtr response);

    bool readServoReadPositions();
    bool writeServoTargetPositions();

    rclcpp::Node::SharedPtr _rcl_node;

    rclcpp::Publisher<MotorPositionsMsg>::SharedPtr _motor_read_positions_rcl_publisher;
    rclcpp::Subscription<MotorPositionsMsg>::SharedPtr _motor_target_positions_rcl_subscription;

    rclcpp::Service<RequestMotorPositionsSrv>::SharedPtr _request_motor_read_positions_rcl_service;

    Serial::SharedPtr _serial;
    MxServo::SharedPtrs _mx_servos;
  };
}

#endif