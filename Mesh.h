#pragma once
#include "gg.h"
using namespace gg;

//
// ���ʂ̃��b�V��
//

class Mesh
{
protected:

  // �V�F�[�_�̃v���O�����I�u�W�F�N�g��
  const GLuint shader;

  // �i�q�Ԋu�̏ꏊ
  const GLint spacingLoc;

  // �ϊ��s��̏ꏊ
  const GLint mpLoc, mvLoc, mgLoc;

  // �e�N�X�`���̃T���v���̏ꏊ
  const GLint color0Loc, color1Loc;

  // ���_�z��I�u�W�F�N�g
  const GLuint object;

  // ���b�V���̕�����
  const int slices, stacks;

public:

  // �R���X�g���N�^
  Mesh(int slices = 1, int stacks = 1, const char *vert = "mesh.vert", const char *frag = "mesh.frag", const char *geom = nullptr);

  // �R�s�[�R���X�g���N�^�𕕂���
  Mesh(const Mesh &mesh) = delete;

  // �f�X�g���N�^
  virtual ~Mesh();

  // ������Z�q�𕕂���
  Mesh &operator=(const Mesh &mesh) = delete;

  // �`��
  virtual void draw(const GgMatrix &mp, const GgMatrix &mv) const;
};
