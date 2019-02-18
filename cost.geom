#version 150 core

layout(triangles, invocation = 200) in;
layout(triangle_strip, max_vertices = 3) out;

// �ψʗʂ͈̔�
uniform float range = 1.5707963 * 0.9;

// �e�N�X�`���̃X�P�[��
uniform vec2 scale = vec2(0.885 * 0.25, 0.885 * 4.0 / 9.0);

// �e�N�X�`���̒��S�ʒu
uniform vec2 offset = vec2(0.0, 0.0);

// �o�[�e�b�N�X�V�F�[�_����󂯎�钸�_����
in vec2 st[];                                       // ���b�V���̋ɍ��W

// ���X�^���C�U�ɑ��钸�_����
out vec2 tc0;                                       // ��̃e�N�X�`�����W
out vec2 tc1;                                       // �Ώۂ̃e�N�X�`�����W

void main(void)
{
  // �����_�����O���郌�C��
  gl_Layer = gl_InvocationID;

  // �ψʗ�
  float delta = gl_InvocationID * range;

  for (int i = 0; i < gl_in.length(); ++i)
  {
    // �������ʐ}��̈ʒu
    gl_Position = gl_in[i].gl_Position;

    // ��̃e�N�X�`�����W
    tc0 = gl_Position.xy * scale + offset;

    // �ܓx�����炷
    vec2 pq = st[i] + vec2(0.0, delta);

    // �ɍ��W���璼�����W�ɕϊ�����
    sin_pq = sin(pq), cos_pq = cos(pq);
    vec2 xy = vec2(-sin_pq.t * cos_pq.s, cos_pq.t);
    float z = sin_pq.t * sin_pq.s;

    // xy ���ʏ�ɂ����钸�_�̒��S����̋����̓��
    float r = dot(xy, xy);
    if (r > 0.0) xy /= sqrt(r);

    // �������ʐ}��̈ʒu
    vec2 uv = 2.0f * acos(-z) * xy / 3.1415927;

    // �Ώۂ̃e�N�X�`�����W
    tc1 = uv * scale + offset;
  }
}
