#pragma once

#include <time.h>

class fps
{
  int frame_count;
  int current_fps;
  clock_t nextUpdate;

  public:
    fps();
    ~fps();

    void updateFps();
    int getFps();
};
