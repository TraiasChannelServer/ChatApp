#include "SceneGuest.h"
#include "ScButton.h"
#include "ScEdit.h"
#include "FontManager.h"
#include "BoundRect.h"
#include "Common.h"
#include "SceneCreator.h"
#include "Command.h"
#include "Logger.h"
#include "DataFileInOut.h"

const char SceneGuest::CONNECT_STEP_STATE[][64] =
{
	"接続していません",
	"承認されるのを待機しています...",
	"接続しています"
};

SceneGuest::SceneGuest()
	: m_Next()
	, m_IP(DataFileInOut::Inst().GetIP())
	, m_Port(DataFileInOut::Inst().GetPortNum())
	, m_Name(DataFileInOut::Inst().GetName())
	, m_ConnectStep()
	, m_ConnectState()
	, m_NetHandle()
	, m_HostName()
	, m_ConnectingGuest()
	, m_Chat()
{
	// コンストラクタでは画面の部品を配置する
	LOG_INFO("ゲスト画面生成");

	int TinyFont = FontManager::Inst().GetFontHandle(FontManager::Type::TINY);
	int SmallFont = FontManager::Inst().GetFontHandle(FontManager::Type::SMALL);
	int MiddleFont = FontManager::Inst().GetFontHandle(FontManager::Type::MIDDLE);
	int BigFont = FontManager::Inst().GetFontHandle(FontManager::Type::BIG);

	unsigned int BlackColor = GetColor(0, 0, 0);
	unsigned int GreenColor = GetColor(0, 255, 0);
	unsigned int RedColor = GetColor(255, 50, 255);

	unsigned int EditColorFore = GetColor(0, 0, 0);
	unsigned int EditColorBack = GetColor(180, 180, 180);
	unsigned int EditColorFrame = GetColor(50, 50, 50);
	unsigned int ToggleColorFore = GetColor(0, 255, 255);
	unsigned int ToggleColorBack = GetColor(50, 50, 100);

	static constexpr int X_POS = 120;

	static constexpr int X_START = 10;
	static constexpr int Y_SIZE = 18;
	static constexpr int Y_STEP_SMALL = 24;
	static constexpr int Y_STEP_MIDDLE = 28;
	static constexpr int Y_STEP_BIG = 40;
	int y = 10;

	AddBaseComponent(new ScLabel("あなたはゲストです", GreenColor, 10, y, SmallFont));

	y += Y_STEP_SMALL;
	AddBaseComponent(new ScLabel("IP：", GreenColor, 10, y, SmallFont));
	{
		static constexpr int IP_X_SIZE = 56;
		int x = X_POS;
		{
			BoundRect* bound = new BoundRect(x, y, IP_X_SIZE, Y_SIZE);
			auto Callback = new DelegateArg<SceneGuest, std::string>(*this, &SceneGuest::SetIP1);
			AddBaseComponent(new ScEdit(std::to_string(m_IP.d1), EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, 3, Callback));
		}
		x += IP_X_SIZE;
		{
			BoundRect* bound = new BoundRect(x, y, IP_X_SIZE, Y_SIZE);
			auto Callback = new DelegateArg<SceneGuest, std::string>(*this, &SceneGuest::SetIP2);
			AddBaseComponent(new ScEdit(std::to_string(m_IP.d2), EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, 3, Callback));
		}
		x += IP_X_SIZE;
		{
			BoundRect* bound = new BoundRect(x, y, IP_X_SIZE, Y_SIZE);
			auto Callback = new DelegateArg<SceneGuest, std::string>(*this, &SceneGuest::SetIP3);
			AddBaseComponent(new ScEdit(std::to_string(m_IP.d3), EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, 3, Callback));
		}
		x += IP_X_SIZE;
		{
			BoundRect* bound = new BoundRect(x, y, IP_X_SIZE, Y_SIZE);
			auto Callback = new DelegateArg<SceneGuest, std::string>(*this, &SceneGuest::SetIP4);
			AddBaseComponent(new ScEdit(std::to_string(m_IP.d4), EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, 3, Callback));
		}
	}

	y += Y_STEP_SMALL;
	AddBaseComponent(new ScLabel("ポート番号：", GreenColor, 10, y, SmallFont));
	{
		BoundRect* bound = new BoundRect(X_POS, y, 200, Y_SIZE);
		auto Callback = new DelegateArg<SceneGuest, std::string>(*this, &SceneGuest::SetPortNum);
		AddBaseComponent(new ScEdit(std::to_string(m_Port), EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, 5, Callback));
	}

	y += Y_STEP_SMALL;
	AddBaseComponent(new ScLabel("名前：", GreenColor, 10, y, SmallFont));
	{
		BoundRect* bound = new BoundRect(X_POS, y, 200, Y_SIZE);
		auto Callback = new DelegateArg<SceneGuest, std::string>(*this, &SceneGuest::SetName);
		AddBaseComponent(new ScEdit(m_Name, EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, DataFileInOut::NAME_LENGTH_MAX, Callback));
	}

	y += Y_STEP_SMALL * 2;
	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(180, 220, 220);
		unsigned int ColorFrame = GetColor(50, 100, 100);
		unsigned int ColorBackHover = GetColor(200, 240, 240);
		unsigned int ColorBackPress = GetColor(160, 200, 200);
		auto Callback = new DelegateVoid<SceneGuest>(*this, &SceneGuest::TryConnect);
		AddBaseComponent(new ScButton("接続を試みる", ColorFore, ColorBack, X_START + 10, y, MiddleFont, ColorFrame, ColorBackHover, ColorBackPress, Callback));
	}

	y += Y_STEP_SMALL * 2;
	m_ConnectState = new ScLabel(CONNECT_STEP_STATE[0], RedColor, 10, y, MiddleFont);
	AddBaseComponent(m_ConnectState);

	y += Y_STEP_SMALL * 2;
	AddBaseComponent(new ScLabel("ホスト名：", GreenColor, 10, y, SmallFont));

	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(210, 210, 210);
		BoundRect* bound = new BoundRect(100, y, 200, Y_SIZE);
		m_HostName = new ScLabel("--未接続--", ColorFore, ColorBack, bound, TextAlignment::Gravity::LEFT, SmallFont);
		AddBaseComponent(m_HostName);
	}

	y += Y_STEP_SMALL * 2;
	AddBaseComponent(new ScLabel("接続中のゲスト：", GreenColor, X_START, y, SmallFont));

	y += Y_STEP_SMALL;
	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(210, 210, 210);
		unsigned int ColorFrame = GetColor(100, 50, 50);
		BoundRect* bound = new BoundRect(X_START + 10, y, 250, Y_SIZE);
		m_ConnectingGuest = new ScList(ColorFore, ColorBack, bound, TextAlignment::Gravity::LEFT, SmallFont, ColorFrame);
		AddBaseComponent(m_ConnectingGuest);
	}

	{
		static constexpr int DELTA_WIDTH = 120;
		unsigned int ColorHead = GetColor(0, 200, 0);
		unsigned int ColorBody = GetColor(255, 255, 255);
		unsigned int ColorFrame = GetColor(150, 180, 180);
		int XPos = Common::WINDOW_X_SIZE / 2 - DELTA_WIDTH;
		int YPos = 20;
		int XSize = Common::WINDOW_X_SIZE / 2 + DELTA_WIDTH - 20;
		int YSize = Common::WINDOW_Y_SIZE - 40 - Y_STEP_MIDDLE;
		{
			// チャット枠
			BoundRect* bound = new BoundRect(XPos, YPos, XSize, YSize);
			m_Chat = new ScChat(bound, TinyFont, SmallFont, ColorHead, ColorBody, ColorFrame);
			AddBaseComponent(m_Chat);
		}
		{
			// チャット入力枠
			BoundRect* bound = new BoundRect(XPos, Common::WINDOW_Y_SIZE - Y_STEP_MIDDLE, XSize, Y_SIZE);
			auto Callback = new DelegateArg<SceneGuest, std::string>(*this, &SceneGuest::SetChatText);
			AddBaseComponent(new ScEdit("", EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, 64, Callback));
		}
	}

	y = Common::WINDOW_Y_SIZE - Y_STEP_MIDDLE;
	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(220, 220, 180);
		unsigned int ColorFrame = GetColor(100, 100, 50);
		unsigned int ColorBackHover = GetColor(240, 240, 200);
		unsigned int ColorBackPress = GetColor(200, 200, 160);
		auto Callback = new DelegateVoid<SceneGuest>(*this, &SceneGuest::RequestAllUpdate);
		AddBaseComponent(new ScButton("全更新リクエスト", ColorFore, ColorBack, X_START + 75, y, MiddleFont, ColorFrame, ColorBackHover, ColorBackPress, Callback));
	}

	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(180, 220, 180);
		unsigned int ColorFrame = GetColor(50, 100, 50);
		unsigned int ColorBackHover = GetColor(200, 240, 200);
		unsigned int ColorBackPress = GetColor(160, 200, 160);
		auto Callback = new DelegateVoid<SceneGuest>(*this, &SceneGuest::End);
		AddBaseComponent(new ScButton("終了", ColorFore, ColorBack, X_START, y, MiddleFont, ColorFrame, ColorBackHover, ColorBackPress, Callback));
	}
}

