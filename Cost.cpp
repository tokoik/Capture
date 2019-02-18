//
// マッチングコスト算出
//
#include "Cost.h"

// ジオメトリシェーダの invocations の値
#define INVOCATIONS 32

// 画像サイズ
#define IMAGE_RESOLUTION 640

// ずらす解像度
#define SHIFT_RESOLUTION 288

// ずらす範囲の割合
#define SHIFT_PERCENTAGE 90

// ずらす量の最大値
#define SHIFT_RANGE (1.5707963f * SHIFT_PERCENTAGE / 100)

// シフト回数
constexpr int shiftCount(SHIFT_RESOLUTION / INVOCATIONS);

// １シフト当たりにずらす量
constexpr GLfloat step(SHIFT_RANGE / shiftCount);

//
// コンストラクタ
//
Cost::Cost(int slices, int stacks, const char *vert, const char *frag, const char *geom)
  : Sphere(slices, stacks, vert, frag, geom)
  , shiftLoc(glGetUniformLocation(shader, "shift"))
  , stepLoc(glGetUniformLocation(shader, "step"))
  , scaleLoc(glGetUniformLocation(shader, "scale"))
  , offsetLoc(glGetUniformLocation(shader, "offset"))
{
  // フレームバッファオブジェクトとターゲットのテクスチャを二つ作る
  glGenTextures(2, texture);
  glGenFramebuffers(2, target);

  for (int i = 0; i < 2; ++i)
  {
    // マッチングコストを格納するレイヤーテクスチャ
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture[i]);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_R32F, IMAGE_RESOLUTION, IMAGE_RESOLUTION, SHIFT_RESOLUTION);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // テクスチャの境界色
    static constexpr GLfloat border[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, border);

    // マッチングコストのレンダリング先のフレームバッファオブジェクト
    glBindFramebuffer(GL_FRAMEBUFFER, target[i]);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture[i], 0);
  }

  // デフォルトのフレームバッファに戻す
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//
// デストラクタ
//
Cost::~Cost()
{
  // フレームバッファオブジェクトを削除する
  glDeleteFramebuffers(2, target);

  // レイヤーテクスチャを削除する
  glDeleteTextures(2, texture);
}

//
// 描画
//
void Cost::draw(const GgMatrix &mp, const GgMatrix &mv) const
{
  // シェーダプログラムの使用開始
  glUseProgram(shader);

  // uniform 変数を設定する
  glUniform2f(spacingLoc, 1.0f / slices, 1.0f / stacks);
  glUniform1i(color0Loc, 1);
  glUniform1i(color1Loc, 0);
  glUniform1f(stepLoc, step);
  glUniform2f(scaleLoc, 0.885f * 0.25f, -0.885f * 4.0f / 9.0f);


  // 隠面消去を無効にする
  glDisable(GL_DEPTH_TEST);

  // デプスバッファには描き込まない
  glDepthMask(GL_FALSE);

  // ビューポートをフレームバッファオブジェクトのサイズに合わせる
  glViewport(0, 0, IMAGE_RESOLUTION, IMAGE_RESOLUTION);

  // メッシュを描画する
  glBindVertexArray(object);
  glUniform2f(offsetLoc, 0.25f, 4.0f / 9.0f);
  glBindFramebuffer(GL_FRAMEBUFFER, target[0]);
  for (int shift = 0; shift < shiftCount; ++shift)
  {
    glUniform1i(shiftLoc, shift);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, (slices + 1) * 2, stacks);
  }
  glUniform2f(offsetLoc, 0.75f, 4.0f / 9.0f);
  glBindFramebuffer(GL_FRAMEBUFFER, target[1]);
  for (int shift = 0; shift < shiftCount; ++shift)
  {
    glUniform1i(shiftLoc, shift);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, (slices + 1) * 2, stacks);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // 隠面消去を有効にする
  glEnable(GL_DEPTH_TEST);

  // デプスバッファに描き込む
  glDepthMask(GL_TRUE);

  // 作成したマッチングコスト画像をテクスチャユニットに割り当てる
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texture[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D_ARRAY, texture[1]);
}
