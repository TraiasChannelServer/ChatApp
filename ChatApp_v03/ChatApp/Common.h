#pragma once

/*
 * アプリ全体で共有するデータ・機能を管理する
 */

#include "DxLib.h"
#include <string>

class Common
{
public:
	static constexpr int WINDOW_X_SIZE = 960;
	static constexpr int WINDOW_Y_SIZE = 720;

public:
	static std::string IP2String(const IPDATA& ip);

public:
	static bool ApplicationExecution;

private:
	Common() = delete;
};