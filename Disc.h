#pragma once
#include "Sphere.h"

//
// 円盤のメッシュ
//

class Disc : public Sphere
{
  // 図形の半径の場所
  const GLint radiusLoc;

  // 図形の中心位置の場所
  const GLint centerLoc;

  // テクスチャのスケールの場所
  const GLint scaleLoc;

  // テクスチャの中心位置の場所
  const GLint offsetLoc;

  // 変位量の場所
  const GLint deltaLoc;

public:

  // コンストラクタ
  Disc(int slices, int stacks, const char *vert = "disc.vert", const char *frag = "discSAD.frag", const char *geom = nullptr);

  // デストラクタ
  virtual ~Disc();

  // 描画
  virtual void draw(const GgMatrix &mp, const GgMatrix &mv) const;
};
