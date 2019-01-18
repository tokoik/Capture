//
// OpenCV によるビデオキャプチャ
//
#include "Capture.h"

// OpenCV のライブラリ
#define CV_VERSION_STR CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)
#if defined(_DEBUG)
#	pragma comment(lib, "opencv_world" CV_VERSION_STR "d.lib")
#else
#	pragma comment(lib, "opencv_world" CV_VERSION_STR ".lib")
#endif

// 標準ライブラリ
#include <iostream>
#include <thread>

// デフォルトコンストラクタ
Capture::Capture()
{
}

// デバイスからキャプチャするコンストラクタ
Capture::Capture(int device, int width, int height, int fps)
	: camera(device)
{
	init(width, height, fps);
}

// ファイルからキャプチャするコンストラクタ
Capture::Capture(const std::string &filename)
	: camera(filename)
{
	init(0, 0, 0);
}

// デストラクタ
Capture::~Capture()
{
	glDeleteTextures(1, &texture);
}

// キャプチャ可能なら最初の 1 フレームをキャプチャする
void Capture::init(int width, int height, int fps)
{
	// カメラが使えるかどうか確かめる
	if (!camera.isOpened()) throw std::runtime_error("Can't open camera device.");

	// カメラの解像度を設定する
	camera.set(CV_CAP_PROP_FRAME_WIDTH, static_cast<double>(width));
	camera.set(CV_CAP_PROP_FRAME_HEIGHT, static_cast<double>(height));
	camera.set(CV_CAP_PROP_FPS, static_cast<double>(fps));

	// カメラから最初のフレームをキャプチャする
  if (!camera.grab()) throw std::runtime_error("Can't capture first frame.");

	// フレームを取り出す
	camera.retrieve(frame, 3);

	// フレームが取り出せたかどうか確かめる
	if (!frame.data) throw std::runtime_error("Can't retrieve first frame.");

	// 色データのテクスチャを作成する
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
  glTexStorage2D(GL_TEXTURE_2D, 4, GL_RGB8, frame.cols, frame.rows);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// テクスチャの境界色
	static constexpr GLfloat border[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
}

// デバイスを開く
void Capture::open(int device, int width, int height, int fps)
{
	camera.open(device);
	init(width, height, fps);
}

// ファイルを開く
void Capture::open(const std::string &filename)
{
	camera.open(filename);
	init(0, 0, 0);
}

// 1 フレームキャプチャしてテクスチャに転送する
void Capture::capture()
{
	// 1 フレーム取り込む
	if (camera.grab())
	{
		// フレームを取り出す
		camera.retrieve(frame, 3);

		// フレームが取り出せたら
		if (frame.data)
		{
			// テクスチャに転送して
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
      glGenerateMipmap(GL_TEXTURE_2D);

			// 次へ
			return;
		}
	}

#if defined(_DEBUG)
	std::cerr << "frame dropped.";
#endif

	// 1 フレーム分待つ
	std::this_thread::sleep_for(std::chrono::milliseconds(16));
}
