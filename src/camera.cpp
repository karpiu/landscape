// view change guards
bool b_view = true;
bool b_view_ch = true;

// camera position
GLfloat c_xpos = 1.0f;
GLfloat c_ypos = 1.0f;
GLfloat c_zpos = 5.0f;

// camera rotation
GLfloat c_yrot = 0.0f;

// view vector
GLfloat vx = 0.0f;
GLfloat vy = 0.0f;
GLfloat vz = -1.0f;

// movement deltas
GLfloat deltaAngle = 0.0f;
GLfloat deltaMove = 0.0f;
GLfloat deltaUpMove = 0.0f;

void computePos(float deltaMove, float deltaUpMove) {
	c_xpos += deltaMove * vx * 0.1f;
  c_ypos += deltaUpMove * 0.1f;
	c_zpos += deltaMove * vz * 0.1f;
}

void computeDir(float deltaAngle) {
	c_yrot += deltaAngle;
	vx = sin(c_yrot);
	vz = -cos(c_yrot);
}
