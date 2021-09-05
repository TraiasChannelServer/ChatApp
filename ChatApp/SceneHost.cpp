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
	"�V�K�Q�X�g�͎󂯕t���Ă��܂���",
	"�V�K�Q�X�g���󂯕t�����ł�...",
	"�V�K�Q�X�g���K�₵�Ă��܂�...",
	"�ȉ��̃Q�X�g���K�₵�Ă��܂���",
	"������..."
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
	, m_Chat()
	, m_ChatText()
{
	// �R���X�g���N�^�ł͉�ʂ̕��i��z�u����
	LOG_INFO("�z�X�g��ʐ���");

	int TinyFont = FontManager::Inst().GetFontHandle(FontManager::Type::TINY);
	int SmallFont = FontManager::Inst().GetFontHandle(FontManager::Type::SMALL);
	int MiddleFont = FontManager::Inst().GetFontHandle(FontManager::Type::MIDDLE);
	int BigFont = FontManager::Inst().GetFontHandle(FontManager::Type::BIG);

	unsigned int BlackColor = GetColor(0, 0, 0);
	unsigned int WhiteColor = GetColor(255, 255, 255);
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

	AddBaseComponent(new ScLabel("���Ȃ��̓z�X�g�ł�", GreenColor, X_START, y, SmallFont));

	y += Y_STEP_SMALL;
	AddBaseComponent(new ScLabel("�|�[�g�ԍ��F", GreenColor, X_START, y, SmallFont));
	{
		BoundRect* bound = new BoundRect(X_POS, y, 200, Y_SIZE);
		auto Callback = new DelegateArg<SceneHost, std::string>(*this, &SceneHost::SetPortNum);
		AddBaseComponent(new ScEdit(std::to_string(m_Port), EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, 5, Callback));
	}

	y += Y_STEP_SMALL;
	AddBaseComponent(new ScLabel("���O�F", GreenColor, X_START, y, SmallFont));
	{
		BoundRect* bound = new BoundRect(X_POS, y, 200, Y_SIZE);
		auto Callback = new DelegateArg<SceneHost, std::string>(*this, &SceneHost::SetName);
		AddBaseComponent(new ScEdit(m_Name, EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, DataFileInOut::NAME_LENGTH_MAX, Callback));
	}

	y += Y_STEP_SMALL * 2;
	AddBaseComponent(new ScLabel("�V�K�Q�X�g�̎�t�F", GreenColor, X_START, y, SmallFont));
	{
		BoundRect* bound = new BoundRect(170, y, 80, Y_SIZE);
		auto Callback = new DelegateArg<SceneHost, bool>(*this, &SceneHost::SetAcceptGuest);
		AddBaseComponent(new ScToggle(ToggleColorFore, ToggleColorBack, bound, SmallFont, Callback));
	}
	y += Y_STEP_SMALL * 2;
	m_AcceptState = new ScLabel(ACCEPT_STEP_STATE[0], RedColor, 10, y, MiddleFont);
	AddBaseComponent(m_AcceptState);
	y += Y_STEP_MIDDLE;
	m_AcceptRejectComponent.push_back(new ScLabel("IP�F", GreenColor, X_START, y, SmallFont));
	m_NewGuestIP = new ScLabel("xxx.xxx.xx.xxx", GreenColor, 80, y, SmallFont);
	m_AcceptRejectComponent.push_back(m_NewGuestIP);
	y += Y_STEP_SMALL;
	m_AcceptRejectComponent.push_back(new ScLabel("���O�F", GreenColor, X_START, y, SmallFont));
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
		m_AcceptRejectComponent.push_back(new ScButton("���F", ColorFore, ColorBack, X_START + 10, y, BigFont, ColorFrame, ColorBackHover, ColorBackPress, Callback));
	}
	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(220, 180, 180);
		unsigned int ColorFrame = GetColor(100, 50, 50);
		unsigned int ColorBackHover = GetColor(240, 200, 200);
		unsigned int ColorBackPress = GetColor(200, 160, 160);
		auto Callback = new DelegateVoid<SceneHost>(*this, &SceneHost::RejectGuest);
		m_AcceptRejectComponent.push_back(new ScButton("����", ColorFore, ColorBack, X_START + 110, y, BigFont, ColorFrame, ColorBackHover, ColorBackPress, Callback));
	}

	y += Y_STEP_BIG + Y_STEP_SMALL;
	AddBaseComponent(new ScLabel("���F�ς݃Q�X�g�F", GreenColor, X_START, y, SmallFont));

	y += Y_STEP_SMALL;
	{
		// ���F�ς݃Q�X�g���X�g
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(210, 210, 210);
		unsigned int ColorFrame = GetColor(100, 50, 50);
		BoundRect* bound = new BoundRect(X_START + 10, y, 250, Y_SIZE);
		m_AcceptedGuest = new ScList(ColorFore, ColorBack, bound, TextAlignment::Gravity::LEFT, SmallFont, ColorFrame);
		AddBaseComponent(m_AcceptedGuest);
	}

	{
		static constexpr int DELTA_WIDTH = 120;
		unsigned int ColorHead = GetColor(0, 200, 0);
		unsigned int ColorBody = WhiteColor;
		unsigned int ColorFrame = GetColor(150, 180, 180);
		int XPos = Common::WINDOW_X_SIZE / 2 - DELTA_WIDTH;
		int YPos = 20;
		int XSize = Common::WINDOW_X_SIZE / 2 + DELTA_WIDTH - 20;
		int YSize = Common::WINDOW_Y_SIZE - 40 - Y_STEP_MIDDLE;
		{
			// �`���b�g�g
			BoundRect* bound = new BoundRect(XPos, YPos, XSize, YSize);
			m_Chat = new ScChat(bound, TinyFont, SmallFont, ColorHead, ColorBody, ColorFrame);
			AddBaseComponent(m_Chat);
		}
		{
			// �`���b�g���͘g
			BoundRect* bound = new BoundRect(XPos, Common::WINDOW_Y_SIZE - Y_STEP_MIDDLE, XSize, Y_SIZE);
			auto Callback = new DelegateArg<SceneHost, std::string>(*this, &SceneHost::SetChatText);
			AddBaseComponent(new ScEdit(m_ChatText, EditColorFore, EditColorBack, bound, SmallFont, EditColorFrame, 64, Callback));
		}
		{
			unsigned int ColorFore = BlackColor;
			unsigned int ColorBack = GetColor(180, 220, 220);
			unsigned int ColorFrame = GetColor(50, 100, 100);
			unsigned int ColorBackHover = GetColor(200, 240, 240);
			unsigned int ColorBackPress = GetColor(160, 200, 200);
			auto Callback = new DelegateVoid<SceneHost>(*this, &SceneHost::SendChatText);
			AddBaseComponent(new ScButton("���M", ColorFore, ColorBack, XPos - 50, Common::WINDOW_Y_SIZE - Y_STEP_MIDDLE, MiddleFont, ColorFrame, ColorBackHover, ColorBackPress, Callback));
		}
	}

	{
		unsigned int ColorFore = BlackColor;
		unsigned int ColorBack = GetColor(180, 220, 180);
		unsigned int ColorFrame = GetColor(50, 100, 50);
		unsigned int ColorBackHover = GetColor(200, 240, 200);
		unsigned int ColorBackPress = GetColor(160, 200, 160);
		auto Callback = new DelegateVoid<SceneHost>(*this, &SceneHost::End);
		AddBaseComponent(new ScButton("�I��", ColorFore, ColorBack, X_START, Common::WINDOW_Y_SIZE - Y_STEP_MIDDLE, MiddleFont, ColorFrame, ColorBackHover, ColorBackPress, Callback));
	}
}

