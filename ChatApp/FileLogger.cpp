#include "FileLogger.h"

FileLogger::FileLogger()
	: FilePath("")
	, Stream()
{
}

FileLogger::FileLogger(const std::string& FilePath)
	: FilePath(FilePath)
	, Stream()
{
	Open(FilePath);
}

FileLogger::~FileLogger()
{
	Close();
}

void FileLogger::Open(const std::string& FilePath)
{
	if (IsOpen() == false)
	{
		Stream.open(FilePath, std::ios::out);
	}
}

void FileLogger::Close()
{
	if (IsOpen())
	{
		Stream.close();
	}
}

bool FileLogger::IsOpen()
{
	return Stream.is_open();
}

void FileLogger::Write(const std::string& Log)
{
	if (IsOpen())
	{
		Stream << Log << std::endl;
	}
}
