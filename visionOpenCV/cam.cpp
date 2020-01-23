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

  Mat frame, frame_HSV, frame_bit, frame_threshold;
  while (cap.read(frame)) {

    if(frame.empty())
    {
      break;
    }

    cvtColor(frame, frame_HSV, COLOR_BGR2HSV);
    blur( frame_HSV, frame_HSV, Size(31,31));

    inRange(frame_HSV, Scalar(94, 80, 2), Scalar(126, 255, 255), frame_threshold);
    bitwise_and(frame,frame,frame_bit, frame_threshold);

    vector<Vec4i> hierarchy;
    vector<vector<Point> > contours;
    findContours( frame_threshold, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE ,Point(0, 0));
    Mat drawing = Mat::zeros( frame_threshold.size(), CV_8UC3 );
    Scalar color = Scalar(255,0,0);
    int i;
    for(i = 0; i < contours.size(); i++) {
      drawContours (drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
    }

    imshow("window_capture_name", frame);
    imshow("window_detection_name", frame_bit);
    imshow("window_detection_nam2", drawing);

    char key = (char) waitKey(30);
    if (key == 'q' || key == 27)
    {
      break;
    }
  }
}
