#include "SceneHost.h"
#include "ScButton.h"
#include "ScEdit.h"
#include "ScToggle.h"
#include "FontManager.h"
#include "BoundRect.h"
#include "Common.h"
#include "SceneCreator.h"
#include "Command.h"
#include "Logger.h"
#include "DataFileInOut.h"

const char SceneHost::ACCEPT_STEP_STATE[][64] =
{
	"新規ゲストは受け付けていません",
	"新規ゲストを受け付け中です...",
	"新規ゲストが訪問しています...",
	"以下のゲストが訪問してきました",
	"処理中..."
};

SceneHost::SceneHost()
	: m_AcceptRejectComponent()
	, m_Next()
	, m_Port(DataFileInOut::Inst().GetPortNum())
	, m_Name(DataFileInOut::Inst().GetName())
	, m_AcceptGuestStep()
	, m_AcceptState()
	, m_NewGuestIP()
	, m_NewGuestName()
	, m_NewGuestNetHandle(-1)
	, m_AcceptOrReject()
	, m_RequestNameTimeStart()
	, m_AcceptedGuest()
{
	// コンストラクタでは画面の部品を配置する
	Logger::Info("ホスト画面生成");

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

	AddBaseComponent(new ScLabel("あなたはホストです", GreenColor, X_START, y, SmallFont));

	y += Y_STEP_SMALL;
	AddBaseComponent(new ScLabel("ポート番号：", GreenColor, X_START, y, SmallFont));
	{
		BoundRect* bound = new BoundRect(X_POS, y, 200, Y_SIZE);
		auto Callback = new DelegateArg<SceneHost, std::string>(*this, &SceneHost::SetPortNum);
		AddBaseComponent(new ScEdit(std::to_string(m_Port), EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, 5, Callback));
	}

	y += Y_STEP_SMALL;
	AddBaseComponent(new ScLabel("名前：", GreenColor, X_START, y, SmallFont));
	{
		BoundRect* bound = new BoundRect(X_POS, y, 200, Y_SIZE);
		auto Callback = new DelegateArg<SceneHost, std::string>(*this, &SceneHost::SetName);
		AddBaseComponent(new ScEdit(m_Name, EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, DataFileInOut::NAME_LENGTH_MAX, Callback));
	}

	y += Y_STEP_SMALL * 2;
	AddBaseComponent(new ScLabel("新規ゲストの受付：", GreenColor, X_START, y, SmallFont));
	{
		BoundRect* bound = new BoundRect(170, y, 80, Y_SIZE);
		auto Callback = new DelegateArg<SceneHost, bool>(*this, &SceneHost::SetAcceptGuest);
		AddBaseComponent(new ScToggle(ToggleColorFore, ToggleColorBack, bound, SmallFont, Callback));
	}
	y += Y_STEP_SMALL * 2;
	m_AcceptState = new ScLabel(ACCEPT_STEP_STATE[0], RedColor, 10, y, MiddleFont);
	AddBaseComponent(m_AcceptState);
	y += Y_STEP_MIDDLE;
	m_AcceptRejectComponent.push_back(new ScLabel("IP：", GreenColor, X_START, y, SmallFont));
	m_NewGuestIP = new ScLabel("xxx.xxx.xx.xxx", GreenColor, 80, y, SmallFont);
	m_AcceptRejectComponent.push_back(m_NewGuestIP);
	y += Y_STEP_SMALL;
	m_AcceptRejectComponent.push_back(new ScLabel("名前：", GreenColor, X_START, y, SmallFont));
	m_NewGuestName = new ScLabel("Guest", GreenColor, 80, y, SmallFont);
	m_AcceptRejectComponent.push_back(m_NewGuestName);

	y += Y_STEP_MIDDLE;
	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(180, 180, 220);
		unsigned int ColorFrame = GetColor(50, 50, 100);
		unsigned int ColorBackHover = GetColor(200, 200, 240);
		unsigned int ColorBackPress = GetColor(160, 160, 200);
		auto Callback = new DelegateVoid<SceneHost>(*this, &SceneHost::AcceptGuest);
		m_AcceptRejectComponent.push_back(new ScButton("承認", ColorFore, ColorBack, X_START + 10, y, BigFont, ColorFrame, ColorBackHover, ColorBackPress, Callback));
	}
	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(220, 180, 180);
		unsigned int ColorFrame = GetColor(100, 50, 50);
		unsigned int ColorBackHover = GetColor(240, 200, 200);
		unsigned int ColorBackPress = GetColor(200, 160, 160);
		auto Callback = new DelegateVoid<SceneHost>(*this, &SceneHost::RejectGuest);
		m_AcceptRejectComponent.push_back(new ScButton("拒否", ColorFore, ColorBack, X_START + 110, y, BigFont, ColorFrame, ColorBackHover, ColorBackPress, Callback));
	}

	y += Y_STEP_BIG + Y_STEP_SMALL;
	AddBaseComponent(new ScLabel("承認済みゲスト：", GreenColor, X_START, y, SmallFont));

	y += Y_STEP_SMALL;
	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(210, 210, 210);
		unsigned int ColorFrame = GetColor(100, 50, 50);
		BoundRect* bound = new BoundRect(X_START + 10, y, 250, Y_SIZE);
		m_AcceptedGuest = new ScList(ColorFore, ColorBack, bound, TextAlignment::Gravity::LEFT, SmallFont, ColorFrame);
		AddBaseComponent(m_AcceptedGuest);
	}

	y = Common::WINDOW_Y_SIZE - Y_STEP_MIDDLE;
	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(180, 220, 180);
		unsigned int ColorFrame = GetColor(50, 100, 50);
		unsigned int ColorBackHover = GetColor(200, 240, 200);
		unsigned int ColorBackPress = GetColor(160, 200, 160);
		auto Callback = new DelegateVoid<SceneHost>(*this, &SceneHost::End);
		AddBaseComponent(new ScButton("終了", ColorFore, ColorBack, X_START, y, MiddleFont, ColorFrame, ColorBackHover, ColorBackPress, Callback));
	}
}

