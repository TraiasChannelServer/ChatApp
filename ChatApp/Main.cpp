#include "DxLib.h"
#include "SceneCreator.h"
#include "FontManager.h"
#include "Common.h"
#include "Logger.h"
#include "DataFileInOut.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	// DxLib_Init関数の前に呼ぶべきDxLibの設定関数群
	SetOutApplicationLogValidFlag(TRUE);
	ChangeWindowMode(TRUE);
	SetMainWindowText("ChatApp");
	SetWindowSizeChangeEnableFlag(TRUE);
	SetDoubleStartValidFlag(FALSE);
	SetGraphMode(Common::WINDOW_X_SIZE, Common::WINDOW_Y_SIZE, 16);
	//SetBackgroundColor(0xf4, 0xf5, 0xf7); // beautiful white
	SetBackgroundColor(0x15, 0x18, 0x1e); // beautiful black
	SetAlwaysRunFlag(TRUE);

	// DxLibの初期化
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	// 設定データ読み込み
	DataFileInOut::Inst().Input();

	// フォントの初期化
	FontManager::Inst().Initialize();

	// 最初に表示するシーンを生成
	Scene* scene = SceneCreator::Create(SceneCreator::Name::CHOICE_HOST_GUEST);

	LOG_INFO("メインループ開始");

	// メインループ
	while (ProcessMessage() == 0 && Common::ApplicationExecution == true) {
		// 現在のシーンを更新
		Scene* next = scene->Update();
		scene->Draw();

		if (next != nullptr)
		{
			// 現在のシーンを破棄
			delete scene;
			// 次のシーンに移る
			scene = next;
		}

		// Debug応急処置：画面表示で処理のフリーズを知ることができる
		{
			static int FrameCount = 0;
			static int Color = GetColor(255, 0, 0);
			DrawString(Common::WINDOW_X_SIZE - 100, 0, std::to_string(++FrameCount).c_str(), Color);
		}

		// 描画
		ScreenFlip();

		// CPU使用率を下げる(60FPSも要らないと思うから)
		WaitTimer(40);
	}

	LOG_INFO("メインループ終了");

	// シーンを破棄
	delete scene;
	scene = nullptr;

	// フォントの破棄
	FontManager::Inst().Destroy();

	// 設定データ書き込み
	DataFileInOut::Inst().Output();

	// DxLibの終了処理
	DxLib_End();

	return 0;
}

/*
https://27bit.com/question-number.html
ポート番号は0～65535までありますが、すべて使用出来るわけではありません。
ポート番号は大きく3つにわかれ、0～1023まではウェルノウンポートと言い、決められているものが多く、使用出来ないものが多々あります。
例えば、私たちが普段使っているインターネット、これは80番のポート番号を使用しています。そのほかには、メールの受信ポートは110番。送信ポートは25番というように決まったものを使っています。
1024～49512までの番号は、登録ポートと言い、多くのソフトウェアによって割り当てられます。使用可能なものもありますが、何らかのソフトウェアと重複する可能性があります。
49513～65535までの番号は、ダイナミック/プライベートポートと言い、基本的には、この間が自由に使えるポート番号です。
*/
