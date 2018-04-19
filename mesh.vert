#version 150 core

//
// 視線の回転を行う
//

// テクスチャ
uniform sampler2D image;

// テクスチャのサイズ
vec2 size = vec2(textureSize(image, 0));

// 格子間隔（格子点数の逆数）
uniform vec2 spacing;

// テクスチャ座標
out vec2 texcoord;

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
	texcoord = (size * p - p + 0.5) / size;

	// メッシュの頂点座標
	gl_Position = vec4(p * vec2(2.0, -2.0) - vec2(1.0, -1.0), 0.0, 1.0);
}
