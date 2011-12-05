#pragma once

#include <string.h>
#include <sstream>
#include <GL/glut.h>

//
// procedures that could be reused
//

using namespace std;

string convertInt(int number);

void drawstring(const char *string);

// for drawing 'flat' text like FPS/TPS.. 
void setOrthographicProjection(int w, int h);
void restorePerspectiveProjection();
