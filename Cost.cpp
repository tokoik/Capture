//
// �}�b�`���O�R�X�g�Z�o
//
#include "Cost.h"

// �W�I���g���V�F�[�_�� invocations �̒l
#define INVOCATIONS 32

// �摜�T�C�Y
#define IMAGE_RESOLUTION 640

// ���炷�𑜓x
#define SHIFT_RESOLUTION 288

// ���炷�͈͂̊���
#define SHIFT_PERCENTAGE 90

// ���炷�ʂ̍ő�l
#define SHIFT_RANGE (1.5707963f * SHIFT_PERCENTAGE / 100)

// �V�t�g��
constexpr int shiftCount(SHIFT_RESOLUTION / INVOCATIONS);

// �P�V�t�g������ɂ��炷��
constexpr GLfloat step(SHIFT_RANGE / shiftCount);

//
// �R���X�g���N�^
//
Cost::Cost(int slices, int stacks, const char *vert, const char *frag, const char *geom)
  : Sphere(slices, stacks, vert, frag, geom)
  , shiftLoc(glGetUniformLocation(shader, "shift"))
  , stepLoc(glGetUniformLocation(shader, "step"))
  , scaleLoc(glGetUniformLocation(shader, "scale"))
  , offsetLoc(glGetUniformLocation(shader, "offset"))
{
  // �t���[���o�b�t�@�I�u�W�F�N�g�ƃ^�[�Q�b�g�̃e�N�X�`�������
  glGenTextures(2, texture);
  glGenFramebuffers(2, target);

  for (int i = 0; i < 2; ++i)
  {
    // �}�b�`���O�R�X�g���i�[���郌�C���[�e�N�X�`��
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture[i]);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_R32F, IMAGE_RESOLUTION, IMAGE_RESOLUTION, SHIFT_RESOLUTION);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // �e�N�X�`���̋��E�F
    static constexpr GLfloat border[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, border);

    // �}�b�`���O�R�X�g�̃����_�����O��̃t���[���o�b�t�@�I�u�W�F�N�g
    glBindFramebuffer(GL_FRAMEBUFFER, target[i]);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture[i], 0);
  }

  // �f�t�H���g�̃t���[���o�b�t�@�ɖ߂�
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//
// �f�X�g���N�^
//
Cost::~Cost()
{
  // �t���[���o�b�t�@�I�u�W�F�N�g���폜����
  glDeleteFramebuffers(2, target);

  // ���C���[�e�N�X�`�����폜����
  glDeleteTextures(2, texture);
}

//
// �`��
//
void Cost::draw(const GgMatrix &mp, const GgMatrix &mv) const
{
  // �V�F�[�_�v���O�����̎g�p�J�n
  glUseProgram(shader);

  // uniform �ϐ���ݒ肷��
  glUniform2f(spacingLoc, 1.0f / slices, 1.0f / stacks);
  glUniform1i(color0Loc, 1);
  glUniform1i(color1Loc, 0);
  glUniform1f(stepLoc, step);
  glUniform2f(scaleLoc, 0.885f * 0.25f, -0.885f * 4.0f / 9.0f);


  // �B�ʏ����𖳌��ɂ���
  glDisable(GL_DEPTH_TEST);

  // �f�v�X�o�b�t�@�ɂ͕`�����܂Ȃ�
  glDepthMask(GL_FALSE);

  // �r���[�|�[�g���t���[���o�b�t�@�I�u�W�F�N�g�̃T�C�Y�ɍ��킹��
  glViewport(0, 0, IMAGE_RESOLUTION, IMAGE_RESOLUTION);

  // ���b�V����`�悷��
  glBindVertexArray(object);
  glUniform2f(offsetLoc, 0.25f, 4.0f / 9.0f);
  glBindFramebuffer(GL_FRAMEBUFFER, target[0]);
  for (int shift = 0; shift < shiftCount; ++shift)
  {
    glUniform1i(shiftLoc, shift);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, (slices + 1) * 2, stacks);
  }
  glUniform2f(offsetLoc, 0.75f, 4.0f / 9.0f);
  glBindFramebuffer(GL_FRAMEBUFFER, target[1]);
  for (int shift = 0; shift < shiftCount; ++shift)
  {
    glUniform1i(shiftLoc, shift);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, (slices + 1) * 2, stacks);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // �B�ʏ�����L���ɂ���
  glEnable(GL_DEPTH_TEST);

  // �f�v�X�o�b�t�@�ɕ`������
  glDepthMask(GL_TRUE);

  // �쐬�����}�b�`���O�R�X�g�摜���e�N�X�`�����j�b�g�Ɋ��蓖�Ă�
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texture[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texture[1]);
}
