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

	// コピーコンストラクタを封じる
	Capture(const Capture &) {};

	// 代入演算子を封じる
	Capture &operator=(const Capture &) {};

	// キャプチャデバイスを開く
	void init();

public:

	// キャプチャした画像
	cv::Mat frame;

	// コンストラクタ
	Capture();
	Capture(int device = 0);
	Capture(const std::string &filename);

	// デストラクタ
	virtual ~Capture();

	// キャプチャデバイスを開く
	void open(int device);
	void open(const std::string &filename);

	// １フレームキャプチャする
	void capture();
};