SceneHost::~SceneHost()
{
	Logger::Info("ホスト画面破棄");

	DataFileInOut::Inst().SetPortNum(m_Port);
	DataFileInOut::Inst().SetName(m_Name);

	Disconnect();
	DeleteComponent(m_AcceptRejectComponent);
}

Scene* SceneHost::Update()
{
	UpdateBase();
	ProcessNewGuest();
	ProcessDissconectGuest();
	ProcessReceiveCommand();
	return m_Next;
}

void SceneHost::Draw()
{
	ClearDrawScreen();
	DrawBase();
	switch (m_AcceptGuestStep)
	{
	case AcceptGuestStep::OFF:
		break;
	case AcceptGuestStep::WAIT_CONNECT:
		break;
	case AcceptGuestStep::REQUEST_NAME:
		break;
	case AcceptGuestStep::SELECT_ACCEPT_REJECT:
		// 接続を求めてきた相手のNetHandleと名前が分かったら
		// 承認・拒否を選択する画面部品を表示する
		DrawComponent(m_AcceptRejectComponent);
		break;
	case AcceptGuestStep::SELECTED:
		break;
	}
}

void SceneHost::ProcessNewGuest()
{
	switch (m_AcceptGuestStep)
	{
	case AcceptGuestStep::OFF:
		// 新規ゲストの受付OFFのとき
		break;
	case AcceptGuestStep::WAIT_CONNECT:
		// 新規ゲストの受付ONのとき

		// 新規接続を待つ
		m_NewGuestNetHandle = GetNewAcceptNetWork();
		if (m_NewGuestNetHandle != -1)
		{
			// 新しい接続を発見したら
			IPDATA ip = {};
			GetNetWorkIP(m_NewGuestNetHandle, &ip);
			Logger::Info("[AcceptGuestStep::WAIT_CONNECT] 新しい接続を発見：NetHandle = %d, IP = %d.%d.%d.%d", m_NewGuestNetHandle, ip.d1, ip.d2, ip.d3, ip.d4);

			Logger::Info("[AcceptGuestStep::WAIT_CONNECT] 現在の承認済みゲストの接続数：%d/%d", m_AcceptedGuest->GetItem().size(), ACCEPTED_GUEST_MAX);

			if (m_AcceptedGuest->GetItem().size() < ACCEPTED_GUEST_MAX)
			{
				// ゲストの接続数が最大数に達していない時
				Logger::Warn("[AcceptGuestStep::WAIT_CONNECT] ゲストの接続数が最大数に達していないのでAcceptGuestStep::REQUEST_NAMEへ進む");

				// 相手のIPを保持
				m_NewGuestIP->ChangeText(Common::IP2String(ip));
				// 処理をAcceptGuestStep::REQUEST_NAMEへ進む
				m_AcceptGuestStep = AcceptGuestStep::REQUEST_NAME;
				m_RequestNameTimeStart = GetNowCount();
			}
			else
			{
				// ゲストの接続数が最大数に達していた時
				Logger::Warn("[AcceptGuestStep::WAIT_CONNECT] ゲストの接続数が最大数に達しているので接続を拒否");

				// 新規接続しようとしているゲストに拒否メッセージを送信（このときホストは名乗らない）
				Command::Message msgSend = Command::MakeConnect(false, DUMMY_TEXT);
				Command::Send(m_NewGuestNetHandle, msgSend);
			}
		}
		break;
	case AcceptGuestStep::REQUEST_NAME:
	{
		// 接続してきた相手はまず自分の名前を名乗るはずなのでそれを待つ

		bool Reject = false;

		Command::ReceiveResult result = Command::CheckReceive(m_NewGuestNetHandle);
		if (result == Command::ReceiveResult::SUCCESS)
		{
			// 相手からメッセージが送られてきたら
			Command::Message Msg = Command::Receive(m_NewGuestNetHandle);

			if (Msg.type == Command::Type::CHANGE_NAME_MYSELF)
			{
				// 相手が名乗ってきたら
				Logger::Warn("[AcceptGuestStep::REQUEST_NAME] 名乗りメッセージであるのでAcceptGuestStep::SELECT_ACCEPT_REJECTへ進む");

				// 相手の名前を保持
				m_NewGuestName->ChangeText(Msg.string.text);
				// 処理をAcceptGuestStep::SELECT_ACCEPT_REJECTへ進む
				m_AcceptGuestStep = AcceptGuestStep::SELECT_ACCEPT_REJECT;
			}
			else
			{
				// 相手が名乗りではないメッセージを送ってきたら
				Logger::Warn("[AcceptGuestStep::REQUEST_NAME] 名乗りメッセージではない");
				// 拒否
				Reject = true;
			}
		}
		else if (result == Command::ReceiveResult::NONE)
		{
			// 何のメッセージも受け取っていないとき
			if (GetNowCount() - m_RequestNameTimeStart >= REQUEST_NAME_TIMEOUT)
			{
				// タイムアウト
				Logger::Warn("[AcceptGuestStep::REQUEST_NAME] タイムアウト");
				// 拒否
				Reject = true;
			}
		}
		else
		{
			// 拒否
			Reject = true;
		}

		if (Reject)
		{
			Logger::Warn("[AcceptGuestStep::REQUEST_NAME] 接続を拒否してAcceptGuestStep::WAIT_CONNECTへ戻る");

			// 新規接続しようとしているゲストに拒否メッセージを送信（このときホストは名乗らない）
			Command::Message msgSend = Command::MakeConnect(false, DUMMY_TEXT);
			Command::Send(m_NewGuestNetHandle, msgSend);

			// 処理をAcceptGuestStep::WAIT_CONNECTへ戻る
			m_AcceptGuestStep = AcceptGuestStep::WAIT_CONNECT;
		}
	}
	break;
	case AcceptGuestStep::SELECT_ACCEPT_REJECT:
		// 接続を求めてきた相手のNetHandleと名前が分かったら
		// 承認・拒否を選択する画面部品を表示（ここでは表示ではなく処理）する
		UpdateComponent(m_AcceptRejectComponent);
		break;
	case AcceptGuestStep::SELECTED:
		// 承認・拒否が選択されたら（承認・拒否の決定の瞬間はコールバック関数）
		if (m_AcceptOrReject)
		{
			// 承認する場合
			Logger::Info("[AcceptGuestStep::SELECTED] 承認");

			// 承認メッセージを送信（このときホストも名乗り返す）
			Command::Message msgSend = Command::MakeConnect(true, m_Name);
			Command::Send(m_NewGuestNetHandle, msgSend);

			// 承認済みゲストに追加
			m_AcceptedGuest->AddItem(m_NewGuestNetHandle, m_NewGuestName->GetText());

			// 全承認済みゲストに新規ゲストの接続を知らせる（新規ゲスト本人を含まない）
			Logger::Info("[AcceptGuestStep::SELECTED] 全承認済みゲストに新規ゲストの接続を知らせる（新規ゲスト本人を含まない）");
			auto& list = m_AcceptedGuest->GetItem();
			for (auto& pair : list)
			{
				int ID = pair.first;
				Logger::Info("[AcceptGuestStep::SELECTED] ID = %d, Name = %s", ID, pair.second.Text.c_str());

				if (ID == m_NewGuestNetHandle)
				{
					Logger::Info("[AcceptGuestStep::SELECTED] 新規ゲスト本人なのでメッセージを送信しない");
					continue;
				}
				Command::Message msgSend2 = Command::MakeNewGuest(m_NewGuestNetHandle, m_NewGuestName->GetText());
				Command::Send(ID, msgSend2);
			}

			// 新規ゲストに全承認済みゲストを知らせる（新規ゲスト本人を含まない）
			Logger::Info("[AcceptGuestStep::SELECTED] 新規ゲストに全承認済みゲストを知らせる（新規ゲスト本人を含まない）");
			for (auto& pair : list)
			{
				int ID = pair.first;
				Logger::Info("[AcceptGuestStep::SELECTED] ID = %d, Name = %s", ID, pair.second.Text.c_str());

				if (ID == m_NewGuestNetHandle)
				{
					Logger::Info("[AcceptGuestStep::SELECTED] 新規ゲスト本人なのでメッセージを送信しない");
					continue;
				}
				Command::Message msgSend2 = Command::MakeNewGuest(ID, pair.second.Text);
				Command::Send(m_NewGuestNetHandle, msgSend2);
			}
		}
		else
		{
			// 拒否する場合
			Logger::Info("[AcceptGuestStep::SELECTED] 拒否");

			// 拒否メッセージを送信（このときホストは名乗らない）
			Command::Message msgSend = Command::MakeConnect(false, DUMMY_TEXT);
			Command::Send(m_NewGuestNetHandle, msgSend);
		}
		// 処理をAcceptGuestStep::WAIT_CONNECTへ戻る
		m_AcceptGuestStep = AcceptGuestStep::WAIT_CONNECT;
		break;
	}
}

