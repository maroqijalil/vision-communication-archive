#include "kuro_vision/device/camera.hpp"

using namespace kuro_vision::device;

Camera::Camera() :
  Shareable<Camera>(),
  Processable(),
  Configurable("kuro_vision", "camera.yaml"),
  _video_capture(),
  _device_name("/dev/video0"),
  _frame_width(320),
  _frame_height(240),
  _frame(cv::Size(_frame_height, _frame_width), CV_8UC3)
{
}

Camera::~Camera()
{
  stopUntilStopped();
}

Camera::Configuration Camera::encode()
{
  auto configuration = Configurable::encode();

  configuration["device_name"] = _device_name;
  configuration["frame_width"] = _frame_width;
  configuration["frame_height"] = _frame_height;

  return configuration;
}

void Camera::decode(Configuration configuration)
{
  Configurable::decode(configuration);

  if (!configuration.isDictionary())
    return;

  configuration["device_name"].setTo(&_device_name);
  configuration["frame_width"].setTo(&_frame_width);
  configuration["frame_height"].setTo(&_frame_height);
}

cv::Mat Camera::getFrame()
{
  return _frame;
}

void Camera::onStart()
{
  Processable::onStart();

  if (_video_capture.isOpened())
    _video_capture.release();

  if (_video_capture.open(_device_name) == false)
    throw Exception("cannot open the camera device");

  if (_video_capture.set(cv::CAP_PROP_FRAME_WIDTH, _frame_width) == false)
    throw Exception("cannot set the frame width");

  if (_video_capture.set(cv::CAP_PROP_FRAME_HEIGHT, _frame_height) == false)
    throw Exception("cannot set the frame height");

  _frame.resize(_frame_height, _frame_width);
}

void Camera::onStop()
{
  Processable::onStop();

  if (_video_capture.isOpened())
    _video_capture.release();
}

void Camera::onUpdate()
{
  Processable::onUpdate();

  _video_capture.read(_frame);
}