SceneGuest::~SceneGuest()
{
	LOG_INFO("ゲスト画面破棄");

	DataFileInOut::Inst().SetIP(m_IP);
	DataFileInOut::Inst().SetPortNum(m_Port);
	DataFileInOut::Inst().SetName(m_Name);

	Disconnect();
}

Scene* SceneGuest::Update()
{
	UpdateBase();
	switch (m_ConnectStep)
	{
	case ConnectStep::OFF:
		// ホストに接続していない状態
		break;
	case ConnectStep::WAIT_ACCEPT:
	{
		// 自分の名前を送信した後
		// ホストから承認・拒否メッセージを待つ
		Command::ReceiveResult result = Command::CheckReceive(m_NetHandle);
		if (result == Command::ReceiveResult::SUCCESS)
		{
			// メッセージが送られてきたら
			Command::Message Msg = Command::Receive(m_NetHandle);

			if (Msg.type == Command::Type::CONNECT)
			{
				// 承認・拒否メッセージだったとき
				if (Msg.single.flag)
				{
					// 承認されたとき
					LOG_INFO("[ConnectStep::WAIT_ACCEPT] 承認された");

					// 受け取ったホスト名を保持
					m_HostName->ChangeText(Msg.string.text);
					// 接続段階へ処理を進む
					SetConnectStep(ConnectStep::ON);
				}
				else
				{
					// 拒否されたとき
					LOG_INFO("[ConnectStep::WAIT_ACCEPT] 拒否された");

					// 切断する
					Disconnect();
					// 未接続段階へ処理を戻る
					SetConnectStep(ConnectStep::OFF);
				}
			}
			else
			{
				// 意味不明なメッセージだったとき
				// 切断する
				Disconnect();
				// 未接続段階へ処理を戻る
				SetConnectStep(ConnectStep::OFF);
			}
		}
	}
	break;
	case ConnectStep::ON:
	{
		// ホストに承認をもらって接続できているとき

		Command::ReceiveResult result = Command::CheckReceive(m_NetHandle);
		if (result == Command::ReceiveResult::SUCCESS)
		{
			// メッセージが送られてきたら
			Command::Message Msg = Command::Receive(m_NetHandle);

			switch (Msg.type)
			{
			case Command::Type::CONNECT:
			{
				if (Msg.single.flag == false)
				{
					// 切断しましょうメッセージだったとき
					Disconnect();
					SetConnectStep(ConnectStep::OFF);
				}
			}
			break;
			case Command::Type::CHAT_TEXT:
			{
				// 誰かがチャット打ちましたメッセージだったとき

				// チャットに追加する
				int ID = Msg.single.num;
				if (ID == -1)
				{
					// ホストのチャットだったとき
					m_Chat->AddLine(-1, m_HostName->GetText(), Msg.string.text);
				}
				else
				{
					// ゲストのチャットだったとき
					m_Chat->AddLine(ID, m_ConnectingGuest->GetText(ID), Msg.string.text);
				}
			}
			break;
			case Command::Type::CHANGE_NAME_MYSELF:
			{
				// ホストが名前変えましたメッセージだったとき
				m_HostName->ChangeText(Msg.string.text);

				// チャットの表示の方も名前の変更を反映する
				m_Chat->ChangeName(-1, Msg.string.text);
			}
			break;
			case Command::Type::CHANGE_NAME_GUEST:
			{
				// 誰かが名前変えましたメッセージだったとき
				// 接続済みゲストから誰なのかを突き止める
				auto& list = m_ConnectingGuest->GetItem();
				for (auto& pair : list)
				{
					int ID = pair.first;
					if (ID == Msg.single.num)
					{
						// 突き止めたら変更を反映する
						m_ConnectingGuest->ChangeText(ID, Msg.string.text);

						// チャットの表示の方も名前の変更を反映する
						m_Chat->ChangeName(ID, Msg.string.text);
						break;
					}
				}
			}
			break;
			case Command::Type::NEW_GUEST:
			{
				// 新規ゲストがやってきましたメッセージだったとき
				m_ConnectingGuest->AddItem(Msg.single.num, Msg.string.text);
			}
			break;
			case Command::Type::DISCONNECT_GUEST:
			{
				// 誰かが切断しましたよメッセージだったとき

				// 接続済みゲストから誰なのかを突き止める
				auto& list = m_ConnectingGuest->GetItem();
				for (auto& pair : list)
				{
					int ID = pair.first;
					if (ID == Msg.single.num)
					{
						// 突き止めたらリストから削除
						m_ConnectingGuest->RemoveItem(ID);
						break;
					}
				}
			}
			break;
			default:
				break;
			}

		}
	}
	break;
	}

	return m_Next;
}

