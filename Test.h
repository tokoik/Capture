#pragma once
#include "Mesh.h"

//
// ���C���[�e�N�X�`���̕`��
//

class Test : public Mesh
{
  // ���C���[�̐[�x�̏ꏊ
  const GLint depthLoc;

public:

  // �R���X�g���N�^
  Test(int slices = 1, int stacks = 1, const char *vert = "test.vert", const char *frag = "test.frag", const char *geom = nullptr);

  // �f�X�g���N�^
  virtual ~Test();

  // �`��
  virtual void draw(const GgMatrix &mp, const GgMatrix &mv, GLfloat depth) const;
  virtual void draw(const GgMatrix &mp, const GgMatrix &mv) { draw(mp, mv, 0.0f); }
};
