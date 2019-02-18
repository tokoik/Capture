#pragma once
#include "Sphere.h"

//
// �}�b�`���O�R�X�g�Z�o
//

class Cost : public Sphere
{
  // �������̏ꏊ
  const GLint shiftLoc;

  // �ψʂ̃X�e�b�v
  const GLint stepLoc;

  // �e�N�X�`���̃X�P�[���̏ꏊ
  const GLint scaleLoc;

  // �e�N�X�`���̒��S�ʒu�̏ꏊ
  const GLint offsetLoc;

  // �}�b�`���O�R�X�g���i�[����e�N�X�`��
  GLuint texture[2];

  // �}�b�`���O�R�X�g�̃����_�����O��
  GLuint target[2];

public:

  // �R���X�g���N�^
  Cost(int slices, int stacks, const char *vert = "cost.vert", const char *frag = "cost.frag", const char *geom = "cost.geom");

  // �f�X�g���N�^
  virtual ~Cost();

  // �`��
  virtual void draw(const GgMatrix &mp, const GgMatrix &mv) const;
};
