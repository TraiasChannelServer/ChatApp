#pragma once

// An Embedded Engineer’s Blog
// とある組み込みエンジニアの備忘録的なブログです。
// C++でログ出力
// https://an-embedded-engineer.hateblo.jp/entry/2020/08/29/230546

#include "FormatToString.h"
#include "FileLogger.h"

#include <string>
#include <mutex>

#define LOGGER_ON

// このマクロを通してLoggerクラスを利用するとログ出力のON/OFFが可能
#ifdef LOGGER_ON
#define LOG_INFO(...) Logger::Info(__VA_ARGS__)
#define LOG_WARN(...) Logger::Warn(__VA_ARGS__)
#define LOG_ERROR(...) Logger::Error(__VA_ARGS__)
#define LOG_BEGIN_END(...) Logger::AutoBeginEnd LoggerAutoBeginEnd(Logger::BeginEnd(__VA_ARGS__))
#else
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_BEGIN_END(...) 
#endif

class Logger
{
public:
	// Begin/Endログ自動出力クラス
	// オブジェクトの生存期間を利用して使う
	class AutoBeginEnd
	{
	public:
		AutoBeginEnd(const std::string& Message)
			: Message_(Message)
		{
			Logger::GetInstance().LogBegin(Message_);
		}

		~AutoBeginEnd()
		{
			Logger::GetInstance().LogEnd(Message_);
		}

	private:
		const std::string Message_;
	};

public:
	// 書式指定Informationログ
	template<typename ... Args>
	static void Info(const std::string& FormatString, Args&& ... args)
	{
		Logger::GetInstance().LogInfo(MakeStringFromFormat(FormatString, std::forward<Args>(args) ...));
	}

	// 書式指定Warningログ
	template<typename ... Args>
	static void Warn(const std::string& FormatString, Args&& ... args)
	{
		Logger::GetInstance().LogWarn(MakeStringFromFormat(FormatString, std::forward<Args>(args) ...));
	}

	// 書式指定Errorログ 
	template<typename ... Args>
	static void Error(const std::string& FormatString, Args&& ... args)
	{
		Logger::GetInstance().LogError(MakeStringFromFormat(FormatString, std::forward<Args>(args) ...));
	}

	// 書式指定BeginEnd自動ログ
	template<typename ... Args>
	static Logger::AutoBeginEnd BeginEnd(const std::string& FormatString, Args&& ... args)
	{
		return Logger::AutoBeginEnd(MakeStringFromFormat(FormatString, std::forward<Args>(args) ...));
	}

private:
	static Logger& GetInstance();

private:
	Logger();
	~Logger();

	Logger(const Logger&) = delete;
	Logger(Logger&&) = delete;
	Logger& operator=(const Logger&) = delete;
	Logger& operator=(Logger&&) = delete;

private:
	void LogInfo(const std::string& Message);
	void LogWarn(const std::string& Message);
	void LogError(const std::string& Message);
	void LogBegin(const std::string& Message);
	void LogEnd(const std::string& Message);

	std::string GetNowTime();

private:
	std::mutex MyMutex;
	FileLogger MyFileLogger;
};

