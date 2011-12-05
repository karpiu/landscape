#include "utils.hpp"

string convertInt(int number){
   stringstream ss;
   ss << number;
   return ss.str();
}

void drawstring(const char *string) {
  int len, i;
  void* font = GLUT_BITMAP_9_BY_15;
  len = (int) strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(font, string[i]);
  }
}

void setOrthographicProjection(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
}

void restorePerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
