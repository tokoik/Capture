// アプリケーション本体
#include "GgApplication.h"

// ビデオキャプチャ
#include "Capture.h"

// メッシュの高さと幅
constexpr int slices(1), stacks(1);

//
// アプリケーションの実行
//
void GgApplication::run()
{
	// ウィンドウを作成する
	Window window("Capture", 1280, 720);

	// 背景描画用のシェーダプログラムを読み込む
	const GLuint shader(ggLoadShader("mesh.vert", "mesh.frag"));
	if (!shader) throw "Can't create program object.";

	// uniform 変数の場所を指定する
	const GLuint spacingLoc(glGetUniformLocation(shader, "spacing"));
	const GLuint imageLoc(glGetUniformLocation(shader, "image"));

	// テクスチャをマッピングするメッシュを作成する
	//   頂点座標値を vertex shader で生成するので VBO は必要ない
	const GLuint mesh([]() { GLuint mesh; glGenVertexArrays(1, &mesh); return mesh; } ());

	// 背景色を指定する
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	// 背面を描画しない
	glEnable(GL_CULL_FACE);

	// ビデオ入力
	Capture video0(0), video1(1);

	// ウィンドウが開いている間繰り返す
	while (window.shouldClose() == GL_FALSE)
	{
		// ウィンドウを消去する
		glClear(GL_COLOR_BUFFER_BIT);

		// シェーダプログラムの使用を開始する
		glUseProgram(shader);

		// メッシュの格子間隔を指定する
		glUniform2f(spacingLoc, 1.0f / slices, 1.0f / stacks);

		// テクスチャユニットを指定する
		glUniform1i(imageLoc, 0);

		// マッピングに使うテクスチャユニット
		glActiveTexture(GL_TEXTURE0);

		// ビデオをキャプチャしてテクスチャに入れる
		video0.capture();

		// メッシュを描画する
		glBindVertexArray(mesh);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, (slices + 1) * 2, stacks);

		// カラーバッファを入れ替えてイベントを取り出す
		window.swapBuffers();
	}
}
