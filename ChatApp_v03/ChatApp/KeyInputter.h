#pragma once

#include <string>

class KeyInputter
{
public:
	enum class State
	{
		INVALID,
		FREE,
		ACTIVE,
		SUCCESS,
		CANCEL,
		FAIL
	};

public:
	static KeyInputter& Inst();

private:
	KeyInputter();
	KeyInputter(const KeyInputter&) = delete;
	KeyInputter(KeyInputter&&) = delete;
	KeyInputter& operator=(const KeyInputter&) = delete;
	KeyInputter& operator=(KeyInputter&&) = delete;

public:
	bool Start(const void* User, int x, int y, int Length);
	State GetState(const void* User);
	bool IsStateFinish(const void* User);
	std::string End(const void* User);
	void Draw();

private:
	int m_Handle;
	const void* m_User;
	int m_X;
	int m_Y;
	int m_Length;
};

inline KeyInputter& KeyInputter::Inst()
{
	static KeyInputter s_Inst;
	return s_Inst;
}

