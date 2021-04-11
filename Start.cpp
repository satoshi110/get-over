#include "NEGAA.h"

//数の定義
#define slnum 20//坂の数
#define plx1 10//x軸を正に移動する板の数
#define plx2 20//x軸を負に移動する板の数
#define ply1 30//ｙ軸を正に移動する板の数
#define ply2 40//ｙ軸を負に移動する板の数
#define plz1 50//ｚ軸を正に移動する板の数
#define plz2 60//ｚ軸を負に移動する板の数
#define pl 70//静止している板の数
#define plyaw 80//ｚ軸に回転する板の数
#define pldt 130//障害物の板の数
#define plnum 130//板の総数
#define boxnum 5//箱の総数
#define linum 5//ライトの数
#define fontnum 10//フォントメッシュの数


//ｘメッシュ
XMESH human;//操作キャラ
XMESH humanhead;//操作キャラの頭
XMESH humandraw;//画面に描写する操作キャラ
XMESH humot1;//操作キャラのモーション（前方）
XMESH humot2;//操作キャラのモーション（後方）
XMESH humot3;//操作キャラのモーション（左方）
XMESH humot4;//操作キャラのモーション（右方）
XMESH ground;//ステージ
XMESH slope[slnum];//坂
XMESH plate[plnum];//板
XMESH missile;//ミサイル
XMESH box[boxnum];//箱

//変数
int i=0;//反復処理のカウント
int t=0;//ゲーム内の時間
int jump=0;//ジャンプのカウント
int jtime=0;//ジャンプの時間
int frame;//モーション中のフレーム数
int cut=50;//1モーションあたりのフレーム数
int remun=9;//残り時間（分）
int resec=59;//残り時間（秒）
int mission=0;//ミッション表示のカウント
int ope=0;//操作説明表示のカウント
int sw[boxnum];//箱のスイッチ

float sp1=0.1;//移動する板のスピード
float sp2=0.01;//移動する板のスピード
float ang=0;//回転の角度
float dt=0.2;//当たり判定の距離
float disx[10];//操作キャラと板のx軸の距離
float disz[10];//操作キャラと板のｚ軸の距離
float lumi=0.5;//光の強さ

//ベクトル
D3DXVECTOR3 vNormal;//アドレス
D3DXVECTOR3 vDir;//方向
D3DXVECTOR3 iner(0,0,0);//慣性
D3DXVECTOR3 vGrav(0,-0.05,0);//重力
D3DXVECTOR3 vJump(0,0.15,0);//ジャンプ
D3DXVECTOR3 deltaX[plnum];//板のx軸移動
D3DXVECTOR3 deltaY[plnum];//板のy軸移動
D3DXVECTOR3 deltaZ[plnum];//板のz軸移動
D3DXVECTOR3 Slip(D3DXVECTOR3* L,D3DXVECTOR3* N);//滑り

RAY_COLLIDE cl;//レイ判定
RAY_COLLIDE cl2;//レイ判定
DISTANT_LIGHT DLight[linum];//方向ライト
POINT_LIGHT PLight;//方向ライト
DWORD robotbgm=0;//ステージBGM
DWORD jumpse=0;//じゃんぷのSE
DWORD boxse=0;//箱のSE
FONT font;//フォント
FONTMESH fm[fontnum];//フォントメッシュ


