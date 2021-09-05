#include "Command.h"
#include "FormatToString.h"
#include "Logger.h"
#include "DxLib.h"
#include <memory>

/*
 * ホストがゲストに接続を許可するかしないかを示すメッセージ
 * 
 * @param AcceptReject true:する, false:しない
 * @param Name         ホスト名
 */
Command::Message Command::MakeConnect(bool AcceptReject, const std::string& Name)
{
	Message msg = {};
	msg.type = Type::CONNECT;
	msg.single.flag = AcceptReject;
	memcpy_s(msg.string.text, sizeof(msg.string.text), Name.c_str(), Name.size());
	return msg;
}

/*
 * チャットメッセージ
 *
 * @param ID            チャットメッセージの送信者のID
 * @param MessageString チャットメッセージ
 */
Command::Message Command::MakeMessage(int ID, const std::string& MessageString)
{
	Message msg = {};
	msg.type = Type::MESSAGE;
	msg.single.num = ID;
	memcpy_s(msg.string.text, sizeof(msg.string.text), MessageString.c_str(), MessageString.size());
	return msg;
}

/*
 * 自分の名前を変更したことを知らせるメッセージ
 *
 * @param Name 変更後の名前
 */
Command::Message Command::MakeChangeNameMySelf(const std::string& Name)
{
	Message msg = {};
	msg.type = Type::CHANGE_NAME_MYSELF;
	memcpy_s(msg.string.text, sizeof(msg.string.text), Name.c_str(), Name.size());
	return msg;
}

/*
 * ホストがゲストに名前を変更したゲストがいることを知らせるメッセージ
 *
 * @param ID   名前を変更した人のID
 * @param Name 変更後の名前
 */
Command::Message Command::MakeChangeNameGuest(int ID, const std::string& Name)
{
	Message msg = {};
	msg.type = Type::CHANGE_NAME_GUEST;
	msg.single.num = ID;
	memcpy_s(msg.string.text, sizeof(msg.string.text), Name.c_str(), Name.size());
	return msg;
}

/*
 * ホストがゲストに新しいゲストが接続したことを知らせるメッセージ
 *
 * @param ID   新しいゲストのID
 * @param Name 新しいゲストの名前
 */
Command::Message Command::MakeNewGuest(int ID, const std::string& Name)
{
	Message msg = {};
	msg.type = Type::NEW_GUEST;
	msg.single.num = ID;
	memcpy_s(msg.string.text, sizeof(msg.string.text), Name.c_str(), Name.size());
	return msg;
}

/*
 * ホストがゲストに誰かゲストが切断したことを知らせるメッセージ
 *
 * @param ID 切断したゲストのID
 */
Command::Message Command::MakeDisconnectGuest(int ID)
{
	Message msg = {};
	msg.type = Type::DISCONNECT_GUEST;
	msg.single.num = ID;
	return msg;
}

/*
 * ゲストがホストに全データを再読み込みしたいことを知らせるメッセージ
 *
 * ホストはゲストにメッセージを送りまくるので
 * ゲストは自身のデータ以外全部消去して待機する
 */
Command::Message Command::MakeAllUpdate()
{
	Message msg = {};
	msg.type = Type::ALL_UPDATE;
	return msg;
}

/*
 * メッセージを送信する
 */
void Command::Send(int NetHandle, const Message& Msg)
{
	NetWorkSend(NetHandle, &Msg, Command::SIZEOF_MESSAGE);

	LOG_INFO("メッセージを送信：NetHandle = %d, Msg = {Type = %s, Single = %d, String = %s}"
		, NetHandle, Command::TypeToString(Msg.type), Msg.single.num, Msg.string.text);
}

/*
 * メッセージを受信しているかを確認する
 * 
 * @return ReceiveResult::NONE     … 受信していない
 *         ReceiveResult::SUCCESS  … 受信している
 *         ReceiveResult::ABNORMAL … 固定長未満のメッセージを受信したので捨てた
 */
Command::ReceiveResult Command::CheckReceive(int NetHandle)
{
	int DataLength = GetNetWorkDataLength(NetHandle);
	if (DataLength >= Command::SIZEOF_MESSAGE)
	{
		// 固定長以上のメッセージが送られてきたとき
		return ReceiveResult::SUCCESS;
	}
	else if (DataLength > 0)
	{
		// 固定長未満のメッセージが送られてきたとき
		// メッセージを読み捨てる
		char Buf[Command::SIZEOF_MESSAGE] = {};
		NetWorkRecv(NetHandle, &Buf, DataLength);
		LOG_WARN("固定長より短いメッセージが届いたので捨てる：NetHandle = %d", NetHandle);

		return ReceiveResult::ABNORMAL;
	}
	return ReceiveResult::NONE;
}

/*
 * メッセージを受信する
 */
Command::Message Command::Receive(int NetHandle)
{
	Command::Message Msg = {};
	NetWorkRecv(NetHandle, &Msg, Command::SIZEOF_MESSAGE);

	LOG_INFO("メッセージを受信：NetHandle = %d, Msg = {Type = %s, Single = %d, String = %s}"
		, NetHandle, Command::TypeToString(Msg.type), Msg.single.num, Msg.string.text);

	return Msg;
}

const char* Command::TypeToString(Type type)
{
	switch (type)
	{
	case Type::CONNECT:
		return "Command::Type::CONNECT";
	case Type::MESSAGE:
		return "Command::Type::MESSAGE";
	case Type::CHANGE_NAME_MYSELF:
		return "Command::Type::CHANGE_NAME_MYSELF";
	case Type::CHANGE_NAME_GUEST:
		return "Command::Type::CHANGE_NAME_GUEST";
	case Type::NEW_GUEST:
		return "Command::Type::NEW_GUEST";
	case Type::DISCONNECT_GUEST:
		return "Command::Type::DISCONNECT_GUEST";
	case Type::ALL_UPDATE:
		return "Command::Type::ALL_UPDATE";
	}

	static std::string ErrorText;
	ErrorText = FormatToString::Make("Command::Type::範囲外(%d)", static_cast<int>(type));
	return ErrorText.c_str();
}

