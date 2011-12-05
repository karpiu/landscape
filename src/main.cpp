#ifdef _WIN32
  #include <windows.h>
  #include <time.h>
#endif

#include <GL/glut.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "utils.hpp"
#include "fps.hpp"
#include "lod_map.hpp"

#define PI_OVER_180  0.0174532925f
#define TABSIZE 1201

using namespace std;

//globals

fps fps_handler;

//window vars
int window_w, window_h;

// view change guards
bool b_view = true;
bool b_view_ch = true;

// key buffer
bool g_key[255];

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

int k = 5;
GLfloat tile_w = 0.01f;
GLfloat tile_l = -0.01f;
GLfloat tile_h = 0.001f;

// struct for storing height from SRTM file
struct node
{
  short h;
  GLfloat r,g,b;
} terrain[TABSIZE][TABSIZE];

lod_map mapa[9];

void create_level(int detail)
{
  int lod = detail+1;
  int tnum = 0;
  int vnum = 0;

  vector<GLfloat> vertv;
  vector<GLfloat> colv;

  for(int i=0; i<TABSIZE-lod; i+=lod)
  {
    for(int j=0; j<TABSIZE-lod; j+=lod)
    {
      colv.push_back(terrain[i][j].r); colv.push_back(terrain[i][j].g); colv.push_back(terrain[i][j].b);
      colv.push_back(terrain[i+lod][j+lod].r); colv.push_back(terrain[i+lod][j+lod].g); colv.push_back(terrain[i+lod][j+lod].b);
      colv.push_back(terrain[i][j+lod].r); colv.push_back(terrain[i][j+lod].g); colv.push_back(terrain[i][j+lod].b);
      colv.push_back(terrain[i][j].r); colv.push_back(terrain[i][j].g); colv.push_back(terrain[i][j].b);
      colv.push_back(terrain[i+lod][j].r); colv.push_back(terrain[i+lod][j].g); colv.push_back(terrain[i+lod][j].b);
      colv.push_back(terrain[i+lod][j+lod].r); colv.push_back(terrain[i+lod][j+lod].g); colv.push_back(terrain[i+lod][j+lod].b);

      vertv.push_back(tile_w*i); vertv.push_back(terrain[i][j].h*tile_h); vertv.push_back(tile_l*j);
      vertv.push_back(tile_w*(i+lod)); vertv.push_back(terrain[i+lod][j+lod].h*tile_h); vertv.push_back(tile_l*(j+lod));
      vertv.push_back(tile_w*i); vertv.push_back(terrain[i][j+lod].h*tile_h); vertv.push_back(tile_l*(j+lod));
      vertv.push_back(tile_w*i); vertv.push_back(terrain[i][j].h*tile_h); vertv.push_back(tile_l*j);
      vertv.push_back(tile_w*(i+lod)); vertv.push_back(terrain[i+lod][j].h*tile_h); vertv.push_back(tile_l*j);
      vertv.push_back(tile_w*(i+lod)); vertv.push_back(terrain[i+lod][j+lod].h*tile_h); vertv.push_back(tile_l*(j+lod));

      tnum+=2;
      vnum+=18;
    }
  }
  mapa[detail].initialize(vertv,colv,vnum,tnum);
}

void create_lod_map() {
  for(int i=0;i<9;i++) {
    create_level(i);
  }
}

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

void print_info()
{
  // start drawing text on screen
  setOrthographicProjection(window_w, window_h);

	glPushMatrix();
    glLoadIdentity();
    glColor3f(1.0f,1.0f,1.0f);
    glRasterPos2i(30,20);
    string fpss = "FPS: "; fpss += convertInt(fps_handler.getFps());
    drawstring(fpss.c_str());
    glRasterPos2i(30,35);
    string lods = "LOD: "; lods += convertInt(k);
    drawstring(lods.c_str());
    glRasterPos2i(30,50);
    string tris = "TPS: "; tris += convertInt(mapa[k-1].getTriangleCount() * fps_handler.getFps());
    drawstring(tris.c_str());
	glPopMatrix();

	restorePerspectiveProjection();
  // end drawing text
}

void render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (deltaUpMove || deltaMove)
      computePos(deltaMove, deltaUpMove);
    if (deltaAngle)
      computeDir(deltaAngle);

  glLoadIdentity();
  if(b_view)
  {
    gluLookAt(	c_xpos, c_ypos, c_zpos,
        c_xpos+vx, c_ypos,  c_zpos+vz,
        0.0f, 1.0f,  0.0f);
	}
	else
	{
    gluLookAt(	c_xpos, c_ypos, c_zpos,
        c_xpos, c_ypos-1.0f,  c_zpos,
        0.0f, 0.0f,  1.0f);
	}

	glPushMatrix();
    mapa[k-1].drawTerrain();
  glPopMatrix();
  
  print_info();

  glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (h == 0) h = 1;
	gluPerspective(45.0f, (float)w/(float)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	window_w = w;
	window_h = h;
}

