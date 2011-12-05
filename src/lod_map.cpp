#include "lod_map.hpp"


lod_map::lod_map()
{
  dataLoaded = false;
}

lod_map::~lod_map()
{
  if(dataLoaded)
  {
    delete [] vertex_array;
    delete [] color_array;
  }
}

void lod_map::initialize(std::vector<GLfloat>& verts, std::vector<GLfloat>& cols, int vnum, int tnum )
{  
  vertex_array = new GLfloat[vnum];
  color_array = new GLfloat[vnum];

  // copy
  for(int i=0; i<vnum; i++){
    vertex_array[i] = verts[i];
    color_array[i] = cols[i];
  }

  vertices_num = vnum;
  triangle_num = tnum;

  dataLoaded = true;
}

void lod_map::drawTerrain()
{
  if(dataLoaded)
  {
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColorPointer(3, GL_FLOAT, 0, color_array);
    glVertexPointer(3, GL_FLOAT, 0, vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, vertices_num/3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
  }
}