void SceneHost::ProcessDissconectGuest()
{
	// ゲストはいつ切断してくるか分からないので常に確認しておく
	bool f = false;
	int LostHandle = GetLostNetWork();
	if (LostHandle == -1)
	{
		return;
	}

	Logger::Info("切断を発見：NetHandle = %d", LostHandle);

	auto& list = m_AcceptedGuest->GetItem();
	for (auto& pair : list)
	{
		int ID = pair.first;
		if (LostHandle == ID)
		{
			// 切断してるゲストを発見したら
			// 承認済みゲストから削除
			m_AcceptedGuest->RemoveItem(ID);
			f = true;

			Logger::Info("切断者のIDを承認済みゲスト内に発見したので削除", LostHandle);
			break;
		}
	}
	if (f)
	{
		// ゲストを削除していたら
		// 全承認済みゲストに切断したゲストがいることを知らせる
		Command::Message msgSend = Command::MakeDisconnectGuest(LostHandle);
		for (auto& pair : list)
		{
			int ID = pair.first;
			Logger::Info("ID = %d, Name = %s", ID, pair.second.Text.c_str());
			Command::Send(ID, msgSend);
		}
	}
}
void SceneHost::ProcessReceiveCommand()
{
	// 全承認済みゲストからメッセージの受信を確認していく
	auto& list = m_AcceptedGuest->GetItem();
	for (auto& pair : list)
	{
		int ID = pair.first;

		Command::ReceiveResult result = Command::CheckReceive(m_NewGuestNetHandle);
		if (result == Command::ReceiveResult::SUCCESS)
		{
			// 相手からメッセージが送られてきたら
			Command::Message Msg = Command::Receive(m_NewGuestNetHandle);

			switch (Msg.type)
			{
			case Command::Type::MESSAGE:
				// ゲストがチャットを打ったとのこと
				break;
			case Command::Type::CHANGE_NAME_MYSELF:
			{
				// ゲストが名前の変更をしたとのこと

				// 届いて来た名前に変更する
				m_AcceptedGuest->ChangeText(ID, Msg.string.text);

				// 全承認済みゲストに知らせる（名前を変えたゲスト本人を除く）
				Command::Message msgSend = Command::MakeChangeNameGuest(ID, Msg.string.text);
				auto& list2 = m_AcceptedGuest->GetItem();
				for (auto& pair : list2)
				{
					int ID2 = pair.first;
					Logger::Info("ID = %d, Name = %s", ID2, pair.second.Text.c_str());

					if (ID2 == ID)
					{
						Logger::Info("名前を変更したゲスト本人なのでメッセージを送信しない");
						continue;
					}
					Command::Send(ID2, msgSend);
				}
			}
			break;
			case Command::Type::ALL_UPDATE:
			{
				// 全データを更新したいとのこと

				// 接続中のゲストを全て知らせる
				auto& list2 = m_AcceptedGuest->GetItem();
				for (auto& pair : list2)
				{
					int ID2 = pair.first;
					Logger::Info("ID = %d, Name = %s", ID2, pair.second.Text.c_str());

					if (ID2 == ID)
					{
						Logger::Info("全データ更新したいゲスト本人なのでメッセージを送信しない");
						continue;
					}
					Command::Message msgSend = Command::MakeNewGuest(ID2, pair.second.Text.c_str());
					Command::Send(ID, msgSend);
				}
			}
			break;
			default:
				break;
			}

		}
	}
}

