#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// ラスタライザから受け取る頂点属性の補間値
in vec2 tc0;                                        // 補間された基準のテクスチャ座標
in vec2 tc1;                                        // 補間された対象のテクスチャ座標

// テクスチャ座標のサンプラ
uniform sampler2D color0;                           // 標準のカラーマップ
uniform sampler2D color1;                           // 参照用のカラーマップ

// フレームバッファに出力するデータ
layout (location = 0) out vec4 fc;                  // フラグメントの色

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
