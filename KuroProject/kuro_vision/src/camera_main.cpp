#include "kuro_utility/process.hpp"
#include "kuro_vision/camera_manager.hpp"
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class CameraMain :
  public kuro_utility::process::ProcessableNode
{
public:

  using CameraManager = kuro_vision::CameraManager;

  CameraMain();
  ~CameraMain();

protected:

  // give a comment
  void onStart() override;

private:

  CameraManager::SharedPtr _camera_manager;
};

CameraMain::CameraMain() :
  ProcessableNode("camera", rclcpp::Rate::make_shared(60ms)),
  _camera_manager(CameraManager::makeShared(getNode()))
{
  // give a comment
  _camera_manager->load();
  addSubprocess(_camera_manager);
}

CameraMain::~CameraMain()
{
  stopUntilStopped();
}

void CameraMain::onStart()
{
  Processable::onStart();

  if (_camera_manager->start() == false)
    throw Exception("cannot start the CameraManager");
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  CameraMain camera_main;

  camera_main.start();
  camera_main.spin();

  return 0;
}