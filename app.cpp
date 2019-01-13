// アプリケーション本体
#include "GgApplication.h"

// ビデオキャプチャ
#include "Capture.h"

// 標準ライブラリ
#include <array>
#include <vector>

// メッシュの高さと幅
constexpr int meshSlices(1), meshStacks(1);

// ビューポートのサイズ
constexpr int width(800), height(450);

// アニメーションの周期（秒）
constexpr double cycle(5.0);

// 球のデータの分割数
constexpr int slices(64);
constexpr int stacks(32);

// データ型
struct vec2
{
  GLfloat x, y;
  vec2() {}
  vec2(GLfloat x, GLfloat y) : x(x), y(y) {}
};
struct vec3
{
  GLfloat x, y, z;
  vec3() {}
  vec3(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}
};
struct vec4
{
  GLfloat x, y, z, w;
  vec4() {}
  vec4(GLfloat x, GLfloat y) : x(x), y(y), z(z), w(w) {}
};
struct idx
{
  GLuint i0, i1, i2;
  idx() {}
  idx(GLuint i0, GLuint i1, GLuint i2) : i0(i0), i1(i1), i2(i2) {}
};

//
// 球のデータの作成
//
static void makeSphere(float radius, int slices, int stacks,
  std::vector<vec3> &position,
  std::vector<vec3> &normal,
  std::vector<vec2> &texture,
  std::vector<idx> &face)
{
	// 頂点の位置とテクスチャ座標を求める
	for (int j = 0; j <= stacks; ++j)
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
      position.emplace_back(x * radius, y * radius, z * radius);

			// 頂点の法線ベクトル
      normal.emplace_back(x, y, z);

			// 頂点のテクスチャ座標値
			const float d(acos(abs(z)) / (hypot(x, y) * 1.775f));
      texture.emplace_back(x * d, y * d);
		}
	}

	// 面の指標を求める
	for (int j = 0; j < stacks; ++j)
	{
		for (int i = 0; i < slices; ++i)
		{
			const int count((slices + 1) * j + i);

			// 上半分の三角形
      face.emplace_back(count, count + slices + 2, count + 1);

			// 下半分の三角形
			face.emplace_back(count, count + slices + 1, count + slices + 2);
		}
	}
}

//
// 正距方位法の円盤データの作成
//
static void makeDisc(float radius, float xoffset, float yoffset, int slices, int stacks,
  std::vector<vec3> &position,
  std::vector<vec3> &normal,
  std::vector<vec2> &texture,
  std::vector<idx> &face)
{
  // 最初の頂点番号
  const int start(static_cast<int>(position.size()));

  // テクスチャのオフセット
  const float toffset(xoffset / radius);

  // 頂点の位置とテクスチャ座標を求める
  for (int j = 0; j <= stacks; ++j)
  {
    const float t(static_cast<float>(j) / static_cast<float>(stacks));
    const float ph(3.141593f * t);
    const float y(cos(ph));
    const float r(sin(ph));

    for (int i = 0; i <= slices; ++i)
    {
      const float s(static_cast<float>(i) / static_cast<float>(slices));
      const float th(-3.141593f * s);
      const float x(r * cos(th));
      const float z(r * sin(th));

      // テクスチャ座標値
      const float w(2.0f * acos(-z) / (3.141593f * hypot(x, y)));
      const float u(-x * w), v(y * w);

      // 頂点の座標値
      position.emplace_back(u * radius + xoffset, v * radius + yoffset, 0.0f);

      // 頂点の法線ベクトル
      normal.emplace_back(0.0f, 0.0f, 1.0f);

      // 頂点のテクスチャ座標値
      const float d(acos(abs(z)) / (hypot(x, y) * 1.775f));
      texture.emplace_back(u * d + toffset + 1.0f, v * d);
    }
  }

  // 面の指標を求める
  for (int j = 0; j < stacks; ++j)
  {
    for (int i = 0; i < slices; ++i)
    {
      const int count((slices + 1) * j + i + start);

      // 上半分の三角形
      face.emplace_back(count, count + slices + 2, count + 1);

      // 下半分の三角形
      face.emplace_back(count, count + slices + 1, count + slices + 2);
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
	glEnable(GL_CULL_FACE);

	// 球体描画用のシェーダプログラムを読み込む
	//const GLuint program(ggLoadShader("sphere.vert", "sphere.frag"));
  const GLuint program(ggLoadShader("disc.vert", "disc.frag"));
  if (!program) throw std::runtime_error("Can't create program object for sphere.");

	// uniform 変数のインデックスの検索（見つからなければ -1）
	const GLint mvLoc(glGetUniformLocation(program, "mv"));
	const GLint mpLoc(glGetUniformLocation(program, "mp"));
	const GLint mgLoc(glGetUniformLocation(program, "mg"));
	const GLint colorLoc(glGetUniformLocation(program, "color"));

	// 球のビュー変換行列を mv に求める
	const GgMatrix mv(ggLookat(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

	// 図形データの作成
  std::vector<vec3> position;
  std::vector<vec3> normal;
  std::vector<vec2> texture;
  std::vector<idx> face;
#if 0
  makeSphere(1.0f, slices, stacks, position, normal, texture, face);
#else
	makeDisc(0.5f, -0.5f, 0.5f, slices, stacks, position, normal, texture, face);
  makeDisc(0.5f,  0.5f, 0.5f, slices, stacks, position, normal, texture, face);
#endif

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
	glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof (vec3), position.data(), GL_STATIC_DRAW);

	// 結合されている頂点バッファオブジェクトを in 変数 pv (index == 0) から参照できるようにする
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// 頂点の法線ベクトル nv 用のバッファオブジェクト
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof (vec3), normal.data(), GL_STATIC_DRAW);

	// 結合されている頂点バッファオブジェクトを in 変数 nv (index == nvLoc) から参照できるようにする
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// 頂点のテクスチャ座標値 tv 用のバッファオブジェクト
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, texture.size() * sizeof (vec2), texture.data(), GL_STATIC_DRAW);

	// 結合されている頂点バッファオブジェクトを in 変数 tv (index == tvLoc) から参照できるようにする
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// 頂点のインデックス face 用のバッファオブジェクト
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, face.size() * sizeof (idx), face.data(), GL_STATIC_DRAW);

	// 背景描画用のシェーダプログラムを読み込む
	const GLuint shader(ggLoadShader("mesh.vert", "mesh.frag"));
	if (!shader) throw std::runtime_error("Can't create program object for mesh.");

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
	Capture video0(0, 1280, 720), video1(2, 1280, 720);

	// 経過時間のリセット
	glfwSetTime(0.0);

  // ウィンドウが開いている間繰り返す
	while (window)
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
		const GgMatrix mw(mv * window.getTrackball());

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
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(face.size()) * 3, GL_UNSIGNED_INT, 0);

#endif

		// カラーバッファを入れ替えてイベントを取り出す
		window.swapBuffers();
	}
}
