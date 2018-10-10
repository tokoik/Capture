// アプリケーション本体
#include "GgApplication.h"

// ビデオキャプチャ
#include "Capture.h"

// メッシュの高さと幅
constexpr int slices(1), stacks(1);

// ビューポートのサイズ
constexpr int width(800), height(450);

//
// 球のデータの作成
//
static void makeSphere(float radius, int slices, int stacks,
  GLfloat(*pv)[3], GLfloat(*nv)[3], GLfloat(*tv)[2], GLuint(*f)[3])
{
  // 頂点の位置とテクスチャ座標を求める
  for (int k = 0, j = 0; j <= stacks; ++j)
  {
    const float t(static_cast<float>(j) / static_cast<float>(stacks));
    const float ph(3.141593f * t);
    const float y(cos(ph));
    const float r(sin(ph));

    for (int i = 0; i <= slices; ++i)
    {
      const float s(static_cast<float>(i) / static_cast<float>(slices));
      const float th(-2.0f * 3.141593f * s);
      const float x(r * cos(th));
      const float z(r * sin(th));

      // 頂点の座標値
      pv[k][0] = x * radius;
      pv[k][1] = y * radius;
      pv[k][2] = z * radius;

      // 頂点の法線ベクトル
      nv[k][0] = x;
      nv[k][1] = y;
      nv[k][2] = z;

      // 頂点のテクスチャ座標値
      tv[k][0] = s;
      tv[k][1] = t;

      ++k;
    }
  }
}

//
// アプリケーションの実行
//
void GgApplication::run()
{
	// ウィンドウを作成する
	Window window("Capture", width, height * 2);

	// 背景描画用のシェーダプログラムを読み込む
	const GLuint shader(ggLoadShader("mesh.vert", "mesh.frag"));
	if (!shader) throw "Can't create program object.";

	// uniform 変数の場所を指定する
	const GLuint image0Loc(glGetUniformLocation(shader, "image0"));
	const GLuint image1Loc(glGetUniformLocation(shader, "image1"));
	const GLuint spacingLoc(glGetUniformLocation(shader, "spacing"));
	const GLuint mcLoc(glGetUniformLocation(shader, "mc"));

	// テクスチャをマッピングするメッシュを作成する
	//   頂点座標値を vertex shader で生成するので VBO は必要ない
	const GLuint mesh([]() { GLuint mesh; glGenVertexArrays(1, &mesh); return mesh; } ());

	// 背景色を指定する
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// 背面を描画しない
	glEnable(GL_CULL_FACE);

	// モデルビュー投影変換行列
	const GgMatrix mc(ggIdentity());

	// ビデオ入力
	Capture video0(0), video1(1);

	// ウィンドウが開いている間繰り返す
	while (window.shouldClose() == GL_FALSE)
	{
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
		glClear(GL_COLOR_BUFFER_BIT);

		// シェーダプログラムの使用を開始する
		glUseProgram(shader);

		// テクスチャユニットを指定する
		glUniform1i(image0Loc, 0);
		glUniform1i(image1Loc, 1);

		// メッシュの格子間隔を指定する
		glUniform2f(spacingLoc, 1.0f / slices, 1.0f / stacks);

		// モデルビュー投影変換行列を指定する
		glUniformMatrix4fv(mcLoc, 1, GL_FALSE, mc.get());

		// メッシュを描画する
		glBindVertexArray(mesh);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, (slices + 1) * 2, stacks);

		// カラーバッファを入れ替えてイベントを取り出す
		window.swapBuffers();
	}
}
