//
// ���ʂ̃��b�V��
//
#include "Mesh.h"

//
// �R���X�g���N�^
//
Mesh::Mesh(int slices, int stacks, const char *vert, const char *frag, const char *geom)
  : shader(ggLoadShader(vert, frag, geom))
  , spacingLoc(glGetUniformLocation(shader, "spacing"))
  , mpLoc(glGetUniformLocation(shader, "mp"))
  , mvLoc(glGetUniformLocation(shader, "mv"))
  , mgLoc(glGetUniformLocation(shader, "mg"))
  , color0Loc(glGetUniformLocation(shader, "color0"))
  , color1Loc(glGetUniformLocation(shader, "color1"))
  , object([] { GLuint vao; glGenVertexArrays(1, &vao); return vao; } ())
  , slices(slices), stacks(stacks)
{
  if (shader == 0) throw std::runtime_error("Can't create the shader program object.");
}

//
// �f�X�g���N�^
//
Mesh::~Mesh()
{
  // �v���O�����I�u�W�F�N�g���폜����
  glDeleteProgram(shader);

  // ���_�z��I�u�W�F�N�g���폜����
  glDeleteVertexArrays(1, &object);
}

//
// �`��
//
void Mesh::draw(const GgMatrix &mp, const GgMatrix &mv) const
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

  // ���b�V����`�悷��
  glBindVertexArray(object);
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, (slices + 1) * 2, stacks);
}
