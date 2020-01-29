#ifndef KURO_VISION_CAMERA_MANAGER_HPP_
#define KURO_VISION_CAMERA_MANAGER_HPP_

#include "kuro_vision/device.hpp"

#include "kuro_interface/msg/compressed_image_msg.hpp"
#include "kuro_interface/msg/image_msg.hpp"
#include "kuro_utility/configuration.hpp"
#include "kuro_utility/process.hpp"
#include "kuro_utility/shareable.hpp"
#include "rclcpp/rclcpp.hpp"

namespace kuro_vision
{
  class CameraManager :
    public kuro_utility::Shareable<CameraManager>,
    public kuro_utility::process::Processable,
    public kuro_utility::configuration::Configurable
  {
  public:

    using Camera = kuro_vision::device::Camera;

    using CompressedImageMsg = kuro_interface::msg::CompressedImageMsg;
    using ImageMsg = kuro_interface::msg::ImageMsg;

    CameraManager(rclcpp::Node::SharedPtr rcl_node);
    ~CameraManager();

    Configuration encode() override;
    void decode(Configuration configuration) override;

  protected:

    void onStart() override;
    void onUpdate() override;

  private:

    void cameraImagePublish();
    void compressedCameraImagePublish();

    rclcpp::Node::SharedPtr _rcl_node;

    rclcpp::Publisher<ImageMsg>::SharedPtr _camera_image_rcl_publisher;
    rclcpp::Publisher<CompressedImageMsg>::SharedPtr _compressed_camera_image_rcl_publisher;

    Camera::SharedPtr _camera;
  };
}

#endif