#pragma once
#include "Mesh.h"

//
// 球体のメッシュ
//

class Sphere : public Mesh
{
public:

  // コンストラクタ
  Sphere(int slices, int stacks, const char *vert = "sphere.vert", const char *frag = "sphere.frag", const char *geom = nullptr);

  // デストラクタ
  virtual ~Sphere();
};
