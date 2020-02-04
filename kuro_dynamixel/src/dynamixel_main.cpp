#include "kuro_dynamixel/dynamixel_manager.hpp"

#include "kuro_utility/process.hpp"
#include "kuro_utility/shareable.hpp"
#include "rclcpp/rclcpp.hpp"

namespace kuro_dynamixel
{
  class DynamixelMain :
    public kuro_utility::Shareable<DynamixelMain>,
    public kuro_utility::process::ProcessableNode
  {
  public:

    using DynamixelManager = kuro_dynamixel::DynamixelManager;

    DynamixelMain();
    ~DynamixelMain();

  protected:

    void onStart() override;

  private:

    DynamixelManager::SharedPtr _dynamixel_manager;
  };

  DynamixelMain::DynamixelMain() :
    Shareable<DynamixelMain>(),
    ProcessableNode("dynamixel"),
    _dynamixel_manager(DynamixelManager::makeShared(getNode()))
  {
    addSubprocess(_dynamixel_manager);
  }

  DynamixelMain::~DynamixelMain()
  {
    stopUntilStopped();
  }

  void DynamixelMain::onStart()
  {
    ProcessableNode::onStart();

    if (_dynamixel_manager->start() == false)
      throw Exception("cannot start the DynamixelManager");
  }
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  auto dynamixel = kuro_dynamixel::DynamixelMain::makeShared();

  dynamixel->start();
  dynamixel->spin();

  return 0;
}