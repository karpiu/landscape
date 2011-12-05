#include "fps.hpp"


fps::fps()
{
  frame_count = 0;
  current_fps = 0;
  nextUpdate = 0;
}

fps::~fps()
{
  // void
}

void fps::updateFps()
{
  frame_count++;
  clock_t overtime = clock() - nextUpdate;
  if (overtime > 0)
  {
    current_fps = frame_count / (float)(1+ (float)overtime/(float)CLOCKS_PER_SEC);
    frame_count = 0;
    nextUpdate = clock() + 1 * CLOCKS_PER_SEC;
  }
}

int fps::getFps()
{
  return current_fps;
}
