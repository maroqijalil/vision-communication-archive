#include "kuro_dynamixel/joint.hpp"
#include "kuro_dynamixel/packet.hpp"
#include "kuro_dynamixel/serial.hpp"

#include "kuro_motion/stream.hpp"
#include "kuro_utility/cli.hpp"
#include "kuro_utility/configuration.hpp"
#include "rclcpp/rclcpp.hpp"

#include <memory>
#include <vector>

using namespace std::chrono_literals;

using BodyJoints = kuro_dynamixel::joint::BodyJoints;
using Joint = kuro_dynamixel::joint::Joint;
using SyncWritePacket = kuro_dynamixel::packet::SyncWritePacket;
using WritePacket = kuro_dynamixel::packet::WritePacket;
using Packet = kuro_dynamixel::packet::Packet;
using Serial = kuro_dynamixel::Serial;

using BodyJointsReceiver = kuro_motion::stream::BodyJointsReceiver;
using BodyJointsStreamer = kuro_motion::stream::BodyJointsStreamer;
using Cli = kuro_utility::Cli;
using Configuration = kuro_utility::configuration::Configuration;

int main(int argc, char **argv)
{
  Cli::print("rcl init");
  rclcpp::init(argc, argv);

  Cli::print("define rcl node");
  auto shared_rcl_node = std::make_shared<rclcpp::Node>("kuro_dynamixel_manager");

  rclcpp::Rate rcl_rate(8ms);

  Cli::print("define serial");
  Serial serial;

  Cli::print("define body joints streamer");
  BodyJointsReceiver write_body_joints_receiver(shared_rcl_node, "/robot/write_body_joints");
  BodyJointsStreamer read_body_joints_streamer(shared_rcl_node, "/robot/read_body_joints");

  Cli::print("define body joints");
  BodyJoints body_joints;

  auto configuration = Configuration::load("kuro_dynamixel", "kuro_dynamixel_manager.yaml");
  if (configuration.isDictionary()) {
    Cli::print("load configuration");
    serial = configuration["serial"];
    body_joints = configuration["body_joints"];
  }
  else {
    Cli::print("fail to load configuration");
    return 1;
  }

  if (serial.open())
  {
    Cli::print("serial opened");
  }
  else
  {
    Cli::print("failed to open the serial");
    return 1;
  }

  Cli::print("write torque enable");
  WritePacket torque_enable_packet(200);
  torque_enable_packet.data.emplace(24, 1);
  serial.write(torque_enable_packet);

  sleep(1);

  Cli::print("write torque enable, one by one");
  std::vector<WritePacket*> torque_enable;
  torque_enable.resize(22);
  for (int i = 0; i < 22; ++i)
  {
    torque_enable[i] = new WritePacket(i+1);
    torque_enable[i]->data.emplace(24, 1);
    printf("write torque %d\n", i+1);
    serial.write(*torque_enable[i]);
  }

  // WritePacket broadcast_torque_enable_packet(Packet::BroadcastId);
  // broadcast_torque_enable_packet.data.emplace(24, 1);
  // serial.write(broadcast_torque_enable_packet);

  Cli::print("spin");
  while (rclcpp::ok())
  {
    rcl_rate.sleep();

    Cli::clearScreen();

    Cli::print("spin some");
    rclcpp::spin_some(shared_rcl_node);

    if (write_body_joints_receiver.messageReceived())
    {
      Cli::print("received write body joints message");
      body_joints = write_body_joints_receiver.getMessage();

      Cli::print("writing body joints packet");
      SyncWritePacket body_joints_packet = body_joints;
      serial.write(body_joints_packet);
    }

    Cli::print("publishing read body joints message");
    read_body_joints_streamer.publishMessage(body_joints);
  }

  return 0;
}