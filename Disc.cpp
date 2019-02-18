//
// 円盤のメッシュ
//
#include "Disc.h"

// 繰り返しのフレーム数
constexpr int frames(static_cast<int>(320 * 0.9f));

// ずらす量の最大値
constexpr GLfloat range(1.5707963f * 0.9f);

//
// コンストラクタ
//
Disc::Disc(int slices, int stacks, const char *vert, const char *frag, const char *geom)
  : Sphere(slices, stacks, vert, frag, geom)
  , radiusLoc(glGetUniformLocation(shader, "radius"))
  , centerLoc(glGetUniformLocation(shader, "center"))
  , scaleLoc(glGetUniformLocation(shader, "scale"))
  , offsetLoc(glGetUniformLocation(shader, "offset"))
  , deltaLoc(glGetUniformLocation(shader, "delta"))
{
}

//
// デストラクタ
//
Disc::~Disc()
{
}

//
// 描画
//
void Disc::draw(const GgMatrix &mp, const GgMatrix &mv) const
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
  glUniform2f(radiusLoc, 0.5f, 0.5f);
  glUniform2f(scaleLoc, 0.885f * 0.25f, -0.885f * 4.0f / 9.0f);

  // メッシュを描画する
  glBindVertexArray(object);
  for (int frame = 0; frame < frames; ++frame)
  {
    glUniform1f(deltaLoc, frame * range / frames);
    glUniform2f(centerLoc, -0.5f, 0.0f);
    glUniform2f(offsetLoc, 0.25f, 4.0f / 9.0f);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, (slices + 1) * 2, stacks);
    glUniform2f(centerLoc, 0.5f, 0.0f);
    glUniform2f(offsetLoc, 0.75f, 4.0f / 9.0f);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, (slices + 1) * 2, stacks);
  }
}
