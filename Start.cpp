#include "NEGAA.h"

//���̒�`
#define slnum 20//��̐�
#define plx1 10//x���𐳂Ɉړ�����̐�
#define plx2 20//x���𕉂Ɉړ�����̐�
#define ply1 30//�����𐳂Ɉړ�����̐�
#define ply2 40//�����𕉂Ɉړ�����̐�
#define plz1 50//�����𐳂Ɉړ�����̐�
#define plz2 60//�����𕉂Ɉړ�����̐�
#define pl 70//�Î~���Ă���̐�
#define plyaw 80//�����ɉ�]����̐�
#define pldt 130//��Q���̔̐�
#define plnum 130//�̑���
#define boxnum 5//���̑���
#define linum 5//���C�g�̐�
#define fontnum 10//�t�H���g���b�V���̐�


//�����b�V��
XMESH human;//����L����
XMESH humanhead;//����L�����̓�
XMESH humandraw;//��ʂɕ`�ʂ��鑀��L����
XMESH humot1;//����L�����̃��[�V�����i�O���j
XMESH humot2;//����L�����̃��[�V�����i����j
XMESH humot3;//����L�����̃��[�V�����i�����j
XMESH humot4;//����L�����̃��[�V�����i�E���j
XMESH ground;//�X�e�[�W
XMESH slope[slnum];//��
XMESH plate[plnum];//��
XMESH missile;//�~�T�C��
XMESH box[boxnum];//��

//�ϐ�
int i=0;//���������̃J�E���g
int t=0;//�Q�[�����̎���
int jump=0;//�W�����v�̃J�E���g
int jtime=0;//�W�����v�̎���
int frame;//���[�V�������̃t���[����
int cut=50;//1���[�V����������̃t���[����
int remun=9;//�c�莞�ԁi���j
int resec=59;//�c�莞�ԁi�b�j
int mission=0;//�~�b�V�����\���̃J�E���g
int ope=0;//��������\���̃J�E���g
int sw[boxnum];//���̃X�C�b�`

float sp1=0.1;//�ړ�����̃X�s�[�h
float sp2=0.01;//�ړ�����̃X�s�[�h
float ang=0;//��]�̊p�x
float dt=0.2;//�����蔻��̋���
float disx[10];//����L�����Ɣ�x���̋���
float disz[10];//����L�����Ɣ̂����̋���
float lumi=0.5;//���̋���

//�x�N�g��
D3DXVECTOR3 vNormal;//�A�h���X
D3DXVECTOR3 vDir;//����
D3DXVECTOR3 iner(0,0,0);//����
D3DXVECTOR3 vGrav(0,-0.05,0);//�d��
D3DXVECTOR3 vJump(0,0.15,0);//�W�����v
D3DXVECTOR3 deltaX[plnum];//��x���ړ�
D3DXVECTOR3 deltaY[plnum];//��y���ړ�
D3DXVECTOR3 deltaZ[plnum];//��z���ړ�
D3DXVECTOR3 Slip(D3DXVECTOR3* L,D3DXVECTOR3* N);//����

RAY_COLLIDE cl;//���C����
RAY_COLLIDE cl2;//���C����
DISTANT_LIGHT DLight[linum];//�������C�g
POINT_LIGHT PLight;//�������C�g
DWORD robotbgm=0;//�X�e�[�WBGM
DWORD jumpse=0;//�����Ղ�SE
DWORD boxse=0;//����SE
FONT font;//�t�H���g
FONTMESH fm[fontnum];//�t�H���g���b�V��


