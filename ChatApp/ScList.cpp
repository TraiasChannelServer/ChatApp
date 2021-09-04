#include "ScList.h"
#include "Logger.h"

ScList::ScList(unsigned int ColorFore, unsigned int ColorBack, Bound* bound, TextAlignment::Gravity gravity, int FontHandle, unsigned int ColorFrame)
	: ScreenComponent(bound)
	, m_ColorFore(ColorFore)
	, m_ColorBack(ColorBack)
	, m_ColorFrame(ColorFrame)
	, m_Gravity(gravity)
	, m_Item()
	, m_FontHandle(FontHandle)
	, m_ItemYSize()
{
	Alignment();
}

void ScList::OnMouseMove(int x, int y)
{
	// Do nothing
}

void ScList::OnMousePress(int x, int y)
{
	// Do nothing
}

void ScList::Draw() const
{
	if (m_Item.empty())
	{
		RECT r = m_Bound->GetRect();
		DrawBox(r.left, r.top, r.right, r.bottom, m_ColorFrame, TRUE);
		DrawBox(r.left + 1, r.top + 1, r.right - 1, r.bottom - 1, m_ColorBack, TRUE);
		DrawStringToHandle(r.left + 5, r.top + 1, "--List-is-empty--", m_ColorFore, m_FontHandle);
	}
	else
	{
		RECT r = m_Bound->GetRect();
		for (const auto& pair : m_Item)
		{
			DrawBox(r.left, r.top, r.right, r.bottom, m_ColorFrame, TRUE);
			DrawBox(r.left + 1, r.top + 1, r.right - 1, r.bottom - 1, m_ColorBack, TRUE);

			const Item& item = pair.second;
			DrawStringToHandle(item.x, item.y, item.Text.c_str(), m_ColorFore, m_FontHandle);

			OffsetRect(&r, 0, m_ItemYSize);
		}
	}
}

void ScList::AddItem(int ID, const std::string& Text)
{
	if (m_Item.count(ID) == 1)
	{
		Logger::Warn("リストにアイテムを追加しようとしたが既に存在する：ID = %d, Text = %s", ID, m_Item[ID].Text.c_str());
		return;
	}

	Logger::Info("リストにアイテムを追加：ID = %d, Text = %s", ID, Text.c_str());

	Item item = {Text};
	m_Item[ID] = item;
	Alignment();
}

void ScList::RemoveItem(int ID)
{
	if (m_Item.count(ID) == 0)
	{
		Logger::Warn("リストからアイテムを削除しようとしたが存在しない：ID = %d", ID);
		return;
	}

	Logger::Info("リストからアイテムを削除：ID = %d, Text = %s", ID, m_Item[ID].Text.c_str());
	m_Item.erase(ID);
	Alignment();
}

void ScList::RemoveAllItem()
{
	Logger::Info("リストから全アイテムを削除");
	m_Item.clear();
	Alignment();
}

void ScList::ChangeText(int ID, const std::string& Text)
{
	if (m_Item.count(ID) == 0)
	{
		Logger::Warn("リストのアイテム名を変更しようとしたが存在しない：ID = %d", ID);
		return;
	}

	Logger::Info("リストのアイテム名を変更：Text = %s → %s", m_Item[ID].Text.c_str(), Text.c_str());

	m_Item[ID].Text = Text;
}

const std::string& ScList::GetText(int ID) const
{
	if (m_Item.count(ID) == 0)
	{
		static std::string ErrorText = "ErrorItem";
		return ErrorText;
	}

	return m_Item.at(ID).Text;
}

const std::map<int, ScList::Item>& ScList::GetItem() const
{
	return m_Item;
}

void ScList::Alignment()
{
	RECT r = m_Bound->GetRect();
	m_ItemYSize = r.bottom - r.top;

	for (auto& pair : m_Item)
	{
		Item& item = pair.second;
		RECT aligned = TextAlignment::Execute(item.Text, r, m_Gravity, m_FontHandle, 8);
		item.x = aligned.left;
		item.y = aligned.top;

		OffsetRect(&r, 0, m_ItemYSize);
	}
}