void MY_NEGAA::OneTime()
{	
	//ｘメッシュの読み込み
	human.Load("X_file/RobotA_still_back.x");
	human.vPos=D3DXVECTOR3(0,0.2,0);

	humandraw.Load("X_file/RobotA_still_back.x");

	humot1.Load("X_file/RobotA.x");
	humot1.RotateYaw(180);
	humot2.Load("X_file/RobotA.x");
	humot3.Load("X_file/RobotA.x");
	humot3.RotateYaw(90);
	humot4.Load("X_file/RobotA.x");
	humot4.RotateYaw(-90);

	ground.Load("X_file/sky_dome.x");
	ground.scale=3;

	missile.Load("X_file/Missile.x");
	missile.vPos=D3DXVECTOR3(75,32,82);
	missile.RotatePitch(-90);
	missile.scale=2.0;

	for(i=0;i<slnum;i++)
	{
		slope[i].Load("X_file/slope.x");
		slope[i].vPos=D3DXVECTOR3(0,-40,0);
	}

	for(i=0;i<plnum;i++)
	{
		plate[i].Load("X_file/plate.x");
		plate[i].vPos=D3DXVECTOR3(0,-30,0);
		deltaX[i]=D3DXVECTOR3(0,0,0);
		deltaY[i]=D3DXVECTOR3(0,0,0);
		deltaZ[i]=D3DXVECTOR3(0,0,0);
	}

	for(i=0;i<boxnum;i++)
	{
		box[i].Load("X_file/Box.x");
		box[i].vPos=D3DXVECTOR3(0,-40,0);
	}
	
	//１階の坂の座標
	slope[0].vPos=D3DXVECTOR3(0,0,10);
	slope[1].vPos=D3DXVECTOR3(0,2.37,20.4);
	slope[2].vPos=D3DXVECTOR3(0,4.74,30.8);
	slope[3].vPos=D3DXVECTOR3(0,7.11,71.2);
	slope[3].RotateRoll(180);
	slope[4].vPos=D3DXVECTOR3(0,4.71,71.2);
	slope[4].RotateYaw(180);

	//２階の坂の座標
	slope[5].vPos=D3DXVECTOR3(0,22,70);
	slope[5].RotateRoll(180);
	slope[5].RotateYaw(90);
	slope[6].vPos=D3DXVECTOR3(0,19.6,70);
	slope[6].RotateYaw(-90);
	slope[7].vPos=D3DXVECTOR3(-65,22,1);
	slope[7].RotateRoll(180);
	slope[8].vPos=D3DXVECTOR3(-65,19.6,1);
	slope[8].RotateYaw(180);
	slope[9].vPos=D3DXVECTOR3(0,22,-64);
	slope[9].RotateRoll(180);
	slope[10].vPos=D3DXVECTOR3(0,19.6,-64);
	slope[10].RotateYaw(180);
	slope[11].vPos=D3DXVECTOR3(75,22,-54);
	slope[11].RotateRoll(180);	
	slope[12].vPos=D3DXVECTOR3(75,19.6,-54);
	slope[12].RotateYaw(180);
	slope[13].vPos=D3DXVECTOR3(75,22,79);
	slope[13].scale=3.0;
	slope[13].RotateRoll(180);
	slope[13].RotateYaw(90);
	slope[14].vPos=D3DXVECTOR3(75,14.8,79);
	slope[14].scale=3.0;
	slope[14].RotateYaw(-90);
	
	//箱1
	box[0].vPos=D3DXVECTOR3(-1,10,35);

	//１階の板の座標
	plate[40].vPos=D3DXVECTOR3(0,7.2,36);
	plate[40].scale=3.0;

	plate[110].vPos=D3DXVECTOR3(0,8.7,50);
	plate[110].scale=3.0;
	plate[110].RotatePitch(90);

	plate[67].vPos=D3DXVECTOR3(0,7.2,82);
	plate[67].scale=3.0;

	//箱2
	box[1].vPos=D3DXVECTOR3(0,10,82);

	//２階への板の座標
	plate[20].vPos=D3DXVECTOR3(0,7.2,76);
	plate[20].scale=3.0;

	plate[30].vPos=D3DXVECTOR3(0,22.2,73);
	plate[30].scale=3.0;

	//箱3
	box[2].vPos=D3DXVECTOR3(0,28.2,65);

	//２階の板の座標
	plate[68].vPos=D3DXVECTOR3(0,25.2,65);
	plate[68].scale=3.0;

	plate[10].vPos=D3DXVECTOR3(-4,22.2,69);
	plate[10].scale=3.0;

	plate[111].vPos=D3DXVECTOR3(-20,23.7,69);
	plate[111].scale=3.0;
	plate[111].RotatePitch(90);
	plate[111].RotateRoll(90);

	plate[112].vPos=D3DXVECTOR3(-45,23.7,69);
	plate[112].scale=3.0;
	plate[112].RotatePitch(90);
	plate[112].RotateRoll(90);

	plate[41].vPos=D3DXVECTOR3(-64,22.2,6);
	plate[41].scale=3.0;

	plate[113].vPos=D3DXVECTOR3(-64.5,23.7,50);
	plate[113].scale=3.0;
	plate[113].RotatePitch(90);

	plate[114].vPos=D3DXVECTOR3(-64.5,26.7,35);
	plate[114].scale=3.0;
	plate[114].RotatePitch(90);

	plate[115].vPos=D3DXVECTOR3(-64.5,23.7,20);
	plate[115].scale=3.0;
	plate[115].RotatePitch(90);

	//箱4
	box[3].vPos=D3DXVECTOR3(-64,25.2,4);

	plate[50].vPos=D3DXVECTOR3(-64.5,22.2,-4);
	plate[50].scale=3.0;

	plate[51].vPos=D3DXVECTOR3(-67.5,22.2,-4);
	plate[51].scale=3.0;

	plate[116].vPos=D3DXVECTOR3(-64.5,23.7,-20);
	plate[116].scale=3.0;
	plate[116].RotatePitch(90);

	plate[117].vPos=D3DXVECTOR3(-64.5,26.7,-20);
	plate[117].scale=3.0;
	plate[117].RotatePitch(90);

	plate[118].vPos=D3DXVECTOR3(-67.5,23.7,-35);
	plate[118].scale=3.0;
	plate[118].RotatePitch(90);

	plate[119].vPos=D3DXVECTOR3(-67.5,26.7,-35);
	plate[119].scale=3.0;
	plate[119].RotatePitch(90);

	plate[120].vPos=D3DXVECTOR3(-64.5,23.7,-50);
	plate[120].scale=3.0;
	plate[120].RotatePitch(90);

	plate[121].vPos=D3DXVECTOR3(-64.5,26.7,-50);
	plate[121].scale=3.0;
	plate[121].RotatePitch(90);

	plate[60].vPos=D3DXVECTOR3(-56,22.2,-64);
	plate[60].scale=3.0;

	plate[61].vPos=D3DXVECTOR3(-48,22.2,-64);
	plate[61].scale=3.0;

	plate[62].vPos=D3DXVECTOR3(-40,22.2,-64);
	plate[62].scale=3.0;

	plate[63].vPos=D3DXVECTOR3(-32,22.2,-64);
	plate[63].scale=3.0;

	plate[64].vPos=D3DXVECTOR3(-24,22.2,-64);
	plate[64].scale=3.0;

	plate[65].vPos=D3DXVECTOR3(-16,22.2,-64);
	plate[65].scale=3.0;

	plate[66].vPos=D3DXVECTOR3(-8,22.2,-64);
	plate[66].scale=3.0;

	plate[70].vPos=D3DXVECTOR3(12,22.2,-64);
	plate[70].scale=10.0;

	plate[71].vPos=D3DXVECTOR3(24,22.2,-64);
	plate[71].scale=10.0;

	plate[72].vPos=D3DXVECTOR3(36,22.2,-64);
	plate[72].scale=10.0;

	plate[73].vPos=D3DXVECTOR3(48,22.2,-64);
	plate[73].scale=10.0;

	plate[74].vPos=D3DXVECTOR3(60,22.2,-64);
	plate[74].scale=10.0;

	plate[75].vPos=D3DXVECTOR3(72,22.2,-64);
	plate[75].scale=10.0;

	//箱5
	box[4].vPos=D3DXVECTOR3(75,25.2,-64);

	plate[42].vPos=D3DXVECTOR3(69,22.2,-49);
	plate[42].scale=3.0;

	plate[43].vPos=D3DXVECTOR3(72,22.2,-49);
	plate[43].scale=3.0;

	plate[44].vPos=D3DXVECTOR3(75,22.2,-49);
	plate[44].scale=3.0;

	plate[45].vPos=D3DXVECTOR3(78,22.2,-49);
	plate[45].scale=3.0;

	plate[46].vPos=D3DXVECTOR3(81,22.2,-49);
	plate[46].scale=3.0;

	plate[80].vPos=D3DXVECTOR3(69,23.7,-30);
	plate[80].scale=3.0;
	plate[80].RotatePitch(90);

	plate[81].vPos=D3DXVECTOR3(75,23.7,-30);
	plate[81].scale=3.0;
	plate[81].RotatePitch(90);

	plate[82].vPos=D3DXVECTOR3(81,23.7,-30);
	plate[82].scale=3.0;
	plate[82].RotatePitch(90);

	plate[95].vPos=D3DXVECTOR3(72,26.7,-30);
	plate[95].scale=3.0;
	plate[95].RotatePitch(90);

	plate[96].vPos=D3DXVECTOR3(78,26.7,-30);
	plate[96].scale=3.0;
	plate[96].RotatePitch(90);

	plate[83].vPos=D3DXVECTOR3(72,23.7,-10);
	plate[83].scale=3.0;
	plate[83].RotatePitch(90);

	plate[84].vPos=D3DXVECTOR3(78,23.7,-10);
	plate[84].scale=3.0;
	plate[84].RotatePitch(90);

	plate[97].vPos=D3DXVECTOR3(69,26.7,-10);
	plate[97].scale=3.0;
	plate[97].RotatePitch(90);

	plate[98].vPos=D3DXVECTOR3(75,26.7,-10);
	plate[98].scale=3.0;
	plate[98].RotatePitch(90);

	plate[99].vPos=D3DXVECTOR3(81,26.7,-10);
	plate[99].scale=3.0;
	plate[99].RotatePitch(90);

	plate[85].vPos=D3DXVECTOR3(69,23.7,10);
	plate[85].scale=3.0;
	plate[85].RotatePitch(90);

	plate[86].vPos=D3DXVECTOR3(72,23.7,10);
	plate[86].scale=3.0;
	plate[86].RotatePitch(90);

	plate[87].vPos=D3DXVECTOR3(75,23.7,10);
	plate[87].scale=3.0;
	plate[87].RotatePitch(90);

	plate[100].vPos=D3DXVECTOR3(78,26.7,10);
	plate[100].scale=3.0;
	plate[100].RotatePitch(90);

	plate[101].vPos=D3DXVECTOR3(81,26.7,10);
	plate[101].scale=3.0;
	plate[101].RotatePitch(90);

	plate[88].vPos=D3DXVECTOR3(75,23.7,30);
	plate[88].scale=3.0;
	plate[88].RotatePitch(90);

	plate[89].vPos=D3DXVECTOR3(78,23.7,30);
	plate[89].scale=3.0;
	plate[89].RotatePitch(90);

	plate[90].vPos=D3DXVECTOR3(81,23.7,30);
	plate[90].scale=3.0;
	plate[90].RotatePitch(90);

	plate[102].vPos=D3DXVECTOR3(69,26.7,30);
	plate[102].scale=3.0;
	plate[102].RotatePitch(90);

	plate[103].vPos=D3DXVECTOR3(72,26.7,30);
	plate[103].scale=3.0;
	plate[103].RotatePitch(90);

	plate[91].vPos=D3DXVECTOR3(69,23.7,50);
	plate[91].scale=3.0;
	plate[91].RotatePitch(90);

	plate[92].vPos=D3DXVECTOR3(72,23.7,50);
	plate[92].scale=3.0;
	plate[92].RotatePitch(90);

	plate[93].vPos=D3DXVECTOR3(78,23.7,50);
	plate[93].scale=3.0;
	plate[93].RotatePitch(90);

	plate[94].vPos=D3DXVECTOR3(81,23.7,50);
	plate[94].scale=3.0;
	plate[94].RotatePitch(90);

	plate[104].vPos=D3DXVECTOR3(72,26.7,50);
	plate[104].scale=3.0;
	plate[104].RotatePitch(90);

	plate[105].vPos=D3DXVECTOR3(75,26.7,50);
	plate[105].scale=3.0;
	plate[105].RotatePitch(90);

	plate[106].vPos=D3DXVECTOR3(78,26.7,50);
	plate[106].scale=3.0;
	plate[106].RotatePitch(90);

	//フォント
	font.Load("arial",20,30,D3DCOLOR_XRGB(0,255,255));
	for(i=0; i<fontnum; i++)
	{
		fm[i].Load(L"→");
		fm[i].vPos=D3DXVECTOR3(0,-20,0);
	}
	fm[0].vPos=D3DXVECTOR3(0,25,70);
	fm[0].RotateYaw(180);
	fm[1].vPos=D3DXVECTOR3(-65,25,0);
	fm[1].RotateYaw(90);
	fm[2].vPos=D3DXVECTOR3(0,25,-65);
	fm[3].vPos=D3DXVECTOR3(75,25,-55);
	fm[3].RotateYaw(-90);
	
	//音の読み込み
	LoadSound(&robotbgm,"wav/robotbgm.wav",0,0,true);
	LoadSound(&jumpse,"wav/jump.wav",0,0,true);
	LoadSound(&boxse,"wav/box.wav",0,0,true);

	//カメラ
	Camera.SetPosition(0,1,0);

	//ライトの角度
	Light.SetAngle(0,90,0);

	//操作キャラのライトの輝度
	PLight.SetLuminance(lumi);

}

