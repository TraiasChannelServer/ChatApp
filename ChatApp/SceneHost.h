#pragma once

/*
 * ホスト専用画面
 */

#include "Scene.h"
#include "ScLabel.h"
#include "ScList.h"
#include <string>

class SceneHost : public Scene
{
private:
	// 新規ゲストの接続を受け付ける段階
	// これを変更したら ACCEPT_STEP_STATE も変更すること
	enum class AcceptGuestStep
	{
		// 受付OFF
		OFF,

		// 以下全部受付ON
		WAIT_CONNECT,
		REQUEST_NAME,
		SELECT_ACCEPT_REJECT,
		SELECTED
	};

	// 新規ゲストの接続を受け付ける段階を表す文字列
	static const char ACCEPT_STEP_STATE[][64];

	static constexpr char DUMMY_TEXT[] = "---";
	static constexpr int REQUEST_NAME_TIMEOUT = 5000; // ミリ秒
	static constexpr int ACCEPTED_GUEST_MAX = 10; // 接続可能な最大人数

public:
	SceneHost();
	~SceneHost();

public:
	Scene* Update() override;
	void Draw() override;

private:
	void ProcessNewGuest();
	void ProcessDissconectGuest();
	void ProcessReceiveCommand();

	// コールバック関数
	void SetPortNum(std::string& Port);
	void SetName(std::string& Name);
	void SetAcceptGuest(bool& OffOn);
	void AcceptGuest();
	void RejectGuest();
	void Disconnect();
	void End();

	void SetAcceptGuestStep(AcceptGuestStep Step);

private:
	// 承認・拒否を選択するときにだけ表示する部品
	std::vector<ScreenComponent*> m_AcceptRejectComponent;

	Scene* m_Next;

	// ホストのポート、名前
	int m_Port;
	std::string m_Name;

	// 新規ゲストの接続を受け付ける段階を管理する
	AcceptGuestStep m_AcceptGuestStep;

	// 新規ゲストの接続の段階を進む中で使う変数
	ScLabel* m_AcceptState;
	ScLabel* m_NewGuestIP;
	ScLabel* m_NewGuestName;
	int m_NewGuestNetHandle;
	bool m_AcceptOrReject;
	int m_RequestNameTimeStart;

	// 承認されたゲスト達を所持する画面部品
	ScList* m_AcceptedGuest;
};