#version 430 core

#define INVOCATIONS 32

layout(triangles, invocations = INVOCATIONS) in;
layout(triangle_strip, max_vertices = 3) out;

// 反復数
uniform int shift = 0;

// 変位のステップ
uniform float step = 1.5707963 / 320;

// テクスチャのスケール
uniform vec2 scale = vec2(0.885 * 0.25, 0.885 * 4.0 / 9.0);

// テクスチャの中心位置
uniform vec2 offset = vec2(0.0, 0.0);

// バーテックスシェーダから受け取る頂点属性
in vec2 st[];                                       // メッシュの極座標

// ラスタライザに送る頂点属性
out vec2 tc0;                                       // 基準のテクスチャ座標
out vec2 tc1;                                       // 対象のテクスチャ座標
out float delta;

void main(void)
{
  // レンダリングするレイヤ
  gl_Layer = gl_InvocationID + shift * INVOCATIONS;

  // 変位量
  delta = gl_Layer * step / INVOCATIONS;

  for (int i = 0; i < gl_in.length(); ++i)
  {
    // 正距方位図上の位置
    gl_Position = gl_in[i].gl_Position;

    // 基準のテクスチャ座標
    tc0 = gl_Position.xy * scale + offset;

    // 緯度をずらす
    vec2 pq = st[i] + vec2(0.0, delta);

    // 極座標から直交座標に変換する
    vec2 sin_pq = sin(pq), cos_pq = cos(pq);
    vec2 xy = vec2(-sin_pq.t * cos_pq.s, cos_pq.t);
    float z = sin_pq.t * sin_pq.s;

    // xy 平面上における頂点の中心からの距離の二乗
    float r = dot(xy, xy);
    if (r > 0.0) xy /= sqrt(r);

    // 正距方位図上の位置
    vec2 uv = 2.0f * acos(-z) * xy / 3.1415927;

    // 対象のテクスチャ座標
    tc1 = uv * scale + offset;

    // 頂点データを生成する
    EmitVertex();
  }

  // 図形データ終了
  EndPrimitive();
}
