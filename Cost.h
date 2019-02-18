#pragma once
#include "Sphere.h"

//
// マッチングコスト算出
//

class Cost : public Sphere
{
  // 反復数の場所
  const GLint shiftLoc;

  // 変位のステップ
  const GLint stepLoc;

  // テクスチャのスケールの場所
  const GLint scaleLoc;

  // テクスチャの中心位置の場所
  const GLint offsetLoc;

  // マッチングコストを格納するテクスチャ
  GLuint texture[2];

  // マッチングコストのレンダリング先
  GLuint target[2];

public:

  // コンストラクタ
  Cost(int slices, int stacks, const char *vert = "cost.vert", const char *frag = "cost.frag", const char *geom = "cost.geom");

  // デストラクタ
  virtual ~Cost();

  // 描画
  virtual void draw(const GgMatrix &mp, const GgMatrix &mv) const;
};
