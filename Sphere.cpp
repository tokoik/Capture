//
// 球体のメッシュ
//
#include "Sphere.h"

//
// コンストラクタ
//
Sphere::Sphere(int slices, int stacks, const char *vert, const char *frag, const char *geom)
  : Mesh(slices, stacks, vert, frag, geom)
{
}

//
// デストラクタ
//
Sphere::~Sphere()
{
}
