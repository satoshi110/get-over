#include "NEGAA.h"
#pragma comment(lib,"NEGAA.lib")

//
//WinMain()
//
INT WINAPI WinMain( HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR szStr,INT iCmdShow)
{
	//MY_NEGAA�I�u�W�F�N�g�𐶐�����B���ꂪ��ԍŏ��ɂ��邱�ƁB
	MY_NEGAA nega;
	nega.Init(hInst,"�Q�[���v���O���~���O���ȒP�w�K with NEGAA (powered by NorthBrain)",800,600);
	
	nega.OneTime();
	//���b�Z�[�W���[�v�i���̃��[�v�̒��ŁA�S�Ă̂��Ƃ��s���܂��j
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
	//�I�u�W�F�N�g�j�� �����ɏ����������Ƃ������Ƃ́A�S�Ă̏����i�Q�[���j���I�������Ƃ������Ƃł��B���������J�����܂��B
	nega.Release();
	//OS�ɖ߂�܂��B
	return 0;
}



