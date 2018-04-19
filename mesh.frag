#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

//
// テクスチャ座標の位置の画素色をそのまま使う
//

// テクスチャ
uniform sampler2D image0, image1;

// テクスチャ座標
in vec2 texcoord0, texcoord1;

// フラグメントの色
layout (location = 0) out vec4 fc;

void main(void)
{
  // 画素の陰影を求める
  fc = texture(image0, texcoord0 * vec2(1.0, 2.0)) + texture(image1, texcoord1 * vec2(1.0, 2.0) - vec2(0.0, 1.0));
}
