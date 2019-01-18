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
