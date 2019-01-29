#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// 変位量
uniform float delta;

// 光源
uniform vec4 lamb   = vec4(0.2, 0.2, 0.2, 1.0);     // 環境光成分の強度
uniform vec4 ldiff  = vec4(1.0, 1.0, 1.0, 0.0);     // 拡散反射成分の強度
uniform vec4 lspec  = vec4(1.0, 1.0, 1.0, 0.0);     // 鏡面反射成分の強度

// 材質
uniform vec4 kamb   = vec4(0.6, 0.6, 0.6, 1.0);     // 環境光の反射係数
uniform vec4 kdiff  = vec4(0.6, 0.6, 0.6, 1.0);     // 拡散反射係数
uniform vec4 kspec  = vec4(0.4, 0.4, 0.4, 1.0);     // 鏡面反射係数
uniform float kshi  = 40.0;                         // 輝き係数

// テクスチャ座標のサンプラ
uniform sampler2D color0;                           // 標準のカラーマップ
uniform sampler2D color1;                           // 参照用のカラーマップ

// フレームバッファに出力するデータ
layout (location = 0) out vec4 fc;                  // フラグメントの色

void main(void)
{
  vec3 nn = normalize(n);                           // 法線ベクトル
  vec3 nl = normalize(l);                           // 光線ベクトル
  vec3 nh = normalize(h);                           // 中間ベクトル

  vec4 iamb = kamb * lamb;
  vec4 idiff = max(dot(nn, nl), 0.0) * kdiff * ldiff;
  vec4 ispec = pow(max(dot(nn, nh), 0.0), kshi) * kspec * lspec;

  fc = vec4(vec3(delta), 1.0);

  vec4 c0 = texture(color0, tc0.xy);
  vec4 c1 = texture(color1, tc1.xy);

  //fc = abs(c1 - c0);
  //return;

  vec4 d0[24];

  d0[0] = textureOffset(color0, tc0.xy, ivec2(-2, -2)) - c0;
  d0[1] = textureOffset(color0, tc0.xy, ivec2(-1, -2)) - c0;
  d0[2] = textureOffset(color0, tc0.xy, ivec2( 0, -2)) - c0;
  d0[3] = textureOffset(color0, tc0.xy, ivec2( 1, -2)) - c0;
  d0[4] = textureOffset(color0, tc0.xy, ivec2( 2, -2)) - c0;

  d0[5] = textureOffset(color0, tc0.xy, ivec2(-2, -1)) - c0;
  d0[6] = textureOffset(color0, tc0.xy, ivec2(-1, -1)) - c0;
  d0[7] = textureOffset(color0, tc0.xy, ivec2( 0, -1)) - c0;
  d0[8] = textureOffset(color0, tc0.xy, ivec2( 1, -1)) - c0;
  d0[9] = textureOffset(color0, tc0.xy, ivec2( 2, -1)) - c0;

  d0[10] = textureOffset(color0, tc0.xy, ivec2(-2,  0)) - c0;
  d0[11] = textureOffset(color0, tc0.xy, ivec2(-1,  0)) - c0;
  d0[12] = textureOffset(color0, tc0.xy, ivec2( 1,  0)) - c0;
  d0[13] = textureOffset(color0, tc0.xy, ivec2( 2,  0)) - c0;

  d0[14] = textureOffset(color1, tc1.xy, ivec2(-2,  1)) - c0;
  d0[15] = textureOffset(color0, tc0.xy, ivec2(-1,  1)) - c0;
  d0[16] = textureOffset(color0, tc0.xy, ivec2( 0,  1)) - c0;
  d0[17] = textureOffset(color0, tc0.xy, ivec2( 1,  1)) - c0;
  d0[18] = textureOffset(color0, tc0.xy, ivec2( 2,  1)) - c0;

  d0[19] = textureOffset(color0, tc0.xy, ivec2(-2,  2)) - c0;
  d0[20] = textureOffset(color0, tc0.xy, ivec2(-1,  2)) - c0;
  d0[21] = textureOffset(color0, tc0.xy, ivec2( 0,  2)) - c0;
  d0[22] = textureOffset(color0, tc0.xy, ivec2( 1,  2)) - c0;
  d0[23] = textureOffset(color0, tc0.xy, ivec2( 2,  2)) - c0;

  vec4 d1[24];

  d1[0] = textureOffset(color1, tc1.xy, ivec2(-2, -2)) - c1;
  d1[1] = textureOffset(color1, tc1.xy, ivec2(-1, -2)) - c1;
  d1[2] = textureOffset(color1, tc1.xy, ivec2( 0, -2)) - c1;
  d1[3] = textureOffset(color1, tc1.xy, ivec2( 1, -2)) - c1;
  d1[4] = textureOffset(color1, tc1.xy, ivec2( 2, -2)) - c1;

  d1[5] = textureOffset(color1, tc1.xy, ivec2(-2, -1)) - c1;
  d1[6] = textureOffset(color1, tc1.xy, ivec2(-1, -1)) - c1;
  d1[7] = textureOffset(color1, tc1.xy, ivec2( 0, -1)) - c1;
  d1[8] = textureOffset(color1, tc1.xy, ivec2( 1, -1)) - c1;
  d1[9] = textureOffset(color1, tc1.xy, ivec2( 2, -1)) - c1;

  d1[10] = textureOffset(color1, tc1.xy, ivec2(-2,  0)) - c1;
  d1[11] = textureOffset(color1, tc1.xy, ivec2(-1,  0)) - c1;
  d1[12] = textureOffset(color1, tc1.xy, ivec2( 1,  0)) - c1;
  d1[13] = textureOffset(color1, tc1.xy, ivec2( 2,  0)) - c1;

  d1[14] = textureOffset(color0, tc0.xy, ivec2(-2,  1)) - c1;
  d1[15] = textureOffset(color1, tc1.xy, ivec2(-1,  1)) - c1;
  d1[16] = textureOffset(color1, tc1.xy, ivec2( 0,  1)) - c1;
  d1[17] = textureOffset(color1, tc1.xy, ivec2( 1,  1)) - c1;
  d1[18] = textureOffset(color1, tc1.xy, ivec2( 2,  1)) - c1;

  d1[19] = textureOffset(color1, tc1.xy, ivec2(-2,  2)) - c1;
  d1[20] = textureOffset(color1, tc1.xy, ivec2(-1,  2)) - c1;
  d1[21] = textureOffset(color1, tc1.xy, ivec2( 0,  2)) - c1;
  d1[22] = textureOffset(color1, tc1.xy, ivec2( 1,  2)) - c1;
  d1[23] = textureOffset(color1, tc1.xy, ivec2( 2,  2)) - c1;

  float a0 = 0.0, a1 = 0.0, ac = 0.0;

  for (int i = 0; i < 24; ++i)
  {
    a0 += d0[i].r * d0[i].r + d0[i].g * d0[i].g + d0[i].b * d0[i].b;
    a1 += d1[i].r * d1[i].r + d1[i].g * d1[i].g + d1[i].b * d1[i].b;
    ac += d0[i].r * d1[i].r + d0[i].g * d1[i].g + d0[i].b * d1[i].b;
  }

  gl_FragDepth = 1.0 - max(ac / (sqrt(a0 * a1)), 0.0);
}