void SceneHost::SetPortNum(std::string& Port)
{
	m_Port = std::atoi(Port.c_str());
	Port = std::to_string(m_Port);
}

void SceneHost::SetName(std::string& Name)
{
	if (Name == "")
	{
		Name = m_Name;
	}
	else
	{
		Logger::Info("ホスト名を変更：%s → %s", m_Name.c_str(), Name.c_str());

		m_Name = Name;
		auto& list = m_AcceptedGuest->GetItem();
		for (auto& pair : list)
		{
			int ID = pair.first;
			Logger::Info("ID = %d, Name = %s", ID, pair.second.Text.c_str());

			Command::Message msgSend = Command::MakeChangeNameMySelf(m_Name);
			Command::Send(ID, msgSend);
		}
	}
}

void SceneHost::SetAcceptGuest(bool& OffOn)
{
	// 新規ゲストの受付トグルのコールバック関数
	Logger::Info("新規ゲストの受付トグル：%sに変更を試みる", OffOn ? "true" : "false");

	if (m_AcceptGuestStep == AcceptGuestStep::OFF)
	{
		// OFFのときはいつでもONにしてOK
		if (OffOn == true)
			m_AcceptGuestStep = AcceptGuestStep::WAIT_CONNECT;
	}
	else if (m_AcceptGuestStep == AcceptGuestStep::WAIT_CONNECT)
	{
		// 新規ゲストが接続を試みていないときはOFFにしてOK
		if (OffOn == false)
			m_AcceptGuestStep = AcceptGuestStep::OFF;
	}
	else
	{
		// 新規ゲストが接続を試みているときはONしか許さない
		OffOn = true;
	}

	// m_AcceptGuestStepの段階がどの状態かを表すラベルにも変更を反映
	m_AcceptState->ChangeText(ACCEPT_STEP_STATE[static_cast<int>(m_AcceptGuestStep)]);

	// DxLibの接続受付状態の変更
	if (OffOn)
		PreparationListenNetWork(m_Port);
	else
		StopListenNetWork();

	Logger::Info("新規ゲストの受付トグル：%sに変更", OffOn ? "true" : "false");
}

