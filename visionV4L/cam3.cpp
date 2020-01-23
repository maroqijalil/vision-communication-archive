#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <linux/videodev2.h>
#include <iostream>

int width = 640;
int height = 320;

int getControl(int control, int camera_fd_)
{
  if (!isPortOpen())
    return -1;

  struct v4l2_queryctrl queryctrl;
  memset(&queryctrl, 0, sizeof(queryctrl));

  queryctrl.id = control;

  if (ioctl(camera_fd_, VIDIOC_QUERYCTRL, &queryctrl) == -1)
  {
    fprintf(stderr, "Query control failed: %d, %s\n", errno, strerror(errno));
    return -1;
  }

  struct v4l2_control control_s;
  memset(&control_s, 0, sizeof(control_s));

  control_s.id = control;

  if (ioctl(camera_fd_, VIDIOC_G_CTRL, &control_s) == -1)
  {
    fprintf(stderr, "Fail to get Control:  %d, %s\n", errno, strerror(errno));
    return -1;
  }

  return control_s.value;
}

int setControl(int control, int value, int camera_fd_)
{
  if (!isPortOpen())
    return -1;

  struct v4l2_queryctrl queryctrl;
  memset(&queryctrl, 0, sizeof(queryctrl));

  queryctrl.id = control;

  if (ioctl(camera_fd_, VIDIOC_QUERYCTRL, &queryctrl) == -1)
  {
    fprintf(stderr, "Query control failed: %d, %s\n", errno, strerror(errno));
    return -1;
  }

  if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
  {
    fprintf(stderr, "Setting control is disabled on %d", control);
    return -1;
  }

  struct v4l2_control control_s;
  memset(&control_s, 0, sizeof(control_s));

  if (value < queryctrl.minimum)
    value = queryctrl.minimum;
  else if (value > queryctrl.maximum)
    value = queryctrl.maximum;

  control_s.id = control;
  control_s.value = value;

  if (ioctl (camera_fd_, VIDIOC_S_CTRL, &control_s) == -1)
  {
    fprintf(stderr, "Fail to set control: %d, %s\n", errno, strerror(errno));
    return -1;
  }

  return value;
}

int main (int argc, char **argv)
{
  int cam_fd;
  std::string dev_name = "/dev/video0";
  struct v4l2_cropcap cropcap;
  struct v4l2_format fmt;
  struct v4l2_streamparm fps;
  struct v4l2_requestbuffers req;
  struct Buffer *buffers_;
  unsigned int n_buffers_;
  enum v4l2_buf_type type;

  memset(&cropcap, 0, sizeof(cropcap));
  memset(&fmt, 0, sizeof(fmt));
  memset(&fps, 0, sizeof(fps));
  memset(&req, 0, sizeof(req));

  cam_fd = open(dev_name, O_RDWR | O_NONBLOCK, 0);

  cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width       = width_;
  fmt.fmt.pix.height      = height_;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
  fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
  req.count   = 4;
  req.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory  = V4L2_MEMORY_MMAP;
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  ioctl(cam_fd, VIDIOC_CROPCAP, &cropcap);
  ioctl (cam_fd, VIDIOC_S_FMT, &fmt);
  ioctl(cam_fd, VIDIOC_G_PARM, &fps);
  ioctl(cam_fd, VIDIOC_REQBUFS, &req);
  ioctl (cam_fd, (true) ? VIDIOC_STREAMON : VIDIOC_STREAMOFF, &type);

  unsigned int min = fmt.fmt.pix.width * 2;
  if (fmt.fmt.pix.bytesperline < min)
    fmt.fmt.pix.bytesperline = min;

  min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
  if (fmt.fmt.pix.sizeimage < min)
    fmt.fmt.pix.sizeimage = min;

  fps.parm.capture.timeperframe.numerator = 1;
  fps.parm.capture.timeperframe.denominator = 30;

  ioctl(cam_fd, VIDIOC_S_PARM, &fps);

  buffers_ = (Buffer *)calloc(req.count, sizeof(*buffers_));

  for (n_buffers_ = 0; n_buffers_ < req.count; ++n_buffers_)
  {
    struct v4l2_buffer buf;
    memset(&buf, 0, sizeof(buf));

    buf.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory  = V4L2_MEMORY_MMAP;
    buf.index   = n_buffers_;

    if (ioctl(cam_fd, VIDIOC_QUERYBUF, &buf) == -1)
    {
      fprintf (stderr, "Fail to query the status of a buffer: %d, %s\n", errno, strerror(errno));
      return false;
    }

    buffers_[n_buffers_].length = buf.length;
    buffers_[n_buffers_].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE,MAP_SHARED, cam_fd, buf.m.offset);

    if (buffers_[n_buffers_].start == MAP_FAILED)
    {
      fprintf (stderr, "Fail to map memory\n");
      return false;
    }
  }

  for (unsigned int n = 0; n < n_buffers_; ++n)
  {
    struct v4l2_buffer buf;
    memset(&buf, 0, sizeof(buf));

    buf.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory  = V4L2_MEMORY_MMAP;
    buf.index   = n;

    if (ioctl(cam_fd, VIDIOC_QBUF, &buf) == -1)
    {
      fprintf (stderr, "Queue buffers failed: %d, %s\n", errno, strerror(errno));
      return false;
    }
  }

  setControl(V4L2_CID_EXPOSURE_AUTO, V4L2_EXPOSURE_MANUAL, cam_fd);
  setControl(V4L2_CID_AUTO_WHITE_BALANCE, false, cam_fd);
  setControl(V4L2_CID_SHARPNESS, 0, cam_fd);
  setControl(V4L2_CID_FOCUS_AUTO, false, cam_fd);
  setControl(V4L2_CID_FOCUS_ABSOLUTE, 0, cam_fd);
  setControl(V4L2_CID_POWER_LINE_FREQUENCY,V4L2_CID_POWER_LINE_FREQUENCY_50HZ, cam_fd);
}
