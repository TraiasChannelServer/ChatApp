#pragma once

/*
 * ゲスト専用画面
 */

#include "Scene.h"
#include "ScLabel.h"
#include "ScList.h"
#include "ScChat.h"
#include <string>

class SceneGuest : public Scene
{
private:
	// 接続の段階
	enum class ConnectStep
	{
		OFF,
		WAIT_ACCEPT,
		ON
	};

	// 接続の段階を表す文字列
	static const char CONNECT_STEP_STATE[][64];

public:
	SceneGuest();
	~SceneGuest();

public:
	Scene* Update() override;
	void Draw() override;

private:
	// コールバック関数
	void SetIP1(std::string& IP);
	void SetIP2(std::string& IP);
	void SetIP3(std::string& IP);
	void SetIP4(std::string& IP);
	void SetPortNum(std::string& Port);
	void SetName(std::string& Name);
	void TryConnect();
	void Disconnect();
	void RequestAllUpdate();
	void SetChatText(std::string& Text);
	void SendChatText();
	void End();

	void SetConnectStep(ConnectStep Step);

private:
	Scene* m_Next;

	// ホストのIP,Port
	IPDATA m_IP;
	int m_Port;

	// ゲストの名前
	std::string m_Name;

	// 接続の段階を管理する
	ConnectStep m_ConnectStep;

	// 接続の段階を文字列で所持する画面部品
	ScLabel* m_ConnectState;

	// 接続したホストの情報
	int m_NetHandle;
	ScLabel* m_HostName;

	// 同じホストに接続しているゲスト達を所持する画面部品
	ScList* m_ConnectingGuest;

	ScChat* m_Chat;
	std::string m_ChatText;
};