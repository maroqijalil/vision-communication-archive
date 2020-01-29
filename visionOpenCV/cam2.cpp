#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
  VideoCapture cap(0);
  // VideoCapture cap2(0);
  Mat frame;
  // Mat frame2;

  cap.set(CAP_PROP_AUTO_EXPOSURE, 0.25);
  
  cap.set(CAP_PROP_FRAME_WIDTH, 320);
  cap.set(CAP_PROP_FRAME_HEIGHT, 240);
  cap.set(CAP_PROP_BRIGHTNESS, 100);
  cap.set(CAP_PROP_CONTRAST, 100);
  cap.set(CAP_PROP_SATURATION, 100);
  cap.set(CAP_PROP_GAIN, 50);
  cap.set(CAP_PROP_EXPOSURE, 300);


  cout << "Video " <<
        ": width = " << cap.get(CAP_PROP_FRAME_WIDTH) <<
        ", height = " << cap.get(CAP_PROP_FRAME_HEIGHT) << endl;
  
  // cout << "Video 2 " <<
  //       ": width = " << cap2.get(CAP_PROP_FRAME_WIDTH) <<
  //       ", height = " << cap2.get(CAP_PROP_FRAME_HEIGHT) << endl;

  while (cap.read(frame))
  {

    // cap2.read(frame2);

    if(frame.empty())
    {
      break;
    }

    imshow("Frame 1", frame);
    // imshow("Frame 2", frame2);

    char key = (char) waitKey(30);
    if (key == 'q' || key == 27)
    {
      break;
    }
  }
}
