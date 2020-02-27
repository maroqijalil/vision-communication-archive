#ifndef KURO_VISION_DEVICE_CAMERA_HPP_
#define KURO_VISION_DEVICE_CAMERA_HPP_

#include "kuro_utility/configuration.hpp"
#include "kuro_utility/process.hpp"
#include "kuro_utility/shareable.hpp"

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"

#include <string>

namespace kuro_vision
{
  namespace device
  {
    class Camera :
      public kuro_utility::Shareable<Camera>,
      public kuro_utility::process::Processable,
      public kuro_utility::configuration::Configurable
    {
    public:

      Camera();
      ~Camera();

      Configuration encode() override;
      void decode(Configuration configuration) override;

      cv::Mat getFrame();

    protected:

      void onStart() override;
      void onStop() override;
      void onUpdate() override;

    private:

      cv::VideoCapture _video_capture;

      std::string _device_name;

      int _frame_width;
      int _frame_height;

      cv::Mat _frame;
    };
  }
}

#endif