#include "Test.h"

//
// レイヤーテクスチャの描画
//

//
// コンストラクタ
//
Test::Test(int slices, int stacks, const char *vert, const char *frag, const char *geom)
  : Mesh(slices, stacks, vert, frag, geom)
  , depthLoc(glGetUniformLocation(shader, "depth"))
{
}

//
// デストラクタ
//
Test::~Test()
{
}

//
// 描画
//
void Test::draw(const GgMatrix &mp, const GgMatrix &mv, GLfloat depth) const
{
  // シェーダプログラムの使用開始
  glUseProgram(shader);

  // 法線変換行列
  const GgMatrix mg(mv.normal());

  // uniform 変数を設定する
  glUniform2f(spacingLoc, 1.0f / slices, 1.0f / stacks);
  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, mv.get());
  glUniformMatrix4fv(mpLoc, 1, GL_FALSE, mp.get());
  glUniformMatrix4fv(mgLoc, 1, GL_FALSE, mg.get());
  glUniform1i(color0Loc, 0);
  glUniform1i(color1Loc, 1);

  // このクラス独自の設定
  glUniform1f(depthLoc, depth * 320.0);

  // メッシュを描画する
  glBindVertexArray(object);
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, (slices + 1) * 2, stacks);
}