void MY_NEGAA::Main()
{
	//BGM
	if(t>1)
	{
		Play(robotbgm,true);
	}

	//目的
	if(mission%2==0 && t>0)
	{
		MessageBox(0,"ミッション：\n\n・ミサイルの発射を止めろ\n・ミサイルへの道筋は目の前の階段から道なりにまっすぐだ\n・回転するキューブを取ると新たな足場が出る\n\nこのウィンドウを閉じる：ENTER","",MB_OK);
		mission++;
	}

	if(Key(DIK_M))
	{
		mission++;
	}

	//操作説明
	if(ope%2==0 && t>0)
	{
		MessageBox(0,"操作説明：\n\n・前進：W　・視点変更（上空）：↑（PGUP）　・ジャンプ：SPACE\n・左進：A　 ・左に旋回　　　　：←（HOME）\n・右進：D　 ・右へ旋回　　　　：→（END）\n・後進：S　 ・視点変更（後方）：↓（PGDN）\n\nこのウィンドウを閉じる：ENTER","",MB_OK);
		ope++;
	}

	if(Key(DIK_O))
	{
		ope++;
	}


	//フォント
	char moji[100];
	sprintf(moji,"ミサイル発射まであと:%2d分:%2d秒\nミッション：M\n操作説明　：O",remun-(t/3600)%3600,resec-(t/60)%60);

	font.vPos=D3DXVECTOR2(30,20);
	font.Draw(moji);

	//ゲームオーバー
	if(remun-(t/3600)%3600==0 && resec-(t/60)%60<0)
	{
		MessageBox(0,"ゲームオ－バー","",MB_OK);//ゲームオーバー表示
		PostQuitMessage(0);//ゲーム終了
	}

	//方向ベクトル
	vDir=D3DXVECTOR3(0,0,0);

	//重力
	vDir+=vGrav;

	//操作キャラのライト
	PLight.vPos=human.vPos;
	PLight.vPos.y+=1.7;

	//描写する操作キャラ
	humandraw.vPos=human.vPos;
	humandraw.vPos.y=human.vPos.y-0.2;

	//操作キャラのモーション
	humot1.vPos=human.vPos;
	humot1.vPos.y=human.vPos.y-0.2;
	humot2.vPos=human.vPos;
	humot2.vPos.y=human.vPos.y-0.2;
	humot3.vPos=human.vPos;
	humot3.vPos.y=human.vPos.y-0.2;
	humot4.vPos=human.vPos;
	humot4.vPos.y=human.vPos.y-0.2;

	//操作キャラの移動
	if(Key(DIK_W))
	{
		vDir+=D3DXVECTOR3(0,0,0.05);
		frame+=1;
		if(frame%cut>=0 && frame%cut<10 )
		{
			humot1.SetAnimTime(2.6);
		}else if(frame%cut>=10 && frame%cut<20)
		{
			humot1.SetAnimTime(2.7);
		}else if(frame%cut>=20 && frame%cut<30)
		{
			humot1.SetAnimTime(2.8);
		}else if(frame%cut>=30 && frame%cut<40)
		{
			humot1.SetAnimTime(2.9);
		}else
		{
			humot1.SetAnimTime(3.0);
		}
		humot1.Draw();
	}else if(Key(DIK_S))
	{
		vDir+=D3DXVECTOR3(0,0,-0.05);
		frame+=1;
		if(frame%cut>=0 && frame%cut<10 )
		{
			humot2.SetAnimTime(2.6);
		}else if(frame%cut>=10 && frame%cut<20)
		{
			humot2.SetAnimTime(2.7);
		}else if(frame%cut>=20 && frame%cut<30)
		{
			humot2.SetAnimTime(2.8);
		}else if(frame%cut>=30 && frame%cut<40)
		{
			humot2.SetAnimTime(2.9);
		}else
		{
			humot2.SetAnimTime(3.0);
		}
		humot2.Draw();
	}else if(Key(DIK_A))
	{
		vDir+=D3DXVECTOR3(-0.05,0,0);
		frame+=1;
		if(frame%cut>=0 && frame%cut<10 )
		{
			humot3.SetAnimTime(2.6);
		}else if(frame%cut>=10 && frame%cut<20)
		{
			humot3.SetAnimTime(2.7);
		}else if(frame%cut>=20 && frame%cut<30)
		{
			humot3.SetAnimTime(2.8);
		}else if(frame%cut>=30 && frame%cut<40)
		{
			humot3.SetAnimTime(2.9);
		}else
		{
			humot3.SetAnimTime(3.0);
		}
		humot3.Draw();
	}else if(Key(DIK_D))
	{
		vDir+=D3DXVECTOR3(0.05,0,0);
		frame+=1;
		if(frame%cut>=0 && frame%cut<10 )
		{
			humot4.SetAnimTime(2.6);
		}else if(frame%cut>=10 && frame%cut<20)
		{	
			humot4.SetAnimTime(2.7);
		}else if(frame%cut>=20 && frame%cut<30)
		{
			humot4.SetAnimTime(2.8);
		}else if(frame%cut>=30 && frame%cut<40)
		{
			humot4.SetAnimTime(2.9);
		}else
		{
			humot4.SetAnimTime(3.0);
		}
		humot4.Draw();
	}else
	{
		humandraw.Draw();
	}

	//人間を回転
	if(Key(DIK_LEFT))
	{
		human.RotateYaw(-2);
		humandraw.RotateYaw(-2);
		humot1.RotateYaw(-2);
		humot2.RotateYaw(-2);
		humot3.RotateYaw(-2);
		humot4.RotateYaw(-2);
	}
	if(Key(DIK_RIGHT))
	{
		human.RotateYaw(2);
		humandraw.RotateYaw(2);
		humot1.RotateYaw(2);
		humot2.RotateYaw(2);
		humot3.RotateYaw(2);
		humot4.RotateYaw(2);
	}	

	//ジャンプ
	if(Key(DIK_SPACE) && jump==0 )
	{
		jump=1;
		jtime=0;
		Play(jump,false);
	}

	//ジャンプ中
	if(jump==1)
	{
		human.MoveWorld(iner.x,iner.y,iner.z);
		jtime+=1;
		if(jtime>60)//下降
		{
			//vDir+=D3DXVECTOR3(0,-0.05,0);
		}else{//上昇
			vDir+=vJump;
		}
	}

	for(i=0; i<boxnum; i++){
		if(sw[i]==1){
			Play(boxse,false);
			sw[i]=2;
			box[i].vPos=D3DXVECTOR3(0,-40,0);
		}
	}

	//操作からの移動に対してカメラを回り込ませる
	D3DXVec3TransformCoord(&vDir,&vDir,&human.mOrientation);

	//箱のモーション
	for(i=0;i<boxnum;i++)
	{
		box[i].RotatePitch(1);
		box[i].RotateRoll(1);
		box[i].RotateYaw(1);
	}

	//レイ判定　（下方向 地面メッシュとの判定）
	cl=CollideEx(&human.vPos,&vGrav,NULL,&ground,&vNormal,NULL);
	if(cl.boHit)
	{
		if(cl.fLength<=0.2)//地面との距離が一定以下なら
		{			
			vDir+=-vGrav;//上に持ち上げる
			iner.x=0;
			iner.y=0;
			iner.z=0;
		}

		if(jump==1 && cl.fLength<=0.2)//着地の判定
		{
			jump=0;
		}
	}

	//レイ判定	(前方 坂メッシュとの判定）
	for(i=0; i<slnum;i++)
	{
		cl=CollideEx(&human.vPos,&vDir,NULL,&slope[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=0.4)//距離が当たり距離内なら当たりにする
			{			
				vDir=Slip(&vDir,&vNormal);//滑りベクトルを計算
				iner.x=0;
				iner.y=0;
				iner.z=0;
				cl2=CollideEx(&human.vPos,&vDir,NULL,&slope[i],NULL,NULL);
				if(cl2.boHit)
				{
					if(cl.fLength<=0.4)
					{
						vDir=D3DXVECTOR3(0,0,0);
					}
				}
			}
		}
	}
	//レイ判定	(前方 坂メッシュとの判定）
	for(i=0; i<slnum;i++)
	{
		humanhead.vPos=human.vPos;
		humanhead.vPos.y+=1.3;
		cl=CollideEx(&humanhead.vPos,&vDir,NULL,&slope[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=0.2)//距離が当たり距離内なら当たりにする
			{			
				vDir=Slip(&vDir,&vNormal);//滑りベクトルを計算
				iner.x=0;
				iner.y=0;
				iner.z=0;
				cl2=CollideEx(&humanhead.vPos,&vDir,NULL,&slope[i],NULL,NULL);
				if(cl2.boHit)
				{
					if(cl.fLength<=0.2)
					{
						vDir=D3DXVECTOR3(0,0,0);
					}
				}
			}
		}
	}

	//レイ判定	(前方 坂メッシュとの判定）
	for(i=0; i<slnum;i++)
	{
		cl=CollideEx(&human.vPos,&iner,NULL,&slope[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=0.4)//距離が当たり距離内なら当たりにする
			{			
				iner.x=0;
				iner.y=0;
				iner.z=0;
				cl2=CollideEx(&human.vPos,&iner,NULL,&slope[i],NULL,NULL);
				if(cl2.boHit)
				{
					if(cl.fLength<=0.4)
					{
						iner=D3DXVECTOR3(0,0,0);
					}
				}
			}
		}
	}
	//レイ判定	(前方 坂メッシュとの判定）
	for(i=0; i<slnum;i++)
	{
		humanhead.vPos=human.vPos;
		humanhead.vPos.y+=1.3;
		cl=CollideEx(&humanhead.vPos,&iner,NULL,&slope[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=0.2)//距離が当たり距離内なら当たりにする
			{			
				iner.x=0;
				iner.y=0;
				iner.z=0;
				cl2=CollideEx(&humanhead.vPos,&iner,NULL,&slope[i],NULL,NULL);
				if(cl2.boHit)
				{
					if(cl.fLength<=0.2)
					{
						iner=D3DXVECTOR3(0,0,0);
					}
				}
			}
		}
	}

	//レイ判定　（下方向　坂メッシュとの判定）
	for(i=0; i<slnum;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&slope[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=0.2)//地面との距離が一定以下なら
			{			
				vDir+=-vGrav;//上に持ち上げる
				iner.x=0;
				iner.y=0;
				iner.z=0;
				cl2=CollideEx(&human.vPos,&vDir,NULL,&slope[i],NULL,NULL);
				if(cl2.boHit)
				{
					if(cl.fLength<=0.2)
					{
						vDir=D3DXVECTOR3(0,0,0);
					}
				}
			}
			if(jump==1 && cl.fLength<=0.4)//着地の判定
			{
				jump=0;
			}

		}
	}

	//レイ判定　（上方向　坂メッシュとの判定）
	for(i=0; i<slnum;i++)
	{
		humanhead.vPos=human.vPos;
		humanhead.vPos.y+=1.3;
		cl=CollideEx(&humanhead.vPos,&vGrav,NULL,&slope[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//プレートとの距離が一定以下なら
			{			
				vDir+=-vGrav-vJump;//下に持ち上げる
				iner.x=0;
				iner.y=0;
				iner.z=0;
				
				cl2=CollideEx(&humanhead.vPos,&vDir,NULL,&slope[i],NULL,NULL);
				if(cl2.boHit)
				{
					if(cl.fLength<=0.2)
					{
						vDir=D3DXVECTOR3(0,0,0);
					}
				}
				
			}
		}
	}
	

	//レイ判定　（下方向　板メッシュとの判定）
	for(i=0; i<plx1;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//地面との距離が一定以下なら
			{			
				vDir+=-vGrav+deltaX[i];//上に持ち上げる
				iner.x=deltaX[i].x;
				iner.y=0;
				iner.z=0;
			}	
			if(jump==1 && cl.fLength<=0.2)//着地の判定
			{
				jump=0;
			}
		}
	}

	//レイ判定　（下方向　板メッシュとの判定）
	for(i=plx1; i<plx2;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//地面との距離が一定以下なら
			{			
				vDir+=-vGrav-deltaX[i];//上に持ち上げる
				iner.x=-deltaX[i].x;
				iner.y=0;
				iner.z=0;
			}	
			if(jump==1 && cl.fLength<=0.2)//着地の判定
			{
				jump=0;
			}
		}
	}

	//レイ判定　（下方向　板メッシュとの判定）
	for(i=plx2; i<ply1;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//地面との距離が一定以下なら
			{			
				vDir+=-vGrav+deltaY[i];//上に持ち上げる
				iner.x=0;
				iner.y=deltaY[i].y;
				iner.z=0;
			}	
			if(jump==1 && cl.fLength<=0.2)//着地の判定
			{
				jump=0;
			}
		}
	}

	//レイ判定　（下方向　板メッシュとの判定）
	for(i=ply1; i<ply2;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//地面との距離が一定以下なら
			{			
				vDir+=-vGrav-deltaY[i];//上に持ち上げる
				iner.x=0;
				iner.y=-deltaY[i].y;
				iner.z=0;
			}	
			if(jump==1 && cl.fLength<=0.2)//着地の判定
			{
				jump=0;
			}
		}
	}

	//レイ判定　（下方向　板メッシュとの判定）
	for(i=ply2; i<plz1;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//地面との距離が一定以下なら
			{			
				vDir+=-vGrav+deltaZ[i];//上に持ち上げる
				iner.x=0;
				iner.y=0;
				iner.z=deltaZ[i].z;
			}	
			if(jump==1 && cl.fLength<=0.2)//着地の判定
			{
				jump=0;
			}
		}
	}

	//レイ判定　（下方向　板メッシュとの判定）
	for(i=plz1; i<plz2;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//地面との距離が一定以下なら
			{			
				vDir+=-vGrav-deltaZ[i];//上に持ち上げる
				iner.x=0;
				iner.y=0;
				iner.z=-deltaZ[i].z;
			}	
			if(jump==1 && cl.fLength<=0.2)//着地の判定
			{
				jump=0;
			}
		}
	}

	//レイ判定　（下方向　板メッシュとの判定）
	for(i=plz2; i<pl;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//地面との距離が一定以下なら
			{			
				vDir+=-vGrav;//上に持ち上げる
				iner.x=0;
				iner.y=0;
				iner.z=0;
			}	
			if(jump==1 && cl.fLength<=0.2)//着地の判定
			{
				jump=0;
			}
		}
	}

	//レイ判定　（下方向　板メッシュとの判定）
	for(i=pl; i<plyaw;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//地面との距離が一定以下なら
			{			
				vDir+=-vGrav+deltaX[i]+deltaZ[i];//上に持ち上げる
				iner.x=0;
				iner.y=0;
				iner.z=0;
			}	
			if(jump==1 && cl.fLength<=0.2)//着地の判定
			{
				jump=0;
			}
		}
	}

	//レイ判定	(前方 坂メッシュとの判定）
	for(i=plyaw; i<pldt;i++)
	{
		cl=CollideEx(&human.vPos,&vDir,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=dt)//距離が当たり距離内なら当たりにする
			{			
				vDir-=vDir*2;//滑りベクトルを計算
				iner.x=0;
				iner.y=0;
				iner.z=0;
				cl2=CollideEx(&human.vPos,&vDir,NULL,&plate[i],NULL,NULL);
				if(cl2.boHit)
				{
					if(cl.fLength<=dt)
					{
						vDir=D3DXVECTOR3(0,0,0);
					}
				}
			}
		}
	}
	
	//レイ判定	(前方 坂メッシュとの判定）
	for(i=plyaw; i<pldt;i++)
	{
		humanhead.vPos=human.vPos;
		humanhead.vPos.y+=1.3;
		cl=CollideEx(&humanhead.vPos,&vDir,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=dt)//距離が当たり距離内なら当たりにする
			{			
				vDir-=vDir*2;//滑りベクトルを計算
				iner.x=0;
				iner.y=0;
				iner.z=0;
				cl2=CollideEx(&humanhead.vPos,&vDir,NULL,&plate[i],NULL,NULL);
				if(cl2.boHit)
				{
					if(cl.fLength<=dt)
					{
						vDir=D3DXVECTOR3(0,0,0);
					}
				}
			}
		}
	}

	//レイ判定	(前方 ミサイルメッシュとの判定）
	
	cl=CollideEx(&human.vPos,&vDir,NULL,&missile,&vNormal,NULL);
	if(cl.boHit)
	{
		if(cl.fLength<=0.2)//距離が当たり距離内なら当たりにする
		{
			MessageBox(0,"ゲームクリア","",MB_OK);
			PostQuitMessage(0);
		}
	}

	//レイ判定	(前方 坂メッシュとの判定）
	for(i=plyaw; i<pldt;i++)
	{
		cl=CollideEx(&human.vPos,&iner,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=dt)//距離が当たり距離内なら当たりにする
			{			
				iner.x=0;
				iner.y=0;
				iner.z=0;
				cl2=CollideEx(&human.vPos,&iner,NULL,&plate[i],NULL,NULL);
				if(cl2.boHit)
				{
					if(cl.fLength<=dt)
					{
						iner=D3DXVECTOR3(0,0,0);
					}
				}
			}
		}
	}
	
	//レイ判定	(前方 坂メッシュとの判定）
	for(i=plyaw; i<pldt;i++)
	{
		humanhead.vPos=human.vPos;
		humanhead.vPos.y+=1.3;
		cl=CollideEx(&humanhead.vPos,&iner,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=dt)//距離が当たり距離内なら当たりにする
			{			
				iner.x=0;
				iner.y=0;
				iner.z=0;
				cl2=CollideEx(&humanhead.vPos,&iner,NULL,&plate[i],NULL,NULL);
				if(cl2.boHit)
				{
					if(cl.fLength<=dt)
					{
						iner=D3DXVECTOR3(0,0,0);
					}
				}
			}
		}
	}

	//レイ判定	(前方 箱との判定）
	for(i=0; i<boxnum;i++)
	{
		cl=CollideEx(&human.vPos,&vDir,NULL,&box[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=0.4)//距離が当たり距離内なら当たりにする
			{			
				sw[i]=1;
			}
		}
	}
	//レイ判定	(前方 箱との判定）
	for(i=0; i<boxnum;i++)
	{
		humanhead.vPos=human.vPos;
		humanhead.vPos.y+=1.3;
		cl=CollideEx(&humanhead.vPos,&vDir,NULL,&box[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=0.2)//距離が当たり距離内なら当たりにする
			{			
				sw[i]=1;
			}
		}
	}

	//レイ判定　（下方向　箱との判定）
	for(i=0; i<boxnum;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&box[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=0.2)//地面との距離が一定以下なら
			{			
				sw[i]=1;
			}
		}
	}

	//レイ判定　（上方向　箱との判定）
	for(i=0; i<slnum;i++)
	{
		humanhead.vPos=human.vPos;
		humanhead.vPos.y+=1.3;
		cl=CollideEx(&humanhead.vPos,&vGrav,NULL,&box[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//プレートとの距離が一定以下なら
			{			
				sw[i]=1;
			}
		}
	}

	//レイ判定	(前方 ミサイルメッシュとの判定）
	
	cl=CollideEx(&human.vPos,&iner,NULL,&missile,&vNormal,NULL);
	if(cl.boHit)
	{
		if(cl.fLength<=0.2)//距離が当たり距離内なら当たりにする
		{
			MessageBox(0,"ゲームクリア","",MB_OK);//ゲームクリア表示
			PostQuitMessage(0);//ゲーム終了
		}
	}


 /*

	//レイ判定　（上方向　板メッシュとの判定）
	for(i=0; i<plnum;i++)
	{
		humanhead.vPos=human.vPos;
		humanhead.vPos.y+=1.7;
		cl=CollideEx(&humanhead.vPos,&vGrav,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//プレートとの距離が一定以下なら
			{			
				vDir+=-vGrav-vJump;//下に持ち上げる
			}
		}
	}
*/
	//ゲーム内の時間を進める
	t+=1;

	//1階の板の移動
	plate[40].vPos+=deltaZ[40];
	if(t%600==0)
	{
		deltaZ[40].z=0;
	}else if(t%600==300 && t%1200!=300)
	{
		deltaZ[40].z=-sp1;
	}else if(t%600==300 && t%1200==300){
		deltaZ[40].z=sp1;
	}
	
	//2階への板の移動
	plate[20].vPos+=deltaY[20];
	if(t%1050==0)
	{
		deltaY[20].y=0;
	}else if(t%1050==300 && t%2100!=300)
	{
		deltaY[20].y=-sp2;
	}else if(t%1050==300 && t%2100==300){
		deltaY[20].y=sp2;
	}
	
	plate[30].vPos-=deltaY[30];
	if(t%1050==0)
	{
		deltaY[30].y=0;
	}else if(t%1050==300 && t%2100!=300)
	{
		deltaY[30].y=-sp2;
	}else if(t%1050==300 && t%2100==300){
		deltaY[30].y=sp2;
	}
	
	//2階の板の移動
	plate[10].vPos-=deltaX[10];
	if(t%900==0)
	{
		deltaX[10].x=0;
	}else if(t%900==300 && t%1800!=300)
	{
		deltaX[10].x=-sp1;
	}else if(t%900==300 && t%1800==300)
	{
		deltaX[10].x=sp1;
	}

	
	plate[41].vPos+=deltaZ[41];
	if(t%900==0)
	{
		deltaZ[41].z=0;
	}else if(t%900==300  && t%1800!=300)
	{
		deltaZ[41].z=-sp1;
	}else if(t%900==300 && t%1800==300){
		deltaZ[41].z=sp1;
	}

	for(i=50; i<52; i++){
		plate[i].vPos-=deltaZ[i];
		if(t%900==0)
		{
			deltaZ[i].z=0;
		}else if(t%900==300  && t%1800!=300)
		{
			deltaZ[i].z=-sp1;
		}else if(t%900==300 && t%1800==300){
			deltaZ[i].z=sp1;
		}
	}

	for(i=70; i<76; i++)
	{
		plate[i].RotateYaw(1);
		disx[i-70]=plate[i].vPos.x-human.vPos.x;
		disz[i-70]=plate[i].vPos.z-human.vPos.z;
		ang=-30;
		deltaX[i].x=(cosf(ang)*disx[i-70]-sinf(ang)*disz[i-70])/60;
		deltaZ[i].z=(sinf(ang)*disx[i-70]+cosf(ang)*disz[i-70])/60;
	}

	for(i=42; i<47; i++)
	{
		plate[i].vPos+=deltaZ[i];
		if(t%1500==0)
		{
			deltaZ[i].z=0;
		}else if(t%1500==300  && t%3000!=300)
		{
			deltaZ[i].z=-sp1;
		}else if(t%1500==300 && t%3000==300){
			deltaZ[i].z=sp1;
		}
	}

	//箱の移動
	if(t%160>=0 && t%160<20){
		box[3].vPos.x-=sp1;
	}
	if(t%160>=20 && t%160<80){
		box[3].vPos.z-=sp1;
	}
	if(t%160>=80 && t%160<100){
		box[3].vPos.x+=sp1;
	}
	if(t%160>=100 && t%160<160){
		box[3].vPos.z+=sp1;
	}

	//実際の移動
	human.MoveWorld(vDir.x,vDir.y,vDir.z);

	//カメラを人間の後ろに配置。方向は人間の方を向くようにする
	D3DXVECTOR3 v(0,2,-7);//操作キャラから見たカメラの位置ベクトル
	if(Key(DIK_UP))//視点変更（上から）
	{
		v.y+=5;
	}
	if(Key(DIK_DOWN))//視点変更（後方から）
	{
		v.y-=2;
		v.z-=13;
	}
	D3DXVec3TransformCoord(&v,&v,&human.mOrientation);//回転行列
	v+=human.vPos;//回転後に操作キャラの座標を足す
	Camera.SetPosition(v.x,v.y,v.z);//位置ベクトルをカメラに代入
	Camera.vLook=human.vPos+D3DXVECTOR3(0,1,0);	//カメラの向きを操作キャラに向ける

	//ライト照射
	PLight.Illuminate();
	/*
	DLight[0].Illuminate();
	DLight[1].Illuminate();
	DLight[2].Illuminate();
	DLight[3].Illuminate();
	DLight[4].Illuminate();
	*/

	/*
	//レイを描画（本来は不要）
	RenderRay(&human.vPos,&vDir,NULL,1000);
	RenderRay(&humanhead.vPos,&vDir,NULL,1000);
	RenderRay(&human.vPos,&vGrav,NULL,1000);
	*/

	//描画
	ground.Draw();
	missile.Draw();
	for(i=0;i<slnum;i++)
	{
		slope[i].Draw();
	}
	if(sw[0]>=1){
		plate[40].Draw();
	}
	if(sw[1]>=1){
		plate[20].Draw();
		plate[30].Draw();
	}
	if(sw[2]>=1){
		plate[10].Draw();
		plate[41].Draw();
	}
	if(sw[3]>=1){
		plate[50].Draw();
		plate[51].Draw();
	}
	if(sw[4]>=1){
		plate[42].Draw();
		plate[43].Draw();
		plate[44].Draw();
		plate[45].Draw();
		plate[46].Draw();
	}
	for(i=plz2;i<plnum;i++)
	{
		plate[i].Draw();
	}
	for(i=0;i<boxnum;i++)
	{
		box[i].Draw();	
	}
	for(i=0;i<fontnum;i++)
	{
		fm[i].Draw();
	}
}

D3DXVECTOR3 Slip(D3DXVECTOR3* l,D3DXVECTOR3* n)
{	  
	D3DXVECTOR3 L;//入射ベクトル（レイ）
	D3DXVECTOR3 N;//ポリゴンの法線
	D3DXVECTOR3 S; //滑りベクトル（滑る方向）
	L=*l;
	N=*n;
	//滑りベクトル S=L-(N * L)/(|N|^2)*N
	S=L - (( D3DXVec3Dot(&N,&L) ) / ( pow(D3DXVec3Length(&N) ,2) ) ) * N;

	return S;
}