void MY_NEGAA::OneTime()
{	
	//�����b�V���̓ǂݍ���
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
	
	//�P�K�̍�̍��W
	slope[0].vPos=D3DXVECTOR3(0,0,10);
	slope[1].vPos=D3DXVECTOR3(0,2.37,20.4);
	slope[2].vPos=D3DXVECTOR3(0,4.74,30.8);
	slope[3].vPos=D3DXVECTOR3(0,7.11,71.2);
	slope[3].RotateRoll(180);
	slope[4].vPos=D3DXVECTOR3(0,4.71,71.2);
	slope[4].RotateYaw(180);

	//�Q�K�̍�̍��W
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
	
	//��1
	box[0].vPos=D3DXVECTOR3(-1,10,35);

	//�P�K�̔̍��W
	plate[40].vPos=D3DXVECTOR3(0,7.2,36);
	plate[40].scale=3.0;

	plate[110].vPos=D3DXVECTOR3(0,8.7,50);
	plate[110].scale=3.0;
	plate[110].RotatePitch(90);

	plate[67].vPos=D3DXVECTOR3(0,7.2,82);
	plate[67].scale=3.0;

	//��2
	box[1].vPos=D3DXVECTOR3(0,10,82);

	//�Q�K�ւ̔̍��W
	plate[20].vPos=D3DXVECTOR3(0,7.2,76);
	plate[20].scale=3.0;

	plate[30].vPos=D3DXVECTOR3(0,22.2,73);
	plate[30].scale=3.0;

	//��3
	box[2].vPos=D3DXVECTOR3(0,28.2,65);

	//�Q�K�̔̍��W
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

	//��4
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

	//��5
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

	//�t�H���g
	font.Load("arial",20,30,D3DCOLOR_XRGB(0,255,255));
	for(i=0; i<fontnum; i++)
	{
		fm[i].Load(L"��");
		fm[i].vPos=D3DXVECTOR3(0,-20,0);
	}
	fm[0].vPos=D3DXVECTOR3(0,25,70);
	fm[0].RotateYaw(180);
	fm[1].vPos=D3DXVECTOR3(-65,25,0);
	fm[1].RotateYaw(90);
	fm[2].vPos=D3DXVECTOR3(0,25,-65);
	fm[3].vPos=D3DXVECTOR3(75,25,-55);
	fm[3].RotateYaw(-90);
	
	//���̓ǂݍ���
	LoadSound(&robotbgm,"wav/robotbgm.wav",0,0,true);
	LoadSound(&jumpse,"wav/jump.wav",0,0,true);
	LoadSound(&boxse,"wav/box.wav",0,0,true);

	//�J����
	Camera.SetPosition(0,1,0);

	//���C�g�̊p�x
	Light.SetAngle(0,90,0);

	//����L�����̃��C�g�̋P�x
	PLight.SetLuminance(lumi);

}