void SceneGuest::Draw()
{
	ClearDrawScreen();
	DrawBase();
	switch (m_ConnectStep)
	{
	case ConnectStep::OFF:
		break;
	case ConnectStep::WAIT_ACCEPT:
		break;
	case ConnectStep::ON:
		break;
	}
}

void SceneGuest::SetIP1(std::string& IP)
{
	// IP1エディットテキストのコールバック関数

	// エディットテキストは数値以外も取りうるので
	// 画面部品とデータでIPを一致させなければならないことに気を付ける
	// IP2-4とPortも同様

	m_IP.d1 = std::stoi(IP);
	IP = std::to_string(m_IP.d1);
}

void SceneGuest::SetIP2(std::string& IP)
{
	m_IP.d2 = std::stoi(IP);
	IP = std::to_string(m_IP.d2);
}

void SceneGuest::SetIP3(std::string& IP)
{
	m_IP.d3 = std::stoi(IP);
	IP = std::to_string(m_IP.d3);
}

void SceneGuest::SetIP4(std::string& IP)
{
	m_IP.d4 = std::stoi(IP);
	IP = std::to_string(m_IP.d4);
}

void SceneGuest::SetPortNum(std::string& Port)
{
	m_Port = std::stoi(Port);
	Port = std::to_string(m_Port);
}

