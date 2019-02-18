#pragma once
#include "Mesh.h"

//
// レイヤーテクスチャの描画
//

class Test : public Mesh
{
  // レイヤーの深度の場所
  const GLint depthLoc;

public:

  // コンストラクタ
  Test(int slices = 1, int stacks = 1, const char *vert = "test.vert", const char *frag = "test.frag", const char *geom = nullptr);

  // デストラクタ
  virtual ~Test();

  // 描画
  virtual void draw(const GgMatrix &mp, const GgMatrix &mv, GLfloat depth) const;
  virtual void draw(const GgMatrix &mp, const GgMatrix &mv) { draw(mp, mv, 0.0f); }
};
