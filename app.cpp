// アプリケーション本体
#include "GgApplication.h"

// ビデオキャプチャ
#include "Capture.h"

// メッシュの高さと幅
constexpr int meshSlices(1), meshStacks(1);

// ビューポートのサイズ
constexpr int width(800), height(450);

// アニメーションの周期（秒）
constexpr double cycle(5.0);

// 球のデータの分割数
constexpr int slices(64);
constexpr int stacks(32);

// 球のデータの頂点数と面数
constexpr int vertices((slices + 1) * (stacks + 1));
constexpr int faces(slices * stacks * 2);

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
			tv[k][0] = s * 2.0f;
			tv[k][1] = t;

			++k;
		}
	}

	// 面の指標を求める
	for (int k = 0, j = 0; j < stacks; ++j)
	{
		for (int i = 0; i < slices; ++i)
		{
			const int count((slices + 1) * j + i);

			// 上半分の三角形
			f[k][0] = count;
			f[k][1] = count + slices + 2;
			f[k][2] = count + 1;
			++k;

			// 下半分の三角形
			f[k][0] = count;
			f[k][1] = count + slices + 1;
			f[k][2] = count + slices + 2;
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

	// 背景色を指定する
	glClearColor(0.1f, 0.2f, 0.3f, 0.0f);

	// 隠面消去を有効にする
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// 球体描画用のシェーダプログラムを読み込む
	const GLuint program(ggLoadShader("sphere.vert", "sphere.frag"));
	if (!program) throw "Can't create program object for sphere.";

	// uniform 変数のインデックスの検索（見つからなければ -1）
	const GLint mvLoc(glGetUniformLocation(program, "mv"));
	const GLint mpLoc(glGetUniformLocation(program, "mp"));
	const GLint mgLoc(glGetUniformLocation(program, "mg"));
	const GLint colorLoc(glGetUniformLocation(program, "color"));

	// 球のビュー変換行列を mv に求める
	const GgMatrix mv(ggLookat(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

	// 図形データの作成
	GLfloat pv[vertices][3];
	GLfloat nv[vertices][3];
	GLfloat tv[vertices][2];
	GLuint face[faces][3];
	makeSphere(1.0f, slices, stacks, pv, nv, tv, face);

	// 頂点配列オブジェクトの作成
	GLuint sphere;
	glGenVertexArrays(1, &sphere);
	glBindVertexArray(sphere);

	// 頂点バッファオブジェクトの作成
	GLuint vbo[4];
	glGenBuffers(sizeof vbo / sizeof vbo[0], vbo);

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	// 頂点の座標値 pv 用のバッファオブジェクト
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof pv, pv, GL_STATIC_DRAW);

	// 結合されている頂点バッファオブジェクトを in 変数 pv (index == 0) から参照できるようにする
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 頂点の法線ベクトル nv 用のバッファオブジェクト
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof nv, nv, GL_STATIC_DRAW);

	// 結合されている頂点バッファオブジェクトを in 変数 nv (index == nvLoc) から参照できるようにする
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// 頂点のテクスチャ座標値 tv 用のバッファオブジェクト
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof tv, tv, GL_STATIC_DRAW);

	// 結合されている頂点バッファオブジェクトを in 変数 tv (index == tvLoc) から参照できるようにする
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// 頂点のインデックス face 用のバッファオブジェクト
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof face, face, GL_STATIC_DRAW);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  

	// 背景描画用のシェーダプログラムを読み込む
	const GLuint shader(ggLoadShader("mesh.vert", "mesh.frag"));
	if (!shader) throw "Can't create program object for mesh.";

	// uniform 変数の場所を指定する
	const GLuint image0Loc(glGetUniformLocation(shader, "image0"));
	const GLuint image1Loc(glGetUniformLocation(shader, "image1"));
	const GLuint spacingLoc(glGetUniformLocation(shader, "spacing"));
	const GLuint mcLoc(glGetUniformLocation(shader, "mc"));

	// テクスチャをマッピングするメッシュを作成する
	//   頂点座標値を vertex shader で生成するので VBO は必要ない
	const GLuint mesh([]() { GLuint mesh; glGenVertexArrays(1, &mesh); return mesh; } ());

	// 背景のモデルビュー投影変換行列
	const GgMatrix mc(ggIdentity());

	// ビデオ入力
	Capture video0(0), video1(1);

	// 経過時間のリセット
	glfwSetTime(0.0);

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if 0

		// シェーダプログラムの使用を開始する
		glUseProgram(shader);

		// テクスチャユニットを指定する
		glUniform1i(image0Loc, 0);
		glUniform1i(image1Loc, 1);

		// メッシュの格子間隔を指定する
		glUniform2f(spacingLoc, 1.0f / meshSlices, 1.0f / meshStacks);

		// モデルビュー投影変換行列を指定する
		glUniformMatrix4fv(mcLoc, 1, GL_FALSE, mc.get());

		// メッシュを描画する
		glBindVertexArray(mesh);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, (meshSlices + 1) * 2, meshStacks);

#else

		// シェーダプログラムの使用開始
		glUseProgram(program);

		// 時刻の計測
		const float t(static_cast<float>(fmod(glfwGetTime(), cycle) / cycle));

		// モデルビュー変換行列 (時刻 t にもとづく回転アニメーション)
		const GgMatrix mw(mv.rotateY(3.1415927f * 1.0f));

		// 法線変換行列
		const GgMatrix mg(mw.normal());

		// 投影変換行列
		const GgMatrix mp(ggPerspective(0.5f, window.getAspect(), 1.0f, 15.0f));

		// uniform 変数を設定する
		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, mw.get());
		glUniformMatrix4fv(mpLoc, 1, GL_FALSE, mp.get());
		glUniformMatrix4fv(mgLoc, 1, GL_FALSE, mg.get());
		glUniform1i(colorLoc, 0);

		// 描画に使う頂点配列オブジェクトの指定
		glBindVertexArray(sphere);

		// 図形の描画
		glDrawElements(GL_TRIANGLES, faces * 3, GL_UNSIGNED_INT, 0);

#endif

		// カラーバッファを入れ替えてイベントを取り出す
		window.swapBuffers();
	}
}
