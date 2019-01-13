#pragma once

// OpenGL/GLFW
#include "gg.h"

// OpenCV
#include <opencv2/opencv.hpp>

// 標準ライブラリ
#include <memory>

class Capture
{
	// キャプチャデバイス
	cv::VideoCapture camera;

	// 画像を転送するテクスチャ
	GLuint texture;

	// キャプチャデバイスを開く
	void init(int width, int height, int fps);

public:

	// キャプチャした画像
	cv::Mat frame;

	// コンストラクタ
	Capture();
	Capture(int device, int width = 0, int height = 0, int fps = 0);
	Capture(const std::string &filename);

	// デストラクタ
	virtual ~Capture();

  // コピーコンストラクタを封じる
  Capture(const Capture &) = delete;

  // 代入演算子を封じる
  Capture &operator=(const Capture &) = delete;

	// キャプチャデバイスを開く
	void open(int device, int width = 0, int height = 0, int fps = 0);
	void open(const std::string &filename);

	// １フレームキャプチャする
	void capture();
};
