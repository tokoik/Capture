// アプリケーション本体
#include "GgApplication.h"

// ビデオキャプチャ
#include "Capture.h"

// メッシュの高さと幅
constexpr int slices(1), stacks(1);

// ビューポートのサイズ
constexpr int width(800), height(450);
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
