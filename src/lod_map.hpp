#pragma once

#include <GL/glut.h>
#include <vector>

class lod_map {
  int triangle_num;
  int vertices_num;
  GLfloat * vertex_array;
  GLfloat * color_array;
  bool dataLoaded;
  
  public:
    lod_map();
    ~lod_map();
    void initialize(std::vector<GLfloat>& verts, std::vector<GLfloat>& cols, int vnum, int tnum );
    void drawTerrain();
    int getTriangleCount() { return triangle_num; }
    int getVerticesCount() { return vertices_num; }
};
