#include "kuro_interface/msg/compressed_image_msg.hpp"
#include "kuro_interface/msg/image_msg.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "rclcpp/rclcpp.hpp"

using namespace std::placeholders;

class ImageViewer
{
public:

  using CompressedImageMsg = kuro_interface::msg::CompressedImageMsg;
  using ImageMsg = kuro_interface::msg::ImageMsg;

  ImageViewer();

  void spin();

private:

  // give a comment
  void cameraImageCallback(const ImageMsg::SharedPtr message);
  void compressedCameraImageCallback(const CompressedImageMsg::SharedPtr message);

  // give a comment
  rclcpp::Node::SharedPtr _rcl_node;
  rclcpp::Subscription<ImageMsg>::SharedPtr _camera_image_rcl_subscription;
  rclcpp::Subscription<CompressedImageMsg>::SharedPtr _compressed_camera_image_rcl_subscription;

  int _image_width;
  int _image_height;

  cv::Mat _image;
};

ImageViewer::ImageViewer() :
  _rcl_node(rclcpp::Node::make_shared("image_viewer")),
  _camera_image_rcl_subscription(
    _rcl_node->create_subscription<ImageMsg>("/camera_image", 10,
      std::bind(&ImageViewer::cameraImageCallback, this, _1)
    )
  ),
  _compressed_camera_image_rcl_subscription(
    _rcl_node->create_subscription<CompressedImageMsg>("/compressed_camera_image", 10,
      std::bind(&ImageViewer::compressedCameraImageCallback, this, _1)
    )
  ),
  _image_width(320),
  _image_height(240),
  _image(cv::Size(_image_width, _image_height), CV_8UC3)
{
}

void ImageViewer::spin()
{
  while (true)
  {
    rclcpp::spin_some(_rcl_node);
    cv::imshow("image_viewer", _image);

    if (cv::waitKey(1) == 'q')
      break;
  }
}

void ImageViewer::cameraImageCallback(const ImageMsg::SharedPtr message)
{
  if (_image_width != message->width || _image_height != message->height)
  {
    _image_width = message->width;
    _image_height = message->height;

    cv::resize(_image, _image, cv::Size(_image_width, _image_height));
  }

  // give a comment
  for (unsigned int i = 0; i < message->data.size(); ++i)
  {
    _image.data[i] = message->data[i];
  }
}

// give a comment
void ImageViewer::compressedCameraImageCallback(const CompressedImageMsg::SharedPtr messsage)
{
  _image = cv::imdecode(messsage->data, 1);
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  ImageViewer image_viewer;

  image_viewer.spin();

  return 0;
}