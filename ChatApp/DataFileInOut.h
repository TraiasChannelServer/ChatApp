#pragma once

#include "DxLib.h"
#include <string>

class DataFileInOut
{
public:
	static constexpr char FILE_PATH[] = "Data\\Setting.dat";
	static constexpr int NAME_LENGTH_MAX = 16;

public:
	static DataFileInOut& Inst();

private:
	DataFileInOut();
	DataFileInOut(const DataFileInOut&) = delete;
	DataFileInOut(DataFileInOut&&) = delete;
	DataFileInOut& operator=(const DataFileInOut&) = delete;
	DataFileInOut& operator=(DataFileInOut&&) = delete;

public:
	void Input();
	void Output();

	const IPDATA& GetIP() const;
	void SetIP(const IPDATA& IP);

	int GetPortNum() const;
	void SetPortNum(int Port);

	const std::string& GetName() const;
	void SetName(const std::string& Name);

private:
	IPDATA m_IP;
	int m_Port;
	std::string m_Name;
};

inline DataFileInOut& DataFileInOut::Inst()
{
	static DataFileInOut s_Inst;
	return s_Inst;
}

inline const IPDATA& DataFileInOut::GetIP() const
{
	return m_IP;
}

inline void DataFileInOut::SetIP(const IPDATA& IP)
{
	m_IP = IP;
}

inline int DataFileInOut::GetPortNum() const
{
	return m_Port;
}

inline void DataFileInOut::SetPortNum(int Port)
{
	m_Port = Port;
}

inline const std::string& DataFileInOut::GetName() const
{
	return m_Name;
}

inline void DataFileInOut::SetName(const std::string& Name)
{
	m_Name = Name;
}

