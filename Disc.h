#pragma once
#include "Sphere.h"

//
// �~�Ղ̃��b�V��
//

class Disc : public Sphere
{
  // �}�`�̔��a�̏ꏊ
  const GLint radiusLoc;

  // �}�`�̒��S�ʒu�̏ꏊ
  const GLint centerLoc;

  // �e�N�X�`���̃X�P�[���̏ꏊ
  const GLint scaleLoc;

  // �e�N�X�`���̒��S�ʒu�̏ꏊ
  const GLint offsetLoc;

  // �ψʗʂ̏ꏊ
  const GLint deltaLoc;

public:

  // �R���X�g���N�^
  Disc(int slices, int stacks, const char *vert = "disc.vert", const char *frag = "discSAD.frag", const char *geom = nullptr);

  // �f�X�g���N�^
  virtual ~Disc();

  // �`��
  virtual void draw(const GgMatrix &mp, const GgMatrix &mv) const;
};
