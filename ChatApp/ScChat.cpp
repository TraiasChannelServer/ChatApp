#include "ScChat.h"
#include "FormatToString.h"
#include "Logger.h"
#include "DxLib.h"

ScChat::ScChat(Bound* bound, int FontHandleHead, int FontHandleBody, unsigned int ColorHead, unsigned int ColorBody, unsigned int ColorFrame)
	: ScreenComponent(bound)
	, m_FontHandleHead(FontHandleHead)
	, m_FontHandleBody(FontHandleBody)
	, m_ColorHead(ColorHead)
	, m_ColorBody(ColorBody)
	, m_ColorFrame(ColorFrame)
	, m_Lines()
	, m_YStep()
	, m_YStepHead()
{
	m_YStep = GetFontSizeToHandle(FontHandleBody) + 6;
	m_YStepHead = GetFontSizeToHandle(FontHandleHead) + 6;
}

void ScChat::OnMouseMove(int x, int y)
{
}

void ScChat::OnMousePress(int x, int y)
{
}

void ScChat::Draw() const
{
	RECT r = m_Bound->GetRect();
	DrawBox(r.left, r.top, r.right, r.bottom, m_ColorFrame, FALSE);

	int Size = m_Lines.size();

	if (Size == 0)
	{
		return;
	}

	const int LineYStep = m_YStep * 3;

	int x = r.left + 10;
	int y = r.bottom - LineYStep * Size + (m_YStep * 3 / 4);

	for (auto& line : m_Lines)
	{
		DrawStringToHandle(x, y, line.Head.c_str(), m_ColorHead, m_FontHandleHead);
		DrawStringToHandle(x, y + m_YStepHead, line.Body.c_str(), m_ColorBody, m_FontHandleBody);
		y += LineYStep;
	}
}

void ScChat::AddLine(int ID, const std::string& Name, const std::string& Text)
{
	Line line;
	line.ID = ID;
	line.Head = MakeHead(ID, Name);
	line.Body = Text;

	LOG_INFO("チャット追加：Head = %s, Body = %s", line.Head.c_str(), line.Body.c_str());

	m_Lines.emplace_back(std::move(line));

	// もし境界外にチャットが出てしまったら古いチャットから消していく
	RECT r = m_Bound->GetRect();
	int y = r.bottom - (m_YStep * 3) * m_Lines.size() + (m_YStep * 3 / 4);
	if (y < r.top)
	{
		const auto& Old = m_Lines.begin();
		LOG_INFO("チャット削除：Head = %s, Body = %s", Old->Head.c_str(), Old->Body.c_str());
		m_Lines.erase(Old);
	}
}

void ScChat::ChangeName(int ID, const std::string& Name)
{
	LOG_INFO("チャットのHeadの変更：ID = %s, Name = %s", ID, Name.c_str());

	for (auto& line : m_Lines)
	{
		if (line.ID == ID)
		{
			line.Head = MakeHead(ID, Name);
		}
	}
}

void ScChat::ClearAll()
{
	LOG_INFO("全てのチャットを削除");
	m_Lines.clear();
}

const std::vector<ScChat::Line>& ScChat::GetLines() const
{
	return m_Lines;
}

std::string ScChat::MakeHead(int ID, const std::string& Name)
{
	if (ID == -1)
	{
		return MakeStringFromFormat("名前：%s ID：ホスト", Name);
	}
	return MakeStringFromFormat("名前：%s ID：%d", Name, ID);
}