void MY_NEGAA::Main()
{
	//BGM
	if(t>1)
	{
		Play(robotbgm,true);
	}

	//�ړI
	if(mission%2==0 && t>0)
	{
		MessageBox(0,"�~�b�V�����F\n\n�E�~�T�C���̔��˂��~�߂�\n�E�~�T�C���ւ̓��؂͖ڂ̑O�̊K�i���瓹�Ȃ�ɂ܂�������\n�E��]����L���[�u�����ƐV���ȑ��ꂪ�o��\n\n���̃E�B���h�E�����FENTER","",MB_OK);
		mission++;
	}

	if(Key(DIK_M))
	{
		mission++;
	}

	//�������
	if(ope%2==0 && t>0)
	{
		MessageBox(0,"��������F\n\n�E�O�i�FW�@�E���_�ύX�i���j�F���iPGUP�j�@�E�W�����v�FSPACE\n�E���i�FA�@ �E���ɐ���@�@�@�@�F���iHOME�j\n�E�E�i�FD�@ �E�E�֐���@�@�@�@�F���iEND�j\n�E��i�FS�@ �E���_�ύX�i����j�F���iPGDN�j\n\n���̃E�B���h�E�����FENTER","",MB_OK);
		ope++;
	}

	if(Key(DIK_O))
	{
		ope++;
	}


	//�t�H���g
	char moji[100];
	sprintf(moji,"�~�T�C�����˂܂ł���:%2d��:%2d�b\n�~�b�V�����FM\n��������@�FO",remun-(t/3600)%3600,resec-(t/60)%60);

	font.vPos=D3DXVECTOR2(30,20);
	font.Draw(moji);

	//�Q�[���I�[�o�[
	if(remun-(t/3600)%3600==0 && resec-(t/60)%60<0)
	{
		MessageBox(0,"�Q�[���I�|�o�[","",MB_OK);//�Q�[���I�[�o�[�\��
		PostQuitMessage(0);//�Q�[���I��
	}

	//�����x�N�g��
	vDir=D3DXVECTOR3(0,0,0);

	//�d��
	vDir+=vGrav;

	//����L�����̃��C�g
	PLight.vPos=human.vPos;
	PLight.vPos.y+=1.7;

	//�`�ʂ��鑀��L����
	humandraw.vPos=human.vPos;
	humandraw.vPos.y=human.vPos.y-0.2;

	//����L�����̃��[�V����
	humot1.vPos=human.vPos;
	humot1.vPos.y=human.vPos.y-0.2;
	humot2.vPos=human.vPos;
	humot2.vPos.y=human.vPos.y-0.2;
	humot3.vPos=human.vPos;
	humot3.vPos.y=human.vPos.y-0.2;
	humot4.vPos=human.vPos;
	humot4.vPos.y=human.vPos.y-0.2;

	//����L�����̈ړ�
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

	//�l�Ԃ���]
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

	//�W�����v
	if(Key(DIK_SPACE) && jump==0 )
	{
		jump=1;
		jtime=0;
		Play(jump,false);
	}

	//�W�����v��
	if(jump==1)
	{
		human.MoveWorld(iner.x,iner.y,iner.z);
		jtime+=1;
		if(jtime>60)//���~
		{
			//vDir+=D3DXVECTOR3(0,-0.05,0);
		}else{//�㏸
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

	//���삩��̈ړ��ɑ΂��ăJ��������荞�܂���
	D3DXVec3TransformCoord(&vDir,&vDir,&human.mOrientation);

	//���̃��[�V����
	for(i=0;i<boxnum;i++)
	{
		box[i].RotatePitch(1);
		box[i].RotateRoll(1);
		box[i].RotateYaw(1);
	}

	//���C����@�i������ �n�ʃ��b�V���Ƃ̔���j
	cl=CollideEx(&human.vPos,&vGrav,NULL,&ground,&vNormal,NULL);
	if(cl.boHit)
	{
		if(cl.fLength<=0.2)//�n�ʂƂ̋��������ȉ��Ȃ�
		{			
			vDir+=-vGrav;//��Ɏ����グ��
			iner.x=0;
			iner.y=0;
			iner.z=0;
		}

		if(jump==1 && cl.fLength<=0.2)//���n�̔���
		{
			jump=0;
		}
	}

	//���C����	(�O�� �⃁�b�V���Ƃ̔���j
	for(i=0; i<slnum;i++)
	{
		cl=CollideEx(&human.vPos,&vDir,NULL,&slope[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=0.4)//�����������苗�����Ȃ瓖����ɂ���
			{			
				vDir=Slip(&vDir,&vNormal);//����x�N�g�����v�Z
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
	//���C����	(�O�� �⃁�b�V���Ƃ̔���j
	for(i=0; i<slnum;i++)
	{
		humanhead.vPos=human.vPos;
		humanhead.vPos.y+=1.3;
		cl=CollideEx(&humanhead.vPos,&vDir,NULL,&slope[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=0.2)//�����������苗�����Ȃ瓖����ɂ���
			{			
				vDir=Slip(&vDir,&vNormal);//����x�N�g�����v�Z
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

	//���C����	(�O�� �⃁�b�V���Ƃ̔���j
	for(i=0; i<slnum;i++)
	{
		cl=CollideEx(&human.vPos,&iner,NULL,&slope[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=0.4)//�����������苗�����Ȃ瓖����ɂ���
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
	//���C����	(�O�� �⃁�b�V���Ƃ̔���j
	for(i=0; i<slnum;i++)
	{
		humanhead.vPos=human.vPos;
		humanhead.vPos.y+=1.3;
		cl=CollideEx(&humanhead.vPos,&iner,NULL,&slope[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=0.2)//�����������苗�����Ȃ瓖����ɂ���
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

	//���C����@�i�������@�⃁�b�V���Ƃ̔���j
	for(i=0; i<slnum;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&slope[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=0.2)//�n�ʂƂ̋��������ȉ��Ȃ�
			{			
				vDir+=-vGrav;//��Ɏ����グ��
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
			if(jump==1 && cl.fLength<=0.4)//���n�̔���
			{
				jump=0;
			}

		}
	}

	//���C����@�i������@�⃁�b�V���Ƃ̔���j
	for(i=0; i<slnum;i++)
	{
		humanhead.vPos=human.vPos;
		humanhead.vPos.y+=1.3;
		cl=CollideEx(&humanhead.vPos,&vGrav,NULL,&slope[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//�v���[�g�Ƃ̋��������ȉ��Ȃ�
			{			
				vDir+=-vGrav-vJump;//���Ɏ����グ��
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
	

	//���C����@�i�������@���b�V���Ƃ̔���j
	for(i=0; i<plx1;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//�n�ʂƂ̋��������ȉ��Ȃ�
			{			
				vDir+=-vGrav+deltaX[i];//��Ɏ����グ��
				iner.x=deltaX[i].x;
				iner.y=0;
				iner.z=0;
			}	
			if(jump==1 && cl.fLength<=0.2)//���n�̔���
			{
				jump=0;
			}
		}
	}

	//���C����@�i�������@���b�V���Ƃ̔���j
	for(i=plx1; i<plx2;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//�n�ʂƂ̋��������ȉ��Ȃ�
			{			
				vDir+=-vGrav-deltaX[i];//��Ɏ����グ��
				iner.x=-deltaX[i].x;
				iner.y=0;
				iner.z=0;
			}	
			if(jump==1 && cl.fLength<=0.2)//���n�̔���
			{
				jump=0;
			}
		}
	}

	//���C����@�i�������@���b�V���Ƃ̔���j
	for(i=plx2; i<ply1;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//�n�ʂƂ̋��������ȉ��Ȃ�
			{			
				vDir+=-vGrav+deltaY[i];//��Ɏ����グ��
				iner.x=0;
				iner.y=deltaY[i].y;
				iner.z=0;
			}	
			if(jump==1 && cl.fLength<=0.2)//���n�̔���
			{
				jump=0;
			}
		}
	}

	//���C����@�i�������@���b�V���Ƃ̔���j
	for(i=ply1; i<ply2;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//�n�ʂƂ̋��������ȉ��Ȃ�
			{			
				vDir+=-vGrav-deltaY[i];//��Ɏ����グ��
				iner.x=0;
				iner.y=-deltaY[i].y;
				iner.z=0;
			}	
			if(jump==1 && cl.fLength<=0.2)//���n�̔���
			{
				jump=0;
			}
		}
	}

	//���C����@�i�������@���b�V���Ƃ̔���j
	for(i=ply2; i<plz1;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//�n�ʂƂ̋��������ȉ��Ȃ�
			{			
				vDir+=-vGrav+deltaZ[i];//��Ɏ����グ��
				iner.x=0;
				iner.y=0;
				iner.z=deltaZ[i].z;
			}	
			if(jump==1 && cl.fLength<=0.2)//���n�̔���
			{
				jump=0;
			}
		}
	}

	//���C����@�i�������@���b�V���Ƃ̔���j
	for(i=plz1; i<plz2;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//�n�ʂƂ̋��������ȉ��Ȃ�
			{			
				vDir+=-vGrav-deltaZ[i];//��Ɏ����グ��
				iner.x=0;
				iner.y=0;
				iner.z=-deltaZ[i].z;
			}	
			if(jump==1 && cl.fLength<=0.2)//���n�̔���
			{
				jump=0;
			}
		}
	}

	//���C����@�i�������@���b�V���Ƃ̔���j
	for(i=plz2; i<pl;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//�n�ʂƂ̋��������ȉ��Ȃ�
			{			
				vDir+=-vGrav;//��Ɏ����グ��
				iner.x=0;
				iner.y=0;
				iner.z=0;
			}	
			if(jump==1 && cl.fLength<=0.2)//���n�̔���
			{
				jump=0;
			}
		}
	}

	//���C����@�i�������@���b�V���Ƃ̔���j
	for(i=pl; i<plyaw;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//�n�ʂƂ̋��������ȉ��Ȃ�
			{			
				vDir+=-vGrav+deltaX[i]+deltaZ[i];//��Ɏ����グ��
				iner.x=0;
				iner.y=0;
				iner.z=0;
			}	
			if(jump==1 && cl.fLength<=0.2)//���n�̔���
			{
				jump=0;
			}
		}
	}

	//���C����	(�O�� �⃁�b�V���Ƃ̔���j
	for(i=plyaw; i<pldt;i++)
	{
		cl=CollideEx(&human.vPos,&vDir,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=dt)//�����������苗�����Ȃ瓖����ɂ���
			{			
				vDir-=vDir*2;//����x�N�g�����v�Z
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
	
	//���C����	(�O�� �⃁�b�V���Ƃ̔���j
	for(i=plyaw; i<pldt;i++)
	{
		humanhead.vPos=human.vPos;
		humanhead.vPos.y+=1.3;
		cl=CollideEx(&humanhead.vPos,&vDir,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=dt)//�����������苗�����Ȃ瓖����ɂ���
			{			
				vDir-=vDir*2;//����x�N�g�����v�Z
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

	//���C����	(�O�� �~�T�C�����b�V���Ƃ̔���j
	
	cl=CollideEx(&human.vPos,&vDir,NULL,&missile,&vNormal,NULL);
	if(cl.boHit)
	{
		if(cl.fLength<=0.2)//�����������苗�����Ȃ瓖����ɂ���
		{
			MessageBox(0,"�Q�[���N���A","",MB_OK);
			PostQuitMessage(0);
		}
	}

	//���C����	(�O�� �⃁�b�V���Ƃ̔���j
	for(i=plyaw; i<pldt;i++)
	{
		cl=CollideEx(&human.vPos,&iner,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=dt)//�����������苗�����Ȃ瓖����ɂ���
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
	
	//���C����	(�O�� �⃁�b�V���Ƃ̔���j
	for(i=plyaw; i<pldt;i++)
	{
		humanhead.vPos=human.vPos;
		humanhead.vPos.y+=1.3;
		cl=CollideEx(&humanhead.vPos,&iner,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=dt)//�����������苗�����Ȃ瓖����ɂ���
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

	//���C����	(�O�� ���Ƃ̔���j
	for(i=0; i<boxnum;i++)
	{
		cl=CollideEx(&human.vPos,&vDir,NULL,&box[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=0.4)//�����������苗�����Ȃ瓖����ɂ���
			{			
				sw[i]=1;
			}
		}
	}
	//���C����	(�O�� ���Ƃ̔���j
	for(i=0; i<boxnum;i++)
	{
		humanhead.vPos=human.vPos;
		humanhead.vPos.y+=1.3;
		cl=CollideEx(&humanhead.vPos,&vDir,NULL,&box[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=0.2)//�����������苗�����Ȃ瓖����ɂ���
			{			
				sw[i]=1;
			}
		}
	}

	//���C����@�i�������@���Ƃ̔���j
	for(i=0; i<boxnum;i++)
	{
		cl=CollideEx(&human.vPos,&vGrav,NULL,&box[i],&vNormal,NULL);
		if(cl.boHit)
		{		
			if(cl.fLength<=0.2)//�n�ʂƂ̋��������ȉ��Ȃ�
			{			
				sw[i]=1;
			}
		}
	}

	//���C����@�i������@���Ƃ̔���j
	for(i=0; i<slnum;i++)
	{
		humanhead.vPos=human.vPos;
		humanhead.vPos.y+=1.3;
		cl=CollideEx(&humanhead.vPos,&vGrav,NULL,&box[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//�v���[�g�Ƃ̋��������ȉ��Ȃ�
			{			
				sw[i]=1;
			}
		}
	}

	//���C����	(�O�� �~�T�C�����b�V���Ƃ̔���j
	
	cl=CollideEx(&human.vPos,&iner,NULL,&missile,&vNormal,NULL);
	if(cl.boHit)
	{
		if(cl.fLength<=0.2)//�����������苗�����Ȃ瓖����ɂ���
		{
			MessageBox(0,"�Q�[���N���A","",MB_OK);//�Q�[���N���A�\��
			PostQuitMessage(0);//�Q�[���I��
		}
	}


 /*

	//���C����@�i������@���b�V���Ƃ̔���j
	for(i=0; i<plnum;i++)
	{
		humanhead.vPos=human.vPos;
		humanhead.vPos.y+=1.7;
		cl=CollideEx(&humanhead.vPos,&vGrav,NULL,&plate[i],&vNormal,NULL);
		if(cl.boHit)
		{
			if(cl.fLength<=0.2)//�v���[�g�Ƃ̋��������ȉ��Ȃ�
			{			
				vDir+=-vGrav-vJump;//���Ɏ����グ��
			}
		}
	}
*/
	//�Q�[�����̎��Ԃ�i�߂�
	t+=1;

	//1�K�̔̈ړ�
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
	
	//2�K�ւ̔̈ړ�
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
	
	//2�K�̔̈ړ�
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

	//���̈ړ�
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

	//���ۂ̈ړ�
	human.MoveWorld(vDir.x,vDir.y,vDir.z);

	//�J������l�Ԃ̌��ɔz�u�B�����͐l�Ԃ̕��������悤�ɂ���
	D3DXVECTOR3 v(0,2,-7);//����L�������猩���J�����̈ʒu�x�N�g��
	if(Key(DIK_UP))//���_�ύX�i�ォ��j
	{
		v.y+=5;
	}
	if(Key(DIK_DOWN))//���_�ύX�i�������j
	{
		v.y-=2;
		v.z-=13;
	}
	D3DXVec3TransformCoord(&v,&v,&human.mOrientation);//��]�s��
	v+=human.vPos;//��]��ɑ���L�����̍��W�𑫂�
	Camera.SetPosition(v.x,v.y,v.z);//�ʒu�x�N�g�����J�����ɑ��
	Camera.vLook=human.vPos+D3DXVECTOR3(0,1,0);	//�J�����̌����𑀍�L�����Ɍ�����

	//���C�g�Ǝ�
	PLight.Illuminate();
	/*
	DLight[0].Illuminate();
	DLight[1].Illuminate();
	DLight[2].Illuminate();
	DLight[3].Illuminate();
	DLight[4].Illuminate();
	*/

	/*
	//���C��`��i�{���͕s�v�j
	RenderRay(&human.vPos,&vDir,NULL,1000);
	RenderRay(&humanhead.vPos,&vDir,NULL,1000);
	RenderRay(&human.vPos,&vGrav,NULL,1000);
	*/

	//�`��
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
	D3DXVECTOR3 L;//���˃x�N�g���i���C�j
	D3DXVECTOR3 N;//�|���S���̖@��
	D3DXVECTOR3 S; //����x�N�g���i��������j
	L=*l;
	N=*n;
	//����x�N�g�� S=L-(N * L)/(|N|^2)*N
	S=L - (( D3DXVec3Dot(&N,&L) ) / ( pow(D3DXVec3Length(&N) ,2) ) ) * N;

	return S;
}