#version 430 core

// 光源
const vec4 pl = vec4(3.0, 4.0, 5.0, 1.0);           // 位置

// 図形の半径
uniform vec2 radius = vec2(1.0, 1.0);

// 図形の中心位置
uniform vec2 center = vec2(0.0, 0.0);

// テクスチャのスケール
uniform vec2 scale = vec2(0.885 * 0.25, 0.885 * 4.0 / 9.0);

// テクスチャの中心位置
uniform vec2 offset = vec2(0.0, 0.0);

// 変位量
uniform float delta;

// 格子間隔
uniform vec2 spacing = vec2(1.0 / 64.0, 1.0 / 32.0);

// 変換行列
uniform mat4 mv;                                    // モデルビュー変換行列
uniform mat4 mp;                                    // プロジェクション変換行列
uniform mat4 mg;                                    // 法線ベクトルの変換行列

// ラスタライザに送る頂点属性
out vec3 n;                                         // 頂点の法線ベクトル
out vec3 l;                                         // 光線ベクトル
out vec3 h;                                         // 中間ベクトル
out vec2 tc0;                                       // テクスチャ座標
out vec2 tc1;                                       // テクスチャ座標

void main(void)
{
	// 頂点位置
	//   各頂点において gl_VertexID が 0, 1, 2, 3, ... のように割り当てられるから、
	//     x = gl_VertexID >> 1 = 0, 0, 1, 1, 2, 2, 3, 3, ...
	//     y = gl_VertexID & 1  = 0, 1, 0, 1, 0, 1, 0, 1, ...
	//   のように GL_TRIANGLE_STRIP 向けの頂点座標値が得られる。
	//   y に gl_InstaceID を足せば glDrawArrayInstanced() のインスタンスごとに y が変化する。
	//   vec2(x, y) に格子間隔（格子点数 - 1 の逆数）spacing をかけた tv は [0, 1] になる。
	vec2 tv = vec2(gl_VertexID >> 1, (gl_VertexID & 1) + gl_InstanceID) * spacing;

  // それを経度θ∈[0, 2π], 緯度φ∈[0, π]に直す
  vec2 st = tv * vec2(-3.1415926, 3.1415926);

  // 極座標から直交座標に変換する
  vec2 sin_st = sin(st), cos_st = cos(st);
  vec2 xy = vec2(-sin_st.t * cos_st.s, cos_st.t);
  float z = sin_st.t * sin_st.s;

  // xy 平面上における頂点の中心からの距離の二乗
  float r = dot(xy, xy);
  if (r > 0.0) xy /= sqrt(r);

  // 正距方位図上の位置
  vec2 uv = 2.0f * acos(-z) * xy / 3.1415927;

  // テクスチャ座標
  tc0 = uv * scale + offset;

  // モデルビュー変換
  vec4 p = mv * vec4(uv * radius + center, 0.0, 1.0);

  // 投影変換
  gl_Position = mp * p;

  // 緯度をずらす
  st += vec2(0.0, delta);

  // 極座標から直交座標に変換する
  sin_st = sin(st), cos_st = cos(st);
  xy = vec2(-sin_st.t * cos_st.s, cos_st.t);
  z = sin_st.t * sin_st.s;

  // xy 平面上における頂点の中心からの距離の二乗
  r = dot(xy, xy);
  if (r > 0.0) xy /= sqrt(r);

  // 正距方位図上の位置
  uv = 2.0f * acos(-z) * xy / 3.1415927;

  // テクスチャ座標
  tc1 = uv * scale + offset;

  // 頂点の法線ベクトル
  vec3 nv = vec3(0.0, 0.0, 1.0);

  // 陰影計算の準備
  vec3 v = -normalize(vec3(p) / p.w);               // 視線ベクトル
  n = normalize(mat3(mg) * nv);                     // 法線ベクトル
  l = normalize((pl * p.w - p * pl.w).xyz);         // 光線ベクトル
  h = normalize(l + v);                             // 中間ベクトル
}
