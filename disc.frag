#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// 光源
const vec4 lamb   = vec4(0.2, 0.2, 0.2, 1.0);       // 環境光成分の強度
const vec4 ldiff  = vec4(1.0, 1.0, 1.0, 0.0);       // 拡散反射成分の強度
const vec4 lspec  = vec4(1.0, 1.0, 1.0, 0.0);       // 鏡面反射成分の強度

// 材質
const vec4 kamb   = vec4(0.6, 0.6, 0.6, 1.0);       // 環境光の反射係数
const vec4 kdiff  = vec4(0.6, 0.6, 0.6, 1.0);       // 拡散反射係数
const vec4 kspec  = vec4(0.4, 0.4, 0.4, 1.0);       // 鏡面反射係数
const float kshi  = 40.0;                           // 輝き係数

// ラスタライザから受け取る頂点属性の補間値
in vec3 n;                                          // 補間された法線ベクトル
in vec3 l;                                          // 補間された光線ベクトル
in vec3 h;                                          // 補間された中間ベクトル
in vec2 tc0;                                        // 補間されたテクスチャ座標
in vec2 tc1;                                        // 補間されたテクスチャ座標

// 変位量
uniform float delta;

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

  vec4 c0[25], c1[25];

  c0[0] = texture(color0, tc0.xy);
  c1[0] = texture(color1, tc1.xy);

  c0[1] = textureOffset(color0, tc0.xy, ivec2(-2, -2));
  c1[1] = textureOffset(color1, tc1.xy, ivec2(-2, -2));
  c0[2] = textureOffset(color0, tc0.xy, ivec2(-1, -2));
  c1[2] = textureOffset(color1, tc1.xy, ivec2(-1, -2));
  c0[3] = textureOffset(color0, tc0.xy, ivec2( 0, -2));
  c1[3] = textureOffset(color1, tc1.xy, ivec2( 0, -2));
  c0[4] = textureOffset(color0, tc0.xy, ivec2( 1, -2));
  c1[4] = textureOffset(color1, tc1.xy, ivec2( 1, -2));
  c0[5] = textureOffset(color0, tc0.xy, ivec2( 2, -2));
  c1[5] = textureOffset(color1, tc1.xy, ivec2( 2, -2));
        
  c0[6] = textureOffset(color0, tc0.xy, ivec2(-2, -1));
  c1[6] = textureOffset(color1, tc1.xy, ivec2(-2, -1));
  c0[7] = textureOffset(color0, tc0.xy, ivec2(-1, -1));
  c1[7] = textureOffset(color1, tc1.xy, ivec2(-1, -1));
  c0[8] = textureOffset(color0, tc0.xy, ivec2( 0, -1));
  c1[8] = textureOffset(color1, tc1.xy, ivec2( 0, -1));
  c0[9] = textureOffset(color0, tc0.xy, ivec2( 1, -1));
  c1[9] = textureOffset(color1, tc1.xy, ivec2( 1, -1));
  c0[10] = textureOffset(color0, tc0.xy, ivec2( 2, -1));
  c1[10] = textureOffset(color1, tc1.xy, ivec2( 2, -1));
        
  c0[11] = textureOffset(color0, tc0.xy, ivec2(-2,  0));
  c1[11] = textureOffset(color1, tc1.xy, ivec2(-2,  0));
  c0[12] = textureOffset(color0, tc0.xy, ivec2(-1,  0));
  c1[12] = textureOffset(color1, tc1.xy, ivec2(-1,  0));
  c0[13] = textureOffset(color0, tc0.xy, ivec2( 1,  0));
  c1[13] = textureOffset(color1, tc1.xy, ivec2( 1,  0));
  c0[14] = textureOffset(color0, tc0.xy, ivec2( 2,  0));
  c1[14] = textureOffset(color1, tc1.xy, ivec2( 2,  0));
        
  c0[15] = textureOffset(color0, tc0.xy, ivec2(-2,  1));
  c1[15] = textureOffset(color1, tc1.xy, ivec2(-2,  1));
  c0[16] = textureOffset(color0, tc0.xy, ivec2(-1,  1));
  c1[16] = textureOffset(color1, tc1.xy, ivec2(-1,  1));
  c0[17] = textureOffset(color0, tc0.xy, ivec2( 0,  1));
  c1[17] = textureOffset(color1, tc1.xy, ivec2( 0,  1));
  c0[18] = textureOffset(color0, tc0.xy, ivec2( 1,  1));
  c1[18] = textureOffset(color1, tc1.xy, ivec2( 1,  1));
  c0[19] = textureOffset(color0, tc0.xy, ivec2( 2,  1));
  c1[19] = textureOffset(color1, tc1.xy, ivec2( 2,  1));
        
  c0[20] = textureOffset(color0, tc0.xy, ivec2(-2,  2));
  c1[20] = textureOffset(color1, tc1.xy, ivec2(-2,  2));
  c0[21] = textureOffset(color0, tc0.xy, ivec2(-1,  2));
  c1[21] = textureOffset(color1, tc1.xy, ivec2(-1,  2));
  c0[22] = textureOffset(color0, tc0.xy, ivec2( 0,  2));
  c1[22] = textureOffset(color1, tc1.xy, ivec2( 0,  2));
  c0[23] = textureOffset(color0, tc0.xy, ivec2( 1,  2));
  c1[23] = textureOffset(color1, tc1.xy, ivec2( 1,  2));
  c0[24] = textureOffset(color0, tc0.xy, ivec2( 2,  2));
  c1[24] = textureOffset(color1, tc1.xy, ivec2( 2,  2));

  float a0 = 0.0, a1 = 0.0, ac = 0.0;

  for (int i = 0; i < 25; ++i)
  {
    a0 += c0[i].r * c0[i].r + c0[i].g * c0[i].g + c0[i].b * c0[i].b;
    a1 += c1[i].r * c1[i].r + c1[i].g * c1[i].g + c1[i].b * c1[i].b;
    ac += c0[i].r * c1[i].r + c0[i].g * c1[i].g + c0[i].b * c1[i].b;
  }

  gl_FragDepth = ac / (sqrt(a0) * sqrt(a1)) * 0.5 + 0.5;
  fc = vec4(vec3(delta), 1.0);
}
