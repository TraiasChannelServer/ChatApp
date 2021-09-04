#pragma once

/*
 * デリゲートパターン
 * 
 * 特定のインスタンスの特定の関数を所持できる
 * コールバック関数に使う
 * 
 * void Func(_任意の引数一つ_&)
 * 引数が参照であるので、呼び出し先でその引数の値を変更できる
 * 
 * 想定している使われ方：
 * １．この値に変更しましたよとコールバック関数を呼び出す
 * ２．呼び出し先がその値はダメですと値の変更を許さないことができる
 */

template<class T>
class DelegateArgBase
{
public:
	virtual void operator()(T& Value) = 0;
	virtual ~DelegateArgBase() = default;
};

template<class T, class U>
class DelegateArg : public DelegateArgBase<U>
{
public:
	using EventFunc = void (T::*)(U&);

public:
	DelegateArg(T& Obj, EventFunc Fnc)
		: m_Obj(Obj)
		, m_Fnc(Fnc)
	{
	}

	void operator()(U& Value) override
	{
		(m_Obj.*m_Fnc)(Value);
	}

private:
	T& m_Obj;
	EventFunc m_Fnc;
};