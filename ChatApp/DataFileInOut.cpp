#include "DataFileInOut.h"
#include <fstream>

DataFileInOut::DataFileInOut()
	: m_IP()
	, m_Port()
	, m_Name()
{
}

void DataFileInOut::Input()
{
	std::ifstream IFStream(FILE_PATH, std::ios::in | std::ios::binary);
	IFStream.read(reinterpret_cast<char*>(&m_IP), sizeof(m_IP));
	IFStream.read(reinterpret_cast<char*>(&m_Port), sizeof(m_Port));

	char Buf[NAME_LENGTH_MAX] = {};
	IFStream.read(Buf, NAME_LENGTH_MAX);
	m_Name = Buf;
}

void DataFileInOut::Output()
{
	std::ofstream OFStream(FILE_PATH, std::ios::out | std::ios::binary);
	OFStream.write(reinterpret_cast<const char*>(&m_IP), sizeof(m_IP));
	OFStream.write(reinterpret_cast<const char*>(&m_Port), sizeof(m_Port));
	OFStream.write(m_Name.c_str(), NAME_LENGTH_MAX);

}
