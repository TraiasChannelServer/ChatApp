#include "KeyInputter.h"
#include "Common.h"
#include "DxLib.h"

#include <memory>

KeyInputter::KeyInputter()
	: m_Handle(-1)
	, m_User(nullptr)
	, m_X()
	, m_Y()
	, m_Length()
{
}

/*
 * この関数を呼んだ後は必ずEnd関数を呼ぶこと
 * 
 * @param User     キー入力を使用するクラスのアドレス
 * @param x        入力中の文字列を描画するX座標
 * @param y        入力中の文字列を描画するY座標
 * @param Length   入力できる最大文字数（半角基準サイズ, \0を含まない）
 * @param InitText 入力開始時点からの初期文字列
 */
bool KeyInputter::Start(const void* User, int x, int y, int Length, const std::string& InitText)
{
	if (GetState(User) != State::FREE)
	{
		return false;
	}

	m_Handle = MakeKeyInput(Length, TRUE, FALSE, FALSE);

	if (m_Handle == -1)
	{
		return false;
	}

	m_User = User;
	m_X = x;
	m_Y = y;
	m_Length = Length;

	SetKeyInputString(InitText.c_str(), m_Handle);
	SetActiveKeyInput(m_Handle);

	return true;
}

/*
 * @return
 *     誰も使ってない      ：State::FREE
 *     使用者と引数が不一致：State::INVALID
 *     使用者と引数が一致  ：入力中：State::ACTIVE
 *                           終了  ：State::SUCCESS or CANCEL or FAIL
 */
KeyInputter::State KeyInputter::GetState(const void* User)
{
	if (User == nullptr)
	{
		return State::INVALID;
	}

	if (m_User == nullptr || m_Handle == -1)
	{
		return State::FREE;
	}

	if (User != m_User)
	{
		return State::INVALID;
	}

	switch (CheckKeyInput(m_Handle))
	{
	case 0: return State::ACTIVE;
	case 1: return State::SUCCESS;
	case 2: return State::CANCEL;
	}
	return State::FAIL;
}

/*
 * @return true:End関数を呼んで結果を受け取れる状態である
 */
bool KeyInputter::IsStateFinish(const void* User)
{
	State state = GetState(User);
	if ((state == State::SUCCESS)
		|| (state == State::CANCEL)
		|| (state == State::FAIL))
	{
		return true;
	}
	return false;
}

/*
 * キー入力結果の文字列を受け取ると同時に
 * クラスの状態をまたStart関数が呼べる状態にする
 * 
 * GetState(User) == State::SUCCESSのときに
 * 一度だけ戻り値を受け取ることができる
 */
std::string KeyInputter::End(const void* User)
{
	if (GetState(User) != State::SUCCESS)
	{
		DeleteKeyInput(m_Handle);
		m_Handle = -1;
		m_User = nullptr;

		return std::string();
	}

	// 入力できる最大文字数はm_Lengthだが、受け取るときは'\0'があるので +1 している
	std::unique_ptr<char[]> Buf = std::unique_ptr<char[]>(new char[m_Length + 1]);
	GetKeyInputString(Buf.get(), m_Handle);

	DeleteKeyInput(m_Handle);
	m_Handle = -1;
	m_User = nullptr;

	return std::string(Buf.get());
}

/*
 * 入力中のときだけその様子を描画する
 */
void KeyInputter::Draw()
{
	if (GetState(m_User) != State::ACTIVE)
	{
		return;
	}
	DrawKeyInputString(m_X, m_Y, m_Handle);
	DrawKeyInputModeString(Common::WINDOW_X_SIZE - 100, Common::WINDOW_Y_SIZE - 16);
}
