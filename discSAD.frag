#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// ����
const vec4 lamb   = vec4(0.2, 0.2, 0.2, 1.0);       // ���������̋��x
const vec4 ldiff  = vec4(1.0, 1.0, 1.0, 0.0);       // �g�U���ː����̋��x
const vec4 lspec  = vec4(1.0, 1.0, 1.0, 0.0);       // ���ʔ��ː����̋��x

// �ގ�
const vec4 kamb   = vec4(0.6, 0.6, 0.6, 1.0);       // �����̔��ˌW��
const vec4 kdiff  = vec4(0.6, 0.6, 0.6, 1.0);       // �g�U���ˌW��
const vec4 kspec  = vec4(0.4, 0.4, 0.4, 1.0);       // ���ʔ��ˌW��
const float kshi  = 40.0;                           // �P���W��

// ���X�^���C�U����󂯎�钸�_�����̕�Ԓl
in vec3 n;                                          // ��Ԃ��ꂽ�@���x�N�g��
in vec3 l;                                          // ��Ԃ��ꂽ�����x�N�g��
in vec3 h;                                          // ��Ԃ��ꂽ���ԃx�N�g��
in vec2 tc0;                                        // ��Ԃ��ꂽ�e�N�X�`�����W
in vec2 tc1;                                        // ��Ԃ��ꂽ�e�N�X�`�����W

// �ψʗ�
uniform float delta;

// �e�N�X�`�����W�̃T���v��
uniform sampler2D color0;                           // �W���̃J���[�}�b�v
uniform sampler2D color1;                           // �Q�Ɨp�̃J���[�}�b�v

// �t���[���o�b�t�@�ɏo�͂���f�[�^
layout (location = 0) out vec4 fc;                  // �t���O�����g�̐F

void main(void)
{
  vec3 nn = normalize(n);                           // �@���x�N�g��
  vec3 nl = normalize(l);                           // �����x�N�g��
  vec3 nh = normalize(h);                           // ���ԃx�N�g��

  vec4 iamb = kamb * lamb;
  vec4 idiff = max(dot(nn, nl), 0.0) * kdiff * ldiff;
  vec4 ispec = pow(max(dot(nn, nh), 0.0), kshi) * kspec * lspec;

  fc = vec4(vec3(delta), 1.0);

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

  gl_FragDepth = (d.r + d.g + d.b) * 0.013333333;
}
