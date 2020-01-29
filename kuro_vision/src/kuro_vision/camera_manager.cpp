#include "kuro_vision/camera_manager.hpp"

#include "opencv2/imgcodecs.hpp"

using namespace kuro_vision;

CameraManager::CameraManager(rclcpp::Node::SharedPtr rcl_node) :
  Shareable<CameraManager>(),
  Processable(),
  Configurable("kuro_vision", "camera_manager.yaml"),
  _rcl_node(rcl_node),
  _camera_image_rcl_publisher(_rcl_node->create_publisher<ImageMsg>("/camera_image", 10)),
  _compressed_camera_image_rcl_publisher(
    _rcl_node->create_publisher<CompressedImageMsg>("/compressed_camera_image", 10)
  ),
  _camera(Camera::makeShared())
{
  addSubprocess(_camera);
}

CameraManager::~CameraManager()
{
  stopUntilStopped();
}

// give a comment
CameraManager::Configuration CameraManager::encode()
{
  auto configuration = Configurable::encode();

  configuration["camera"] = _camera->encode();

  return configuration;
}

// give a comment
void CameraManager::decode(Configuration configuration)
{
  Configurable::decode(configuration);

  if (!configuration.isDictionary())
    return;

  _camera->decode(configuration["camera"]);
}

void CameraManager::onStart()
{
  Processable::onStart();

  if (_camera->start() == false)
    throw Exception("cannot start the Camera");
}

void CameraManager::onUpdate()
{
  Processable::onUpdate();

  compressedCameraImagePublish();
}

void CameraManager::cameraImagePublish()
{
  auto frame = _camera->getFrame();

  ImageMsg message;

  message.width = frame.cols;
  message.height = frame.rows;

  auto frame_size = message.width * message.height;
  message.data.resize(frame_size * 3);

  for (int i = 0; i < frame_size * 3; ++i)
  {
    message.data[i] = frame.data[i];
  }

  _camera_image_rcl_publisher->publish(message);
}

// give a comment
void CameraManager::compressedCameraImagePublish()
{
  CompressedImageMsg message;
  cv::imencode(".jpg", _camera->getFrame(), message.data, {cv::IMWRITE_JPEG_QUALITY, 80 });

  _compressed_camera_image_rcl_publisher->publish(message);
}