void SceneHost::AcceptGuest()
{
	// 新規ゲストの承認ボタンのコールバック関数
	Logger::Info("承認ボタン押下");

	// プログラム上 100% true になるはずの条件
	if (m_AcceptGuestStep == AcceptGuestStep::SELECT_ACCEPT_REJECT)
	{
		// 承認フラグを立てて次の段階に進む
		m_AcceptOrReject = true;
		m_AcceptGuestStep = AcceptGuestStep::SELECTED;
		m_AcceptState->ChangeText(ACCEPT_STEP_STATE[static_cast<int>(m_AcceptGuestStep)]);
	}
	else
	{
		Logger::Warn("このログは出力されてはならない：m_AcceptGuestStep = %d", m_AcceptGuestStep);
	}
}

void SceneHost::RejectGuest()
{
	// 新規ゲストの拒否ボタンのコールバック関数
	Logger::Info("拒否ボタン押下");

	// プログラム上 100% true になるはずの条件
	if (m_AcceptGuestStep == AcceptGuestStep::SELECT_ACCEPT_REJECT)
	{
		// 拒否フラグを立てて次の段階に進む
		m_AcceptOrReject = false;
		m_AcceptGuestStep = AcceptGuestStep::SELECTED;
		m_AcceptState->ChangeText(ACCEPT_STEP_STATE[static_cast<int>(m_AcceptGuestStep)]);
	}
	else
	{
		Logger::Warn("このログは出力されてはならない：m_AcceptGuestStep = %d", m_AcceptGuestStep);
	}
}

void SceneHost::Disconnect()
{
	// 全承認済みゲストにホストが切断することを知らせる
	Logger::Info("全承認済みゲストにホストが切断することを知らせる");

	Command::Message msgSend = Command::MakeConnect(false, m_Name);
	auto& list = m_AcceptedGuest->GetItem();
	for (auto& pair : list)
	{
		int ID = pair.first;
		Logger::Info("ID = %d, Name = %s", ID, pair.second.Text.c_str());
		Command::Send(ID, msgSend);
	}
	m_AcceptedGuest->RemoveAllItem();
}

void SceneHost::End()
{
	Logger::Info("終了ボタン押下：m_Next = %d", m_Next);

	if (m_Next == nullptr)
	{
		Disconnect();
		m_Next = SceneCreator::Create(SceneCreator::Name::CHOICE_HOST_GUEST);
	}
}

