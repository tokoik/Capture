#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// ���X�^���C�U����󂯎�钸�_�����̕�Ԓl
in vec2 tc0;                                        // ��Ԃ��ꂽ��̃e�N�X�`�����W
in vec2 tc1;                                        // ��Ԃ��ꂽ�Ώۂ̃e�N�X�`�����W

// �e�N�X�`�����W�̃T���v��
uniform sampler2D color0;                           // �W���̃J���[�}�b�v
uniform sampler2D color1;                           // �Q�Ɨp�̃J���[�}�b�v

// �t���[���o�b�t�@�ɏo�͂���f�[�^
layout (location = 0) out vec4 fc;                  // �t���O�����g�̐F

void main(void)
{
  vec4 d = abs(texture(color0, tc0.xy) - texture(color1, tc1.xy));

  d += abs(textureOffset(color0, tc0.xy, ivec2(-2, -2)) - textureOffset(color1, tc1.xy, ivec2(-2, -2)));
  d += abs(textureOffset(color0, tc0.xy, ivec2(-1, -2)) - textureOffset(color1, tc1.xy, ivec2(-1, -2)));
  d += abs(textureOffset(color0, tc0.xy, ivec2( 0, -2)) - textureOffset(color1, tc1.xy, ivec2( 0, -2)));
  d += abs(textureOffset(color0, tc0.xy, ivec2( 1, -2)) - textureOffset(color1, tc1.xy, ivec2( 1, -2)));
  d += abs(textureOffset(color0, tc0.xy, ivec2( 2, -2)) - textureOffset(color1, tc1.xy, ivec2( 2, -2)));

  d += abs(textureOffset(color0, tc0.xy, ivec2(-2, -1)) - textureOffset(color1, tc1.xy, ivec2(-2, -1)));
  d += abs(textureOffset(color0, tc0.xy, ivec2(-1, -1)) - textureOffset(color1, tc1.xy, ivec2(-1, -1)));
  d += abs(textureOffset(color0, tc0.xy, ivec2( 0, -1)) - textureOffset(color1, tc1.xy, ivec2( 0, -1)));
  d += abs(textureOffset(color0, tc0.xy, ivec2( 1, -1)) - textureOffset(color1, tc1.xy, ivec2( 1, -1)));
  d += abs(textureOffset(color0, tc0.xy, ivec2( 2, -1)) - textureOffset(color1, tc1.xy, ivec2( 2, -1)));

  d += abs(textureOffset(color0, tc0.xy, ivec2(-2,  0)) - textureOffset(color1, tc1.xy, ivec2(-2,  0)));
  d += abs(textureOffset(color0, tc0.xy, ivec2(-1,  0)) - textureOffset(color1, tc1.xy, ivec2(-1,  0)));
  d += abs(textureOffset(color0, tc0.xy, ivec2( 1,  0)) - textureOffset(color1, tc1.xy, ivec2( 1,  0)));
  d += abs(textureOffset(color0, tc0.xy, ivec2( 2,  0)) - textureOffset(color1, tc1.xy, ivec2( 2,  0)));

  d += abs(textureOffset(color1, tc1.xy, ivec2(-2,  1)) - textureOffset(color0, tc0.xy, ivec2(-2,  1)));
  d += abs(textureOffset(color0, tc0.xy, ivec2(-1,  1)) - textureOffset(color1, tc1.xy, ivec2(-1,  1)));
  d += abs(textureOffset(color0, tc0.xy, ivec2( 0,  1)) - textureOffset(color1, tc1.xy, ivec2( 0,  1)));
  d += abs(textureOffset(color0, tc0.xy, ivec2( 1,  1)) - textureOffset(color1, tc1.xy, ivec2( 1,  1)));
  d += abs(textureOffset(color0, tc0.xy, ivec2( 2,  1)) - textureOffset(color1, tc1.xy, ivec2( 2,  1)));

  d += abs(textureOffset(color0, tc0.xy, ivec2(-2,  2)) - textureOffset(color1, tc1.xy, ivec2(-2,  2)));
  d += abs(textureOffset(color0, tc0.xy, ivec2(-1,  2)) - textureOffset(color1, tc1.xy, ivec2(-1,  2)));
  d += abs(textureOffset(color0, tc0.xy, ivec2( 0,  2)) - textureOffset(color1, tc1.xy, ivec2( 0,  2)));
  d += abs(textureOffset(color0, tc0.xy, ivec2( 1,  2)) - textureOffset(color1, tc1.xy, ivec2( 1,  2)));
  d += abs(textureOffset(color0, tc0.xy, ivec2( 2,  2)) - textureOffset(color1, tc1.xy, ivec2( 2,  2)));

  fc.r = (d.r + d.g + d.b) * 0.013333333;
}
