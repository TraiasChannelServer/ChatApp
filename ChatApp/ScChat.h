#pragma once

#include "ScreenComponent.h"
#include <vector>
#include <string>

class ScChat : public ScreenComponent
{
public:
	struct Line
	{
		int ID;
		std::string Head;
		std::string Body;
	};

public:
	ScChat(Bound* bound, int FontHandleHead, int FontHandleBody, unsigned int ColorHead, unsigned int ColorBody, unsigned int ColorFrame);

public:
	void OnMouseMove(int x, int y) override;
	void OnMousePress(int x, int y) override;
	void Draw() const override;

	void AddLine(int ID, const std::string& Name, const std::string& Text);
	void ChangeName(int ID, const std::string& Name);
	void ClearAll();

	const std::vector<Line>& GetLines() const;

private:
	std::string MakeHead(int ID, const std::string& Name);

private:
	int m_FontHandleHead;
	int m_FontHandleBody;
	unsigned int m_ColorHead;
	unsigned int m_ColorBody;
	unsigned int m_ColorFrame;
	int m_YStep;
	int m_YStepHead;

	std::vector<Line> m_Lines;
};

