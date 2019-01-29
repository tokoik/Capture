// アプリケーション本体
#include "GgApplication.h"

// ビデオキャプチャ
#include "Capture.h"

// 図形
#include "Disc.h"

// 標準ライブラリ
#include <array>
#include <vector>

// ビューポートのサイズ
constexpr int width(800), height(450);

// アニメーションの周期（秒）
constexpr double cycle(5.0);

//
// アプリケーションの実行
//
void GgApplication::run()
{
  // ウィンドウを作成する
  Window window("Capture", width, height * 2);

  // 背景色を指定する
  glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

  // 隠面消去を有効にする
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  // 球体描画用のシェーダプログラムを読み込む
  //const Mesh *const mesh(new Mesh);
  //const Mesh *const mesh(new Sphere(64, 32));
  const Mesh *const mesh(new Disc(32, 32));

  // 球のビュー変換行列
  const GgMatrix mv(ggLookat(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

  // マッチングコストを格納するテクスチャ
  GLuint cost;
  glGenTextures(1, &cost);
  glBindTexture(GL_TEXTURE_2D_ARRAY, cost);
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 4, GL_R32F, 640, 640, 320);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // マッチングコストのレンダリング先のフレームバッファオブジェクト
  GLuint fb;
  glGenFramebuffers(1, &fb);
  glBindFramebuffer(GL_FRAMEBUFFER, fb);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fb, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // 背景のモデルビュー投影変換行列
  const GgMatrix mc(ggIdentity());

  // ビデオ入力
  Capture video0(2, 1280, 720), video1(0, 1280, 720);

  // 経過時間のリセット
  glfwSetTime(0.0);

  // ウィンドウが開いている間繰り返す
  while (window)
  {
    // 時刻の計測
    const float t(static_cast<float>(fmod(glfwGetTime(), cycle) / cycle));

    //
    // ビデオのキャプチャ
    //

    // 一つ目のカメラからビデオをキャプチャしてテクスチャユニット 0 に入れる
    glActiveTexture(GL_TEXTURE0);
    video0.capture();

    // 二つ目のカメラからビデオをキャプチャしてテクスチャユニット 1 に入れる
    glActiveTexture(GL_TEXTURE1);
    video1.capture();

    //
    // 図形の描画
    //

  // 画面を消去する
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 投影変換行列
    const GgMatrix mp(ggPerspective(0.5f, window.getAspect(), 1.0f, 15.0f));

    // 図形の描画
    mesh->draw(mp, mv * window.getTrackball());

    // カラーバッファを入れ替えてイベントを取り出す
    window.swapBuffers();
  }
}