void SceneGuest::SetName(std::string& Name)
{
	// 名前エディットテキストのコールバック関数

	if (Name == "")
	{
		// 空文字への変更は却下
		Name = m_Name;
	}
	else
	{
		// 名前の変更
		LOG_INFO("自分の名前を変更：%s → %s", m_Name.c_str(), Name.c_str());
		m_Name = Name;

		// 自分の名前変更しましたメッセージがホストから戻ってくるので
		// チャットの表示名の変更はそこで行う

		if (m_ConnectStep == ConnectStep::ON)
		{
			// ホストに接続しているとき
			// 名前変更しましたメッセージを送信
			Command::Message msgSend = Command::MakeChangeNameMySelf(m_Name);
			Command::Send(m_NetHandle, msgSend);
		}
	}
}

void SceneGuest::TryConnect()
{
	// 接続を試みるボタンのコールバック関数
	LOG_INFO("接続を試みるボタン押下");

	if (m_ConnectStep == ConnectStep::OFF)
	{
		m_NetHandle = ConnectNetWork(m_IP, m_Port);
		if (m_NetHandle == -1)
		{
			LOG_INFO("接続失敗");

			// 接続失敗したらOFFのまま（一応代入）
			SetConnectStep(ConnectStep::OFF);
		}
		else
		{
			LOG_INFO("接続成功");

			// 接続成功したらまずは名乗る決まりなので
			// 自分の名前をホストに送信する
			Command::Message msgSend = Command::MakeChangeNameMySelf(m_Name);
			Command::Send(m_NetHandle, msgSend);

			// 承認待ち段階へ処理を進む
			SetConnectStep(ConnectStep::WAIT_ACCEPT);
		}
	}
	else
	{
		if (m_ConnectStep == ConnectStep::WAIT_ACCEPT)
			LOG_INFO("現在既に接続を試み中");
		else if (m_ConnectStep == ConnectStep::ON)
			LOG_INFO("現在既に接続済み");
	}
}