SceneHost::~SceneHost()
{
	LOG_INFO("�z�X�g��ʔj��");

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
		// �ڑ������߂Ă��������NetHandle�Ɩ��O������������
		// ���F�E���ۂ�I�������ʕ��i��\������
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
		// �V�K�Q�X�g�̎�tOFF�̂Ƃ�
		break;
	case AcceptGuestStep::WAIT_CONNECT:
		// �V�K�Q�X�g�̎�tON�̂Ƃ�

		// �V�K�ڑ���҂�
		m_NewGuestNetHandle = GetNewAcceptNetWork();
		if (m_NewGuestNetHandle != -1)
		{
			// �V�����ڑ��𔭌�������
			IPDATA ip = {};
			GetNetWorkIP(m_NewGuestNetHandle, &ip);
			LOG_INFO("[AcceptGuestStep::WAIT_CONNECT] �V�����ڑ��𔭌��FNetHandle = %d, IP = %d.%d.%d.%d", m_NewGuestNetHandle, ip.d1, ip.d2, ip.d3, ip.d4);

			LOG_INFO("[AcceptGuestStep::WAIT_CONNECT] ���݂̏��F�ς݃Q�X�g�̐ڑ����F%d/%d", m_AcceptedGuest->GetItem().size(), ACCEPTED_GUEST_MAX);

			if (m_AcceptedGuest->GetItem().size() < ACCEPTED_GUEST_MAX)
			{
				// �Q�X�g�̐ڑ������ő吔�ɒB���Ă��Ȃ���
				LOG_WARN("[AcceptGuestStep::WAIT_CONNECT] �Q�X�g�̐ڑ������ő吔�ɒB���Ă��Ȃ��̂�AcceptGuestStep::REQUEST_NAME�֐i��");

				// �����IP��ێ�
				m_NewGuestIP->ChangeText(Common::IP2String(ip));
				// ������AcceptGuestStep::REQUEST_NAME�֐i��
				SetAcceptGuestStep(AcceptGuestStep::REQUEST_NAME);
				m_RequestNameTimeStart = GetNowCount();
			}
			else
			{
				// �Q�X�g�̐ڑ������ő吔�ɒB���Ă�����
				LOG_WARN("[AcceptGuestStep::WAIT_CONNECT] �Q�X�g�̐ڑ������ő吔�ɒB���Ă���̂Őڑ�������");

				// �V�K�ڑ����悤�Ƃ��Ă���Q�X�g�ɋ��ۃ��b�Z�[�W�𑗐M�i���̂Ƃ��z�X�g�͖����Ȃ��j
				Command::Message msgSend = Command::MakeConnect(false, DUMMY_TEXT);
				Command::Send(m_NewGuestNetHandle, msgSend);
			}
		}
		break;
	case AcceptGuestStep::REQUEST_NAME:
	{
		// �ڑ����Ă�������͂܂������̖��O�𖼏��͂��Ȃ̂ł����҂�

		bool Reject = false;

		Command::ReceiveResult result = Command::CheckReceive(m_NewGuestNetHandle);
		if (result == Command::ReceiveResult::SUCCESS)
		{
			// ���肩�烁�b�Z�[�W�������Ă�����
			Command::Message Msg = Command::Receive(m_NewGuestNetHandle);

			if (Msg.type == Command::Type::CHANGE_NAME_MYSELF)
			{
				// ���肪������Ă�����
				LOG_WARN("[AcceptGuestStep::REQUEST_NAME] ����胁�b�Z�[�W�ł���̂�AcceptGuestStep::SELECT_ACCEPT_REJECT�֐i��");

				// ����̖��O��ێ�
				m_NewGuestName->ChangeText(Msg.string.text);
				// ������AcceptGuestStep::SELECT_ACCEPT_REJECT�֐i��
				SetAcceptGuestStep(AcceptGuestStep::SELECT_ACCEPT_REJECT);
			}
			else
			{
				// ���肪�����ł͂Ȃ����b�Z�[�W�𑗂��Ă�����
				LOG_WARN("[AcceptGuestStep::REQUEST_NAME] ����胁�b�Z�[�W�ł͂Ȃ�");
				// ����
				Reject = true;
			}
		}
		else if (result == Command::ReceiveResult::NONE)
		{
			// ���̃��b�Z�[�W���󂯎���Ă��Ȃ��Ƃ�
			if (GetNowCount() - m_RequestNameTimeStart >= REQUEST_NAME_TIMEOUT)
			{
				// �^�C���A�E�g
				LOG_WARN("[AcceptGuestStep::REQUEST_NAME] �^�C���A�E�g");
				// ����
				Reject = true;
			}
		}
		else
		{
			// ����
			Reject = true;
		}

		if (Reject)
		{
			LOG_WARN("[AcceptGuestStep::REQUEST_NAME] �ڑ������ۂ���AcceptGuestStep::WAIT_CONNECT�֖߂�");

			// �V�K�ڑ����悤�Ƃ��Ă���Q�X�g�ɋ��ۃ��b�Z�[�W�𑗐M�i���̂Ƃ��z�X�g�͖����Ȃ��j
			Command::Message msgSend = Command::MakeConnect(false, DUMMY_TEXT);
			Command::Send(m_NewGuestNetHandle, msgSend);

			// ������AcceptGuestStep::WAIT_CONNECT�֖߂�
			SetAcceptGuestStep(AcceptGuestStep::WAIT_CONNECT);
		}
	}
	break;
	case AcceptGuestStep::SELECT_ACCEPT_REJECT:
		// �ڑ������߂Ă��������NetHandle�Ɩ��O������������
		// ���F�E���ۂ�I�������ʕ��i��\���i�����ł͕\���ł͂Ȃ������j����
		UpdateComponent(m_AcceptRejectComponent);
		break;
	case AcceptGuestStep::SELECTED:
		// ���F�E���ۂ��I�����ꂽ��i���F�E���ۂ̌���̏u�Ԃ̓R�[���o�b�N�֐��j
		if (m_AcceptOrReject)
		{
			// ���F����ꍇ
			LOG_INFO("[AcceptGuestStep::SELECTED] ���F");

			// ���F���b�Z�[�W�𑗐M�i���̂Ƃ��z�X�g�������Ԃ��j
			Command::Message msgSend = Command::MakeConnect(true, m_Name);
			Command::Send(m_NewGuestNetHandle, msgSend);

			// ���F�ς݃Q�X�g�ɒǉ�
			m_AcceptedGuest->AddItem(m_NewGuestNetHandle, m_NewGuestName->GetText());

			// �S���F�ς݃Q�X�g�ɐV�K�Q�X�g�̐ڑ���m�点��i�V�K�Q�X�g�{�l���܂܂Ȃ��j
			LOG_INFO("[AcceptGuestStep::SELECTED] �S���F�ς݃Q�X�g�ɐV�K�Q�X�g�̐ڑ���m�点��i�V�K�Q�X�g�{�l���܂܂Ȃ��j");
			auto& list = m_AcceptedGuest->GetItem();
			for (auto& pair : list)
			{
				int ID = pair.first;
				LOG_INFO("[AcceptGuestStep::SELECTED] ID = %d, Name = %s", ID, pair.second.Text.c_str());

				if (ID == m_NewGuestNetHandle)
				{
					LOG_INFO("[AcceptGuestStep::SELECTED] �V�K�Q�X�g�{�l�Ȃ̂Ń��b�Z�[�W�𑗐M���Ȃ�");
					continue;
				}
				Command::Message msgSend2 = Command::MakeNewGuest(m_NewGuestNetHandle, m_NewGuestName->GetText());
				Command::Send(ID, msgSend2);
			}

			// �V�K�Q�X�g�ɑS���F�ς݃Q�X�g��m�点��i�V�K�Q�X�g�{�l���܂܂Ȃ��j
			LOG_INFO("[AcceptGuestStep::SELECTED] �V�K�Q�X�g�ɑS���F�ς݃Q�X�g��m�点��i�V�K�Q�X�g�{�l���܂ށj");
			for (auto& pair : list)
			{
				int ID = pair.first;
				LOG_INFO("[AcceptGuestStep::SELECTED] ID = %d, Name = %s", ID, pair.second.Text.c_str());

				Command::Message msgSend2 = Command::MakeNewGuest(ID, pair.second.Text);
				Command::Send(m_NewGuestNetHandle, msgSend2);
			}
		}
		else
		{
			// ���ۂ���ꍇ
			LOG_INFO("[AcceptGuestStep::SELECTED] ����");

			// ���ۃ��b�Z�[�W�𑗐M�i���̂Ƃ��z�X�g�͖����Ȃ��j
			Command::Message msgSend = Command::MakeConnect(false, DUMMY_TEXT);
			Command::Send(m_NewGuestNetHandle, msgSend);
		}
		// ������AcceptGuestStep::WAIT_CONNECT�֖߂�
		SetAcceptGuestStep(AcceptGuestStep::WAIT_CONNECT);
		break;
	}
}

