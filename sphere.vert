#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// 光源
uniform vec4 pl = vec4(3.0, 4.0, 5.0, 1.0);         // 位置

// 格子間隔
uniform vec2 spacing = vec2(1.0 / 64.0, 1.0 / 32.0);

// 変換行列
uniform mat4 mv;                                    // 視野変換行列
uniform mat4 mp;                                    // 投影変換行列
uniform mat4 mg;                                    // 法線変換行列

// ラスタライザに送る頂点属性
out vec3 n;                                         // 頂点の法線ベクトル
out vec3 l;                                         // 光線ベクトル
out vec3 h;                                         // 中間ベクトル
out vec4 tc;                                        // テクスチャ座標

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
  vec2 st = tv * vec2(-6.2831853, 3.1415926);

  // 極座標から直交座標に変換する
  vec2 sin_st = sin(st), cos_st = cos(st);
  vec3 nv = vec3(sin_st.t * cos_st.s, cos_st.t, sin_st.t * sin_st.s);
  vec4 pv = vec4(nv, 1.0);

  // モデルビュー変換
  vec4 p = mv * pv;                                 // 視点座標系の頂点の位置

  // 投影変換
  gl_Position = mp * p;

  // 陰影計算の準備
  vec3 v = -normalize(vec3(p) / p.w);               // 視線ベクトル
  n = normalize(mat3(mg) * nv);                     // 法線ベクトル
  l = normalize((pl * p.w - p * pl.w).xyz);         // 光線ベクトル
  h = normalize(l + v);                             // 中間ベクトル

  // 二重魚眼画像の中心位置とその点の z 値
  tc = vec4(0.25, 0.75, 4.0 / 9.0, pv.z);

  // xy 平面上における頂点の中心からの距離の二乗
  float r = dot(pv.xy, pv.xy);

  // もし頂点が xy 平面上の中心にあったら終わる
  if (r == 0.0) return;

  // 二重魚眼画像のテクスチャ座標を求める
  vec2 d = acos(abs(pv.z)) * pv.xy / (sqrt(r) * 1.775f);
  tc.xyz += vec3(-0.25 * d.x, 0.25 * d.x, -4.0 * d.y / 9.0);
}
