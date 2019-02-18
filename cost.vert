#version 430 core

// 格子間隔
uniform vec2 spacing = vec2(1.0 / 32.0, 1.0 / 32.0);

// ジオメトリシェーダに送る頂点属性
out vec2 st;                                        // メッシュの極座標

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

  // それを経度θ∈[0, 2π], 緯度φ∈[0, π]に直す
  st = p * vec2(-3.1415926, 3.1415926);

  // 極座標から直交座標に変換する
  vec2 sin_st = sin(st), cos_st = cos(st);
  vec2 xy = vec2(-sin_st.t * cos_st.s, cos_st.t);
  float z = sin_st.t * sin_st.s;

  // xy 平面上における頂点の中心からの距離の二乗
  float r = dot(xy, xy);
  if (r > 0.0) xy /= sqrt(r);

  // 正距方位図上の位置
  gl_Position = vec4(2.0 * acos(-z) * xy / 3.1415927, 0.0, 1.0);
}
