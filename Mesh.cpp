//
// 平面のメッシュ
//
#include "Mesh.h"

//
// コンストラクタ
//
Mesh::Mesh(int slices, int stacks, const char *vert, const char *frag, const char *geom)
  : shader(ggLoadShader(vert, frag, geom))
  , spacingLoc(glGetUniformLocation(shader, "spacing"))
  , mpLoc(glGetUniformLocation(shader, "mp"))
  , mvLoc(glGetUniformLocation(shader, "mv"))
  , mgLoc(glGetUniformLocation(shader, "mg"))
  , color0Loc(glGetUniformLocation(shader, "color0"))
  , color1Loc(glGetUniformLocation(shader, "color1"))
  , object([] { GLuint vao; glGenVertexArrays(1, &vao); return vao; } ())
  , slices(slices), stacks(stacks)
{
  if (shader == 0) throw std::runtime_error("Can't create the shader program object.");
}

//
// デストラクタ
//
Mesh::~Mesh()
{
  // プログラムオブジェクトを削除する
  glDeleteProgram(shader);

  // 頂点配列オブジェクトを削除する
  glDeleteVertexArrays(1, &object);
}

//
// 描画
//
void Mesh::draw(const GgMatrix &mp, const GgMatrix &mv) const
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

  // メッシュを描画する
  glBindVertexArray(object);
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, (slices + 1) * 2, stacks);
}
