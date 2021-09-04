#pragma once

// An Embedded Engineer’s Blog
// とある組み込みエンジニアの備忘録的なブログです。
// C++で文字列フォーマッティング(書式付き文字列生成)
// https://an-embedded-engineer.hateblo.jp/entry/2020/08/23/161317

#include <string>
#include <cstdio>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <memory>

namespace FormatToString
{
// std::string型をconst char*に変換
template<typename T, typename std::enable_if<std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, std::string>::value>::type* = nullptr>
auto Convert(T&& value)
{
	return std::forward<T>(value).c_str();
}

// std::string型以外は、そのまま出力
template<typename T, typename std::enable_if<!std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, std::string>::value>::type* = nullptr>
auto Convert(T&& value)
{
	return std::forward<T>(value);
}

// 文字列のフォーマッティング(内部処理)
template<typename ... Args>
std::string Make(const std::string& FormatString, Args&& ... args)
{
	// フォーマット後の文字数を算出
	int str_len = std::snprintf(nullptr, 0, FormatString.c_str(), args ...);

	// フォーマット失敗
	if (str_len < 0)
	{
		//throw std::runtime_error("String Formatting Error");
		return std::string("[ERR] String Formatting Failed: ").append(FormatString);
	}

	// バッファサイズを算出(文字列長 + null文字サイズ)
	size_t buffer_size = str_len + sizeof(char);

	// バッファサイズ分メモリ確保
	std::unique_ptr<char[]> buffer(std::make_unique<char[]>(buffer_size));

	// 文字列のフォーマット
	std::snprintf(buffer.get(), buffer_size, FormatString.c_str(), std::forward<Args>(args) ...);

	// 文字列をstd::string型に変換して出力
	return std::string(buffer.get(), buffer.get() + str_len);
}
}

// 文字列のフォーマッティング
template<typename ... Args>
std::string MakeStringFromFormat(const std::string& FormatString, Args&& ... args)
{
	// 各パラメータの型を変換して、文字列のフォーマッティング
	return FormatToString::Make(FormatString, FormatToString::Convert(std::forward<Args>(args)) ...);
}