#version 430 core

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
uniform sampler2D color;                            // カラーマップ

// ラスタライザから受け取る頂点属性の補間値
in vec3 n;                                          // 補間された法線ベクトル
in vec3 l;                                          // 補間された光線ベクトル
in vec3 h;                                          // 補間された中間ベクトル
in vec4 tc;                                         // 補間されたテクスチャ座標

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

  vec4 c0 = texture(color, tc.xz);
  vec4 c1 = texture(color, tc.yz);
  float b = smoothstep(-0.02, 0.02, tc.w);
  vec4 c = mix(c0, c1, b);
  fc = (iamb + idiff) * c + ispec;
}