void keyboard(unsigned char key, int x, int y) {
	if(key == 27)
    exit(0);
  if(key > '0' && key <= '9')
    k = key-48;
}

void keyboard_up(unsigned char key, int x, int y) {
  // void
}

void special_keys(int a_keys, int x, int y) {
	switch (a_keys) {
		case GLUT_KEY_F1:
			// switch view
			if (b_view_ch) {
        b_view = b_view ? false : true;
        b_view_ch = false;
        if (!b_view) {
          c_xpos = (TABSIZE/2)*tile_w;
          c_ypos = terrain[TABSIZE/2][TABSIZE/2].h*tile_h+10.0f;
          c_zpos = (TABSIZE/2)*tile_l;
        } else {
          c_xpos = (TABSIZE/2)*tile_w;
          c_ypos = terrain[TABSIZE/2][TABSIZE/2].h*tile_h+1.0f;
          c_zpos = (TABSIZE/2)*tile_l;
        }
			}
		break;
		default:
			g_key[a_keys] = true;
		break;
	}
}

void special_keys_up(int key, int x, int y) {
  if(key == GLUT_KEY_F1 && !b_view_ch) {
    b_view_ch = true;
  }
	g_key[key] = false;
}

void game_function()
{
  // up-down movement
	if (g_key[GLUT_KEY_PAGE_UP]) deltaUpMove = 0.5f;
	if (g_key[GLUT_KEY_PAGE_DOWN]) deltaUpMove = -0.5f;
  if (!g_key[GLUT_KEY_PAGE_UP] && !g_key[GLUT_KEY_PAGE_DOWN]) deltaUpMove = 0.0f;

  // front-back movement
	if (g_key[GLUT_KEY_UP]) deltaMove = 0.5f;
	if (g_key[GLUT_KEY_DOWN]) deltaMove = -0.5f;
	if (!g_key[GLUT_KEY_UP] && !g_key[GLUT_KEY_DOWN]) deltaMove = 0.0f;

  // camera rotation
	if (g_key[GLUT_KEY_LEFT]) deltaAngle = -0.01f;
	if (g_key[GLUT_KEY_RIGHT]) deltaAngle = 0.01f;
	if (!g_key[GLUT_KEY_LEFT] && !g_key[GLUT_KEY_RIGHT]) deltaAngle = 0.0f;

  render();

  fps_handler.updateFps();
}

void initialize ()
{
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

void read_input(const char* filename)
{
  short res,ht;
  int c1, c2;
  unsigned int u1, u2;
  FILE * f1;
  if( !(f1 = fopen(filename,"rb")) ){
    cout << "Error: could not open file " << filename << "\n";
    exit(0);
  }

  // read height from hgt file
  for(int i=0; i<TABSIZE; i++)
  {
    for(int j=0; j<TABSIZE; j++)
    {
      c1=fgetc(f1);
      c2=fgetc(f1);

      u1 = (unsigned int) c1;
      u2 = (unsigned int) c2;
      res = 0;
      res = (res << 8) + u1;
      res = (res << 8) + u2;

      terrain[i][j].h = res;

      // choose color for each vertex
      ht = terrain[i][j].h;
      if(ht < 0)
        { terrain[i][j].r=0.0f; terrain[i][j].g=0.0f; terrain[i][j].b=1.0f; } // blue
      else if (ht < 500 )
        { terrain[i][j].r=0.0f; terrain[i][j].g=ht/500.0f; terrain[i][j].b=0.0f; } //->green
      else if (ht < 1000)
        { terrain[i][j].r=ht/500.0f-1.0f; terrain[i][j].g=1.0f; terrain[i][j].b=0.0f; } //->yellow
      else if (ht < 1500)
        { terrain[i][j].r=1.0f; terrain[i][j].g=ht/500.0f-2.0f; terrain[i][j].b=0.0f; } //->red
      else
        { terrain[i][j].r=1.0f; terrain[i][j].g=1.0f; terrain[i][j].b=1.0f; }               //white
    }
  }
  fclose(f1);

  // set camera to center of map
  c_xpos = (TABSIZE/2)*tile_w;
  c_ypos = terrain[TABSIZE/2][TABSIZE/2].h*tile_h;
  c_zpos = (TABSIZE/2)*tile_l;
}

int main(int argc, char** argv)
{
  read_input(argv[1]);
  create_lod_map();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(640, 480);
  glutCreateWindow("Wyswietlanie terenu - Michal Karpinski");
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth( 1.0f );
	glutIgnoreKeyRepeat(true);
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);
	glutSpecialFunc(special_keys);
	glutSpecialUpFunc(special_keys_up);
	glutIdleFunc(game_function);
	initialize();
	glutMainLoop();
	return 0;
}
