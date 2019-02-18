#include "Test.h"

//
// ���C���[�e�N�X�`���̕`��
//

//
// �R���X�g���N�^
//
Test::Test(int slices, int stacks, const char *vert, const char *frag, const char *geom)
  : Mesh(slices, stacks, vert, frag, geom)
  , depthLoc(glGetUniformLocation(shader, "depth"))
{
}

//
// �f�X�g���N�^
//
Test::~Test()
{
}

//
// �`��
//
void Test::draw(const GgMatrix &mp, const GgMatrix &mv, GLfloat depth) const
{
  // �V�F�[�_�v���O�����̎g�p�J�n
  glUseProgram(shader);

  // �@���ϊ��s��
  const GgMatrix mg(mv.normal());

  // uniform �ϐ���ݒ肷��
  glUniform2f(spacingLoc, 1.0f / slices, 1.0f / stacks);
  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, mv.get());
  glUniformMatrix4fv(mpLoc, 1, GL_FALSE, mp.get());
  glUniformMatrix4fv(mgLoc, 1, GL_FALSE, mg.get());
  glUniform1i(color0Loc, 0);
  glUniform1i(color1Loc, 1);

  // ���̃N���X�Ǝ��̐ݒ�
  glUniform1f(depthLoc, depth * 320.0);

  // ���b�V����`�悷��
  glBindVertexArray(object);
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, (slices + 1) * 2, stacks);
}
