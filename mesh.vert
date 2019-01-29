#version 150 core

//
// 視線の回転を行う
//

// テクスチャ
uniform sampler2D color0, color1;

// テクスチャのサイズ
vec2 size0 = vec2(textureSize(color0, 0));
vec2 size1 = vec2(textureSize(color1, 0));

// 格子間隔
uniform vec2 spacing = vec2(1.0 / 64.0, 1.0 / 32.0);

// 変換行列
uniform mat4 mv;                                    // 視野変換行列
uniform mat4 mp;                                    // 投影変換行列
uniform mat4 mg;                                    // 法線変換行列

// テクスチャ座標
out vec2 tc0, tc1;

void main(void)
{
	// 頂点位置
	//   各頂点において gl_VertexID が 0, 1, 2, 3, ... のように割り当てられるから、
	//     x = gl_VertexID >> 1 = 0, 0, 1, 1, 2, 2, 3, 3, ...
	//     y = gl_VertexID & 1  = 0, 1, 0, 1, 0, 1, 0, 1, ...
	//   のように GL_TRIANGLE_STRIP 向けの頂点座標値が得られる。
	//   y に gl_InstaceID を足せば glDrawArrayInstanced() のインスタンスごとに y が変化する。
	//   vec2(x, y) に格子間隔（格子点数 - 1 の逆数）spacing をかけた p は [0, 1] になる。
	vec2 p = vec2(gl_VertexID >> 1, (gl_VertexID & 1) + gl_InstanceID) * spacing;

	// 各画素の中央におけるテクスチャ座標
	tc0 = (size0 * p - p + 0.5) / size0;
	tc1 = (size1 * p - p + 0.5) / size1;

	// メッシュの頂点座標
	gl_Position = mp * mv * vec4(p * vec2(2.0, -2.0) - vec2(1.0, -1.0), 0.0, 1.0);
}
