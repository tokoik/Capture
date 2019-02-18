// アプリケーション本体
#include "GgApplication.h"

// ビデオキャプチャ
#include "Capture.h"

// 図形
#include "Disc.h"

// マッチングコスト
#include "Cost.h"

// テスト描画
#include "Test.h"

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

  // 図形描画用のシェーダプログラムを読み込む
  const Mesh mesh;
  const Sphere sphere(64, 32);
  const Disc disc(32, 32);
  const Test test;

  // コスト関数描画用のシェーダを読み込む
  const Cost cost(32, 23);

  // 球のビュー変換行列
  const GgMatrix mv(ggLookat(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

  // 背景のモデルビュー投影変換行列
  const GgMatrix mc(ggIdentity());

  // ビデオ入力
  Capture video0(1, 1280, 720), video1(2, 1280, 720);

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
    mesh.draw(mp, mv * window.getTrackball());
    //sphere.draw(mp, mv * window.getTrackball());
    //disc.draw(mp, mv * window.getTrackball());
    //cost.draw(mp, mv * window.getTrackball()); window.resetViewport(); test.draw(mp, mv * window.getTrackball(), t);

    // カラーバッファを入れ替えてイベントを取り出す
    window.swapBuffers();
  }
}
