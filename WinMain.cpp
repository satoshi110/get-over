#include "NEGAA.h"
#pragma comment(lib,"NEGAA.lib")

//
//WinMain()
//
INT WINAPI WinMain( HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR szStr,INT iCmdShow)
{
	//MY_NEGAAオブジェクトを生成する。これが一番最初にすること。
	MY_NEGAA nega;
	nega.Init(hInst,"ゲームプログラミング超簡単学習 with NEGAA (powered by NorthBrain)",800,600);
	
	nega.OneTime();
	//メッセージループ（このループの中で、全てのことを行います）
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );

    while( msg.message!=WM_QUIT )
    {
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {            			
			TranslateMessage( &msg );             
			DispatchMessage( &msg );
        }
        else
		{
			nega.LoopStart();

			nega.Main();

			nega.LoopEnd();
		}
	}	
	//オブジェクト破棄 ここに処理が来たということは、全ての処理（ゲーム）が終了したということです。メモリを開放します。
	nega.Release();
	//OSに戻ります。
	return 0;
}



