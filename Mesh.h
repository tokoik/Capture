#pragma once
#include "gg.h"
using namespace gg;

//
// 平面のメッシュ
//

class Mesh
{
protected:

  // シェーダのプログラムオブジェクト名
  const GLuint shader;

  // 格子間隔の場所
  const GLint spacingLoc;

  // 変換行列の場所
  const GLint mpLoc, mvLoc, mgLoc;

  // テクスチャのサンプラの場所
  const GLint color0Loc, color1Loc;

  // 頂点配列オブジェクト
  const GLuint object;

  // メッシュの分割数
  const int slices, stacks;

public:

  // コンストラクタ
  Mesh(int slices = 1, int stacks = 1, const char *vert = "mesh.vert", const char *frag = "mesh.frag", const char *geom = nullptr);

  // コピーコンストラクタを封じる
  Mesh(const Mesh &mesh) = delete;

  // デストラクタ
  virtual ~Mesh();

  // 代入演算子を封じる
  Mesh &operator=(const Mesh &mesh) = delete;

  // 描画
  virtual void draw(const GgMatrix &mp, const GgMatrix &mv) const;
};