void SceneGuest::Disconnect()
{
	if (m_NetHandle != -1)
	{
		CloseNetWork(m_NetHandle);
		LOG_INFO("切断しました：NetHandle = %d", m_NetHandle);
		m_NetHandle = -1;
	}
	else
	{
		LOG_INFO("切断済み：NetHandle = %d", m_NetHandle);
	}
}

void SceneGuest::RequestAllUpdate()
{
	LOG_INFO("全更新リクエストボタン押下");

	if (m_ConnectStep == ConnectStep::ON)
	{
		// ホストに接続しているとき
		LOG_INFO("接続しているので実行");

		// 名前など自身のデータ以外すべて削除する
		m_ConnectingGuest->RemoveAllItem();
		m_Chat->ClearAll();

		// 全更新リクエストを送信
		Command::Message msgSend = Command::MakeAllUpdate();
		Command::Send(m_NetHandle, msgSend);
	}
	else
	{
		LOG_INFO("接続していないので実行しない");
	}
}

void SceneGuest::SetChatText(std::string& Text)
{
	// チャットテキストが入力されたときのコールバック関数

	if (m_ConnectStep == ConnectStep::ON)
	{
		LOG_INFO("チャットを送信する");

		// ホストにチャットを送信
		Command::Message msgSend = Command::MakeChatText(m_NetHandle, Text);
		Command::Send(m_NetHandle, msgSend);

		// ホストがチャットの反映を送り返してくるのでここではチャットに追加しない

		// チャット欄を空にする
		Text = "";
	}
}

void SceneGuest::End()
{
	LOG_INFO("終了ボタン押下：m_Next = %d", m_Next);

	if (m_Next == nullptr)
	{
		Disconnect();
		m_Next = SceneCreator::Create(SceneCreator::Name::CHOICE_HOST_GUEST);
	}
}

/*
 * m_ConnectStepを変更したらm_ConnectStateも変更する必要があるので
 * m_ConnectStepを変更するときは必ずこの関数で行う
 */
void SceneGuest::SetConnectStep(ConnectStep Step)
{
	m_ConnectStep = Step;
	m_ConnectState->ChangeText(CONNECT_STEP_STATE[static_cast<int>(m_ConnectStep)]);
}


