#pragma once
#include "Mesh.h"

//
// ���̂̃��b�V��
//

class Sphere : public Mesh
{
public:

  // �R���X�g���N�^
  Sphere(int slices, int stacks, const char *vert = "sphere.vert", const char *frag = "sphere.frag", const char *geom = nullptr);

  // �f�X�g���N�^
  virtual ~Sphere();
};
