#version 430 core

//
// テクスチャ座標の位置の画素色をそのまま使う
//

// テクスチャ
uniform sampler2DArray color0, color1;

// テクスチャ座標
in vec3 tc0, tc1;

// フラグメントの色
layout (location = 0) out vec4 fc;

void main(void)
{
	// 画素の陰影を求める
	fc = texture(color0, tc0) + texture(color1, tc1);
}