void SceneHost::ProcessDissconectGuest()
{
	// �Q�X�g�͂��ؒf���Ă��邩������Ȃ��̂ŏ�Ɋm�F���Ă���
	bool f = false;
	int LostHandle = GetLostNetWork();
	if (LostHandle == -1)
	{
		return;
	}

	LOG_INFO("�ؒf�𔭌��FNetHandle = %d", LostHandle);

	auto& list = m_AcceptedGuest->GetItem();
	for (auto& pair : list)
	{
		int ID = pair.first;
		if (LostHandle == ID)
		{
			// �ؒf���Ă�Q�X�g�𔭌�������
			// ���F�ς݃Q�X�g����폜
			m_AcceptedGuest->RemoveItem(ID);
			f = true;

			LOG_INFO("�ؒf�҂�ID�����F�ς݃Q�X�g���ɔ��������̂ō폜", LostHandle);
			break;
		}
	}
	if (f)
	{
		// �Q�X�g���폜���Ă�����
		// �S���F�ς݃Q�X�g�ɐؒf�����Q�X�g�����邱�Ƃ�m�点��
		Command::Message msgSend = Command::MakeDisconnectGuest(LostHandle);
		for (auto& pair : list)
		{
			int ID = pair.first;
			LOG_INFO("ID = %d, Name = %s", ID, pair.second.Text.c_str());
			Command::Send(ID, msgSend);
		}
	}
}
void SceneHost::ProcessReceiveCommand()
{
	// �S���F�ς݃Q�X�g���烁�b�Z�[�W�̎�M���m�F���Ă���
	auto& list = m_AcceptedGuest->GetItem();
	for (auto& pair : list)
	{
		int ID = pair.first;

		Command::ReceiveResult result = Command::CheckReceive(m_NewGuestNetHandle);
		if (result == Command::ReceiveResult::SUCCESS)
		{
			// ���肩�烁�b�Z�[�W�������Ă�����
			Command::Message Msg = Command::Receive(m_NewGuestNetHandle);

			switch (Msg.type)
			{
			case Command::Type::CHAT_TEXT:
			{
				// �Q�X�g���`���b�g��ł����Ƃ̂���

				// �`���b�g�ɒǉ�����
				m_Chat->AddLine(ID, m_AcceptedGuest->GetText(ID), Msg.string.text);

				// �S���F�ς݃Q�X�g�ɒm�点��i�`���b�g��ł����Q�X�g�{�l���܂ށj
				Command::Message msgSend = Command::MakeChatText(ID, Msg.string.text);
				auto& list2 = m_AcceptedGuest->GetItem();
				for (auto& pair2 : list2)
				{
					int ID2 = pair2.first;
					LOG_INFO("ID = %d, Name = %s", ID2, pair2.second.Text.c_str());
					Command::Send(ID2, msgSend);
				}
			}
			break;
			case Command::Type::CHANGE_NAME_MYSELF:
			{
				// �Q�X�g�����O�̕ύX�������Ƃ̂���

				// �͂��ė������O�ɕύX����
				m_AcceptedGuest->ChangeText(ID, Msg.string.text);

				// �`���b�g�̕\���̕������O�̕ύX�𔽉f����
				m_Chat->ChangeName(ID, Msg.string.text);

				// �S���F�ς݃Q�X�g�ɒm�点��i���O��ς����Q�X�g�{�l���܂ށj
				Command::Message msgSend = Command::MakeChangeNameGuest(ID, Msg.string.text);
				auto& list2 = m_AcceptedGuest->GetItem();
				for (auto& pair2 : list2)
				{
					int ID2 = pair2.first;
					LOG_INFO("ID = %d, Name = %s", ID2, pair2.second.Text.c_str());
					Command::Send(ID2, msgSend);
				}
			}
			break;
			case Command::Type::ALL_UPDATE:
			{
				// �S�f�[�^���X�V�������Ƃ̂���

				// �ڑ����̃Q�X�g��S�Ēm�点��i�Q�X�g�{�l���܂ށj
				auto& list2 = m_AcceptedGuest->GetItem();
				for (auto& pair2 : list2)
				{
					int ID2 = pair2.first;
					LOG_INFO("ID = %d, Name = %s", ID2, pair2.second.Text.c_str());
					Command::Message msgSend = Command::MakeNewGuest(ID2, pair2.second.Text.c_str());
					Command::Send(ID, msgSend);
				}

				// �`���b�g�̗�����S�Ēm�点��
				auto& lines = m_Chat->GetLines();
				for (auto& line : lines)
				{
					Command::Message msgSend = Command::MakeChatText(line.ID, line.Body);
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
	m_Port = std::stoi(Port);
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
		LOG_INFO("�z�X�g����ύX�F%s �� %s", m_Name.c_str(), Name.c_str());
		m_Name = Name;

		// �`���b�g�̕\���̕������O�̕ύX�𔽉f����
		m_Chat->ChangeName(-1, m_Name);

		// �S���F�ς݃Q�X�g�Ƀz�X�g�����O��ύX�������Ƃ�m�点��
		auto& list = m_AcceptedGuest->GetItem();
		for (auto& pair : list)
		{
			int ID = pair.first;
			LOG_INFO("ID = %d, Name = %s", ID, pair.second.Text.c_str());

			Command::Message msgSend = Command::MakeChangeNameMySelf(m_Name);
			Command::Send(ID, msgSend);
		}
	}
}

void SceneHost::SetAcceptGuest(bool& OffOn)
{
	// �V�K�Q�X�g�̎�t�g�O���̃R�[���o�b�N�֐�
	LOG_INFO("�V�K�Q�X�g�̎�t�g�O���F%s�ɕύX�����݂�", OffOn ? "true" : "false");

	if (m_AcceptGuestStep == AcceptGuestStep::OFF)
	{
		// OFF�̂Ƃ��͂��ł�ON�ɂ���OK
		if (OffOn == true)
			SetAcceptGuestStep(AcceptGuestStep::WAIT_CONNECT);
	}
	else if (m_AcceptGuestStep == AcceptGuestStep::WAIT_CONNECT)
	{
		// �V�K�Q�X�g���ڑ������݂Ă��Ȃ��Ƃ���OFF�ɂ���OK
		if (OffOn == false)
			SetAcceptGuestStep(AcceptGuestStep::OFF);
	}
	else
	{
		// �V�K�Q�X�g���ڑ������݂Ă���Ƃ���ON���������Ȃ�
		OffOn = true;
	}

	// DxLib�̐ڑ���t��Ԃ̕ύX
	if (OffOn)
		PreparationListenNetWork(m_Port);
	else
		StopListenNetWork();

	LOG_INFO("�V�K�Q�X�g�̎�t�g�O���F%s�ɕύX����", OffOn ? "true" : "false");
}

void SceneHost::AcceptGuest()
{
	// �V�K�Q�X�g�̏��F�{�^���̃R�[���o�b�N�֐�
	LOG_INFO("���F�{�^������");

	// �v���O������ 100% true �ɂȂ�͂��̏���
	if (m_AcceptGuestStep == AcceptGuestStep::SELECT_ACCEPT_REJECT)
	{
		// ���F�t���O�𗧂ĂĎ��̒i�K�ɐi��
		m_AcceptOrReject = true;
		SetAcceptGuestStep(AcceptGuestStep::SELECTED);
	}
	else
	{
		LOG_ERROR("���̃��O�͏o�͂���Ă͂Ȃ�Ȃ��Fm_AcceptGuestStep = %d", m_AcceptGuestStep);
	}
}

void SceneHost::RejectGuest()
{
	// �V�K�Q�X�g�̋��ۃ{�^���̃R�[���o�b�N�֐�
	LOG_INFO("���ۃ{�^������");

	// �v���O������ 100% true �ɂȂ�͂��̏���
	if (m_AcceptGuestStep == AcceptGuestStep::SELECT_ACCEPT_REJECT)
	{
		// ���ۃt���O�𗧂ĂĎ��̒i�K�ɐi��
		m_AcceptOrReject = false;
		SetAcceptGuestStep(AcceptGuestStep::SELECTED);
	}
	else
	{
		LOG_ERROR("���̃��O�͏o�͂���Ă͂Ȃ�Ȃ��Fm_AcceptGuestStep = %d", m_AcceptGuestStep);
	}
}

void SceneHost::SetChatText(std::string& Text)
{
	// �`���b�g�e�L�X�g�����͂��ꂽ�Ƃ��̃R�[���o�b�N�֐�
	m_ChatText = Text;
}

void SceneHost::SendChatText()
{
	LOG_INFO("���M�{�^������");

	if (m_AcceptedGuest->GetItem().size() > 0)
	{
		// �`���b�g��ǉ�
		m_Chat->AddLine(-1, m_Name, m_ChatText);

		// �S���F�ς݃Q�X�g�Ƀz�X�g���`���b�g��ł������Ƃ�m�点��
		Command::Message msgSend = Command::MakeChatText(-1, m_ChatText);
		auto& list = m_AcceptedGuest->GetItem();
		for (auto& pair : list)
		{
			int ID = pair.first;
			LOG_INFO("ID = %d, Name = %s", ID, pair.second.Text.c_str());
			Command::Send(ID, msgSend);
		}
	}
	else
	{
		LOG_INFO("���F�ς݃Q�X�g����l�����Ȃ��̂Ŏ��s���Ȃ�");
	}
}

void SceneHost::Disconnect()
{
	// �S���F�ς݃Q�X�g�Ƀz�X�g���ؒf���邱�Ƃ�m�点��
	LOG_INFO("�S���F�ς݃Q�X�g�Ƀz�X�g���ؒf���邱�Ƃ�m�点��");

	Command::Message msgSend = Command::MakeConnect(false, m_Name);
	auto& list = m_AcceptedGuest->GetItem();
	for (auto& pair : list)
	{
		int ID = pair.first;
		LOG_INFO("ID = %d, Name = %s", ID, pair.second.Text.c_str());
		Command::Send(ID, msgSend);
	}
	m_AcceptedGuest->RemoveAllItem();
}

void SceneHost::End()
{
	LOG_INFO("�I���{�^�������Fm_Next = %d", m_Next);

	if (m_Next == nullptr)
	{
		Disconnect();
		m_Next = SceneCreator::Create(SceneCreator::Name::CHOICE_HOST_GUEST);
	}
}

/*
 * m_AcceptGuestStep��ύX������m_AcceptState���ύX����K�v������̂�
 * m_AcceptGuestStep��ύX����Ƃ��͕K�����̊֐��ōs��
 */
void SceneHost::SetAcceptGuestStep(AcceptGuestStep Step)
{
	m_AcceptGuestStep = Step;
	m_AcceptState->ChangeText(ACCEPT_STEP_STATE[static_cast<int>(m_AcceptGuestStep)]);
}

