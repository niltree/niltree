#ifndef _EXO_VIDEO_MODE_H_
#define _EXO_VIDEO_MODE_H_

#include "VideoCommon.h"

#include <cassert>


extern "C" {
#include <libavutil/avstring.h>
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libswscale/swscale.h>
}

namespace ffmpeg {

class VideoCamera
{
public:
  AVFormatContext* pFormatCtx;
  int videoStream;
  AVCodec* pCodec;
  AVFrame* pFrame;
  AVFrame* pFrameRGB;
  AVPacket packet;
  VideoCamera(AVFormatContext* formatContext, AVCodec* codec, int videoStream);
  ~VideoCamera();
  AVCodecContext* getCodecContext() const;
  AVPixelFormat getFormat() const;
  size_t getWidth() const;
  size_t getHeight() const;
  size_t getSize() const;
  void copy(uint8_t* buffer) const;
  bool update();
  static VideoCamera* open(const char* deviceName, AVDictionary* options);
};

struct VideoMode {
  int width;
  int height;
  double FPS;

  VideoMode(int width = 0, int height = 0, double FPS = 0)
    : width(width)
    , height(height)
    , FPS(FPS)
  {
  }

  operator bool() const {
    return width || height || static_cast<int>(FPS);
  }

  bool operator==(const VideoMode& r) const {
    if (width != r.width) return false;
    if (height != r.height) return false;
    if (!FuzzyCompare(FPS, r.FPS)) return false;
    return true;
  }

  const DeviceString& toString() const {
    DeviceStringStream stringStream;
    stringStream << width << "x" << height << "@" << FPS << "fps";
    return stringStream.str();
  }

  static size_t getDevices(DeviceList& devices);
  static size_t getDeviceModes(VideoModeList& modes, const DeviceString& deviceName);
  static VideoCamera* open(const DeviceString& name, const DeviceString& opts);
  static void close(VideoCamera*& device);
};

}

#endif