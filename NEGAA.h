#pragma once
#define _CRT_SECURE_NO_DEPRECATE

#pragma warning(disable : 4793)
#pragma warning(disable : 4819)
#pragma warning(disable : 4996)
#pragma warning(disable : 4805)
#pragma warning(disable : 4018)
#pragma warning(disable : 4244)
#pragma warning(disable : 4101)
#pragma warning(disable : 4803)
#pragma warning(disable : 4305)
#pragma warning(disable : 4267)
#pragma warning(disable : 4005)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <crtdbg.h>
#include <windows.h>
#include <d3dx9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Dsound.h>


#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }

#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#define MFAIL(code,string) if(FAILED( code ) ) { MessageBox(0,string,"error",MB_OK); return E_FAIL; }

#define MEM_NEW(p,t) { p=NULL;p= new t;if(!p){ \
	MessageBox(0,"p","memory allocate failed",MB_OK); return E_OUTOFMEMORY;} }

#define NEGAA_MAX_OBJECT 5000
#define MAX_MIDI 500
#define MAX_ANIM_CLONE 50

//
//文字列　表示用
//
void RenderString(LPSTR str,int x,int y);


//
//
//
enum HRTF_LEVEL
{
	HRTF_LEVEL_NULL,
	HRTF_NONE,
	HRTF_LOW,
	HRTF_HIGH,
};
//
//
//
enum EFFECTOR
{
	EFFECTOR_NULL,
	GARGLE,
	CHORUS,
	FLANGER,
	ECHO,
	KODAMA,
	DISTORTION,
	COMPRESSOR,
	PARAEQ,
	I3DL2,
	WAVES
};
//
//
//
enum EFFECT_PRESET
{
	EFFECT_PRESET_NULL,
	//現実系
	TUNNEL,
	MOUNTAIN_ECHO,
	BRICK_BIG_WALL,
	WATER,
	BIG_WIND,
	TRANSCEIVER,
	SENTOU,
	OLD_RADIO,
	//変換系
	CREATURE,
	ROBOT,
	SPACEMAN,
	CLEAR_VOICE,
	CLEAR_VOICE2,
	TAPE_FF,
	TAPE_FF_x2,
	TAPE_FF_x4,
	TAPE_SLOW,
	TAPE_SLOW_x2,
	TAPE_SLOW_x4,
	GAIJIN,

};

//
//
//
enum NEGAA_OBJ_TYPE
{
	TYPE_NULL,
	TYPE_BOX,
	TYPE_SPHERE,
	TYPE_CYLINDER,
	TYPE_TORUS,
	TYPE_TEAPOT,
	TYPE_FONTMESH,
	TYPE_XMESH,
	TYPE_SPRITE2D,
	TYPE_SPRITE3D,
	TYPE_BILLBOARD,
	TYPE_DISTANTLIGHT,
	TYPE_POINTLIGHT,
	TYPE_SPOTLIGHT,
	TYPE_CAMERA,
	TYPE_RESERVED1,
	TYPE_RESERVED2,

};

enum FILL_MODE
{
	FILL_POINT,
	FILL_WIRE,
	FILL_SOLID
};

enum SHADE_MODE
{
	SHADE_FLAT,
	SHADE_SOFT,
	SHADE_PHONG,
};
//
//
//
struct RECORD
{
	float Key;//Z値
	NEGAA_OBJ_TYPE Type;
	void* pObj;
};
#define RECORD_SWAP(a,b) RECORD tmp; memcpy(&tmp,&a,sizeof(RECORD)); memcpy(&a,&b,sizeof(RECORD))  ; memcpy(&b,&tmp,sizeof(RECORD));
#define RECORD_COPY(a,b) memcpy(&a,&b,sizeof(RECORD));

//
//
// 
struct THING
{
	LPD3DXFRAME pFrameRoot;
    LPD3DXANIMATIONCONTROLLER pAnimController;
	
	THING()
	{
		ZeroMemory(this,sizeof(THING));
	}
};
//
//
//
class RENDER_QUE
{
public:
	bool boAlphaSort;
	RENDER_QUE();
	
	RECORD m_Obj[NEGAA_MAX_OBJECT];

	int m_iNumObj;
	HRESULT Insert(NEGAA_OBJ_TYPE type,void* pObj);
	HRESULT Render(D3DXMATRIX* mView);
	HRESULT Sort();
	void Heap(RECORD* pData,INT Amount);
protected:
	D3DXMATRIX m_mView;
};
//
//
//
struct NEGAA_OBJECT
{
	NEGAA_OBJ_TYPE Type;
	void* pObj;
};
//
//
//
class OBJECT_MANAGER
{
public:
	OBJECT_MANAGER();
	DWORD m_dwNumObject;
	NEGAA_OBJECT m_Obj[ NEGAA_MAX_OBJECT ];
	
	HRESULT Insert(NEGAA_OBJ_TYPE type,void* pObj,DWORD* pdwIndex);
	HRESULT Delete(DWORD dwIndex);

};
//
//
//
class THING_BASE
{
public:
	//変数
	DWORD m_dwMyIndex;
	OBJECT_MANAGER* m_pManager;
	float alpha;
	NEGAA_OBJ_TYPE m_Type;
	D3DXVECTOR3 vPos;
	
	float r;
	D3DXVECTOR3 vCenter;

	float scale;
	D3DXVECTOR3 vAxisX;
	D3DXVECTOR3 vAxisY;
	D3DXVECTOR3 vAxisZ;
	D3DXMATRIX mOrientation;
	LPD3DXMESH m_pMesh;
	D3DMATERIAL9 MeshMaterial;	
	D3DXVECTOR3 vDirection;
	
	LPDIRECT3DDEVICE9 m_pDevice;
	bool boDevIsNull;
	bool m_boLoaded;
	bool m_boLoadRequired;
	D3DXMATRIX mWorld;
	float fRange;
	RENDER_QUE* m_pRenderQue;
	float m_NoseYaw;
	float m_NosePitch;
	float m_NoseRoll;
	D3DXMATRIX mOuterGimbal;
	D3DXMATRIX mMiddleGimbal;
	D3DXMATRIX mInnerGimbal;
	D3DXMATRIX mParent;
	bool m_boLighting;

	//関数
	THING_BASE();
	HRESULT ResetPosition();
	HRESULT ResetAngle();
	HRESULT ResetScale();
	HRESULT SetPosition(float x,float y,float z);
	HRESULT MoveLocal(float x,float y,float z);
	HRESULT MoveWorld(float x,float y,float z);
	HRESULT SetAngle(float yaw,float pitch,float roll);
	HRESULT RotateYaw(float yaw);
	HRESULT RotatePitch(float pitch);
	HRESULT RotateRoll(float roll);	
	HRESULT RotateAxis(D3DXVECTOR3* pvAxis,float angle);
	HRESULT SetYaw(float y);
	HRESULT SetPitch(float p);
	HRESULT SetRoll(float r);
	HRESULT GetYaw(float* y);
	HRESULT GetPitch(float* p);
	HRESULT GetRoll(float* r);
	HRESULT SetColor(BYTE a,BYTE r,BYTE g,BYTE b);
	HRESULT SetRange(float range);
	HRESULT AdjustFaceDirection(float yaw,float pitch,float roll);

protected:
	float yaw;
	float pitch;
	float roll;
	
};

//
//
//デフォルトで、注視点は視点から(0,0,1)の位置。
class CAMERA
{
public:
	DWORD m_dwMyIndex;
	OBJECT_MANAGER* m_pManager;
	float alpha;
	NEGAA_OBJ_TYPE m_Type;	
	float yaw;
	float pitch;
	float roll;
	float r;
	D3DXVECTOR3 vCenter;
	float scale;
	D3DXVECTOR3 vAxisX;
	D3DXVECTOR3 vAxisY;
	D3DXVECTOR3 vAxisZ;
	D3DXMATRIX mOrientation;
	LPD3DXMESH m_pMesh;
	D3DMATERIAL9 MeshMaterial;	
	D3DXVECTOR3 vDirection;	
	LPDIRECT3DDEVICE9 m_pDevice;
	bool boDevIsNull;
	bool m_boLoaded;
	bool m_boLoadRequired;
	D3DXMATRIX mWorld;
	float fRange;
	RENDER_QUE* m_pRenderQue;
	float m_NoseYaw;
	float m_NosePitch;
	float m_NoseRoll;	
	
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vUp;
	float fZoom;
	bool boShowGeometry;
	bool boShowGeometryHold;
	bool boMeshCreated;
	
	CAMERA();
	~CAMERA();



	HRESULT ResetPosition();
	HRESULT ResetAngle();
	HRESULT ResetScale();
	HRESULT SetAngle(float yaw,float pitch,float roll);
	HRESULT SetColor(BYTE a,BYTE r,BYTE g,BYTE b);
	HRESULT SetRange(float range);
	HRESULT AdjustFaceDirection(float yaw,float pitch,float roll);



	HRESULT MoveLocal(float x,float y,float z);
	HRESULT MoveWorld(float x,float y,float z);
	HRESULT Reset();
	HRESULT Zoom(float zoom);
	HRESULT RotateYaw(float yaw);
	HRESULT RotatePitch(float pitch);
	HRESULT RotateRoll(float roll);
	HRESULT SetEyeDirection(D3DXVECTOR3* pv);
	HRESULT Draw();
	HRESULT TrueDraw();
	HRESULT SetPosition(float x,float y,float z);
	HRESULT SetLookPoint(float x,float y,float z);
	HRESULT CAMERA::GetPosition(float* x,float* y,float* z);
	void ShowCameraObject(bool toggle);
	D3DXVECTOR3 GetEyeVEctor();
	HRESULT Init();
protected:
	
	D3DXVECTOR3 vPos;
};


//レイでの当たり判定用　情報構造体
struct RAY_COLLIDE
{
	BOOL boHit;
	float fLength;
};


//
//Distant light
//
class DISTANT_LIGHT : public THING_BASE
{
public:
	DWORD m_dwIndex;

	DISTANT_LIGHT();
	~DISTANT_LIGHT();	

	HRESULT Illuminate();
	HRESULT SetLuminance(float lumi);
	HRESULT SetColor(BYTE r,BYTE g,BYTE b);
	HRESULT SetAngle(float yaw,float pitch,float roll);

	void ShowLightObject(bool toggle);
	HRESULT ResetAngle();
	HRESULT Draw();
	HRESULT TrueDraw();

	bool boShowGeometry;
	bool boMeshCreated;

	bool boOff;


	HRESULT Init();

};
//
//Point light
//
class POINT_LIGHT : public THING_BASE
{
public:
	DWORD m_dwIndex;

	POINT_LIGHT();
	~POINT_LIGHT();		

	HRESULT Draw();
	HRESULT TrueDraw();
	HRESULT Illuminate();
	HRESULT SetLuminance(float lumi);
	HRESULT SetColor(BYTE r,BYTE g,BYTE b);
	void ShowLightObject(bool toggle);
	HRESULT ResetPosition();
	HRESULT SetPosition(float x,float y,float z);

	bool boShowGeometry;

	bool boMeshCreated;
	bool boOff;
	
	HRESULT Init();

};
//
//Spot light
//
class SPOT_LIGHT : public THING_BASE
{
public:
	DWORD m_dwIndex;

	SPOT_LIGHT();
	~SPOT_LIGHT();	

	HRESULT Draw();
	HRESULT TrueDraw();
	HRESULT Illuminate();
	HRESULT SetLuminance(float lumi);
	HRESULT SetColor(BYTE r,BYTE g,BYTE b);
	HRESULT ResetAngle();
	HRESULT ResetPosition();
	HRESULT SetPosition(float x,float y,float z);
	HRESULT SetAngle(float yaw,float pitch,float roll);

	void ShowLightObject(bool toggle);
	HRESULT SetConeAngle(float outer,float inner);

	bool boShowGeometry;

	bool boMeshCreated;
	bool boOff;

	HRESULT Init();

};

//
//ボックスメッシュ
//
class BOX : public THING_BASE
{
	public:
		BOX* m_pRef;
	BOX();
	~BOX();
	HRESULT Init();
	HRESULT Load();
	HRESULT Draw();
	HRESULT TrueDraw();
};
//
//スフィアメッシュ
//
class SPHERE  : public THING_BASE
{
	public:
		SPHERE* m_pRef;
	SPHERE();
	~SPHERE();
	HRESULT Init();
	HRESULT Load();
	HRESULT Draw();
	HRESULT TrueDraw();

};
//
//シリンダー
//
class CYLINDER : public THING_BASE
{
	public:
		CYLINDER* m_pRef;
	CYLINDER();
	~CYLINDER();
	HRESULT Init();
	HRESULT Load();
	HRESULT Draw();
	HRESULT TrueDraw();
};
//
//トーラス
//
class TORUS : public THING_BASE
{
	public:
		TORUS* m_pRef;
	TORUS();
	~TORUS();
	HRESULT Init();
	HRESULT Load();
	HRESULT Draw();
	HRESULT TrueDraw();
};
//
//ティーポット
//
class TEAPOT : public THING_BASE
{
	public:
		TEAPOT* m_pRef;
	TEAPOT();
	~TEAPOT();
	HRESULT Init();
	HRESULT Load();
	HRESULT Draw();
	HRESULT TrueDraw();

};
//
//フォントメッシュ
//
class FONTMESH : public THING_BASE
{
	public:
		FONTMESH* m_pRef;
	FONTMESH();
	~FONTMESH();
	HRESULT Init();
	HRESULT Load(WCHAR* szText);
	HRESULT Draw();
	HRESULT TrueDraw();

protected:	
	WCHAR m_szText[MAX_PATH+1];

};

//
//xファイルメッシュ
//
class XMESH : public THING_BASE
{
	public:
	
	float m_fAnimSpeed;
	float m_fAnimPosition;
	D3DMATERIAL9 MeshMaterial;
	CHAR m_szFileName[MAX_PATH+1];
	THING thing;
	XMESH* m_pRef;
	LPD3DXANIMATIONCONTROLLER m_pCloneAnimController;

	XMESH();
	~XMESH();
	HRESULT Init();
	HRESULT Load(CHAR* szFileName);
	HRESULT Refer(XMESH* pXMesh);
	HRESULT Draw();
	HRESULT TrueDraw(D3DXMATRIX* m);
	HRESULT SetColor(BYTE a,BYTE r,BYTE g,BYTE b);
	HRESULT SetAnimationSpeed(float fSpeed);
	HRESULT ReleaseAllTexture();
	void ReleaseAllTextureSub(LPD3DXFRAME);
	HRESULT GetAnimTime(double* time);
	HRESULT SetAnimTime(double time);
	HRESULT LoadXFile(CHAR* szFileName);
	VOID Free();
};


//
//2Dスプライト
//
class SPRITE_2D
{
public:
	float z;
	bool boEffect;
	DWORD m_dwMyIndex;
	OBJECT_MANAGER* m_pManager;
	float alpha;
	NEGAA_OBJ_TYPE m_Type;
	SPRITE_2D* m_pRef;
	SPRITE_2D();
	~SPRITE_2D();
	HRESULT Load(CHAR* szFileName,D3DXCOLOR ColorKey);
	HRESULT Draw(RECT* pRect);
	HRESULT Refer(SPRITE_2D* p);
	HRESULT TrueDraw();
	HRESULT SetColor(BYTE a,BYTE r,BYTE g,BYTE b);
	HRESULT SetPosition(float x,float y);
	HRESULT Move(float x,float y);
	HRESULT SetAngle(float roll);
	HRESULT Rotate(float roll);
	
	HRESULT ResetPosition();
	HRESULT ResetAngle();
	HRESULT ResetScale();

	D3DXVECTOR2 vPos;
	float roll;
	float scale;

	LPDIRECT3DDEVICE9 m_pDevice;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DMATERIAL9 MeshMaterial;
	CHAR* m_szFileName;	
	DWORD m_dwImageHeight;
	DWORD m_dwImageWidth;
	D3DXCOLOR m_ColorKey;
	RENDER_QUE* m_pRenderQue;

	bool boDevIsNull;	
	bool m_boLoaded;
	bool m_boLoadRequired;
	
	HRESULT Init();
	HRESULT LoadTexture(CHAR* szFileName);

	RECT m_Rect;
	bool m_boRect;
	D3DXVECTOR2 vCenter;
	float r;
};

//
//3Dスプライト
//
class SPRITE_3D
{
public:
	bool boEffect;
	DWORD m_dwMyIndex;
	OBJECT_MANAGER* m_pManager;
	float alpha;
	NEGAA_OBJ_TYPE m_Type;
	SPRITE_3D* m_pRef;
	SPRITE_3D();
	~SPRITE_3D();
	HRESULT Load(CHAR* szFileName,D3DXCOLOR ColorKey);
	HRESULT Refer(SPRITE_3D* p);
	HRESULT Draw(RECT* pRect);
	HRESULT TrueDraw();
	HRESULT SetColor(BYTE a,BYTE r,BYTE g,BYTE b);
	HRESULT SetEmissive(BYTE a,BYTE r,BYTE g,BYTE b);
	HRESULT Rotate(float yaw,float pitch,float roll);
	HRESULT Move(float x,float y,float z);
	HRESULT SetAngle(float yaw,float pitch,float roll);
	DWORD m_dwImageHeight;
	DWORD m_dwImageWidth;
	
	
	HRESULT ResetPosition();
	HRESULT ResetAngle();
	HRESULT ResetScale();

	D3DXVECTOR3 vPos;
	
	float scale;

	LPDIRECT3DDEVICE9 m_pDevice;
	LPDIRECT3DTEXTURE9 m_pTexture;

	D3DXVECTOR3 vAxisX;
	D3DXVECTOR3 vAxisY;
	D3DXVECTOR3 vAxisZ;

	D3DXMATRIX mWorld;
	D3DXVECTOR3 vDir;
	CHAR* m_szFileName;
	float yaw;
	float pitch;
	float roll;
	D3DXCOLOR m_ColorKey;
	D3DXMATRIX mOrientation;
	RENDER_QUE* m_pRenderQue;

	bool boDevIsNull;
	bool m_boLoaded;
	bool m_boLoadRequired;	
	D3DMATERIAL9 MeshMaterial;	
	HRESULT Init();
	HRESULT LoadTexture(CHAR* szFileName);

	RECT m_Rect;
	bool m_boRect;
	D3DXVECTOR3 vCenter;
	float r;
	D3DXMATRIX mParent;
};

//
//ビルボード（3Dスプライト）
//
class BILLBOARD
{
public:
	bool boEffect;
	DWORD m_dwMyIndex;
	OBJECT_MANAGER* m_pManager;
	float alpha;
	NEGAA_OBJ_TYPE m_Type;
	BILLBOARD* m_pRef;
	BILLBOARD();
	~BILLBOARD();
	HRESULT Load(CHAR* szFileName,D3DXCOLOR ColorKey);
	HRESULT Refer(BILLBOARD* p);
	HRESULT Draw(RECT* pRect);
	HRESULT TrueDraw();
	HRESULT SetColor(BYTE a,BYTE r,BYTE g,BYTE b);
	HRESULT SetEmissive(BYTE a,BYTE r,BYTE g,BYTE b);
	HRESULT Rotate(float roll);
	HRESULT Move(float x,float y,float z);
	HRESULT SetAngle(float yaw,float pitch,float roll);
	DWORD m_dwImageHeight;
	DWORD m_dwImageWidth;
	
	
	HRESULT ResetPosition();
	HRESULT ResetAngle();
	HRESULT ResetScale();

	D3DXVECTOR3 vPos;
	
	float scale;

	LPDIRECT3DDEVICE9 m_pDevice;
	LPDIRECT3DTEXTURE9 m_pTexture;

	D3DXVECTOR3 vAxisX;
	D3DXVECTOR3 vAxisY;
	D3DXVECTOR3 vAxisZ;

	D3DXMATRIX mWorld;
	D3DXVECTOR3 vDir;
	CHAR* m_szFileName;
	float yaw;
	float pitch;
	float roll;
	D3DXCOLOR m_ColorKey;
	D3DXMATRIX mOrientation;
	RENDER_QUE* m_pRenderQue;

	bool boDevIsNull;
	bool m_boLoaded;
	bool m_boLoadRequired;	
	D3DMATERIAL9 MeshMaterial;	
	HRESULT Init();
	HRESULT LoadTexture(CHAR* szFileName);

	RECT m_Rect;
	bool m_boRect;
	D3DXVECTOR3 vCenter;
	float r;
	D3DXMATRIX mParent;
};
//
//
//
class FONT
{
public:
	bool boDevIsNull;
	bool m_boLoaded;
	bool m_boLoadRequired;	
	LPDIRECT3DDEVICE9 m_pDevice;
	LPD3DXFONT m_pFont;
	int m_iLoadStyle;//=1 Load() =2 LoadEx()

	TCHAR m_szFontName[50];
	int m_iWidth;
	int m_iHeight;
	D3DCOLOR m_Color;

	D3DXFONT_DESC m_desc;

	D3DXVECTOR2 vPos;

	FONT();
	~FONT();
	HRESULT Init();
	HRESULT Load(TCHAR* FontName, int width,int height, D3DCOLOR color);//簡易属性
	HRESULT LoadEx(D3DXFONT_DESC* desc);//全ての属性
	HRESULT ReLoad();//直前に行ったロードを再度実行
	HRESULT Draw(char* string);
};
//
//
HRESULT GetRadius(LPDIRECT3DDEVICE9 pDevice,LPD3DXMESH pMesh,D3DXVECTOR3* pvCenter,float* pfRadius);

//
//NEGAAクラス
//
class NEGAA
{
public:
	bool m_boMainFocus;
	HWND m_hWnd;
#ifndef NO_DMUSIC
	int m_iMIDIIndex[MAX_MIDI];
	int m_iNumMIDI;
	bool m_MIDIGlobal[MAX_MIDI];
	LONG m_MIDIvol[MAX_MIDI];
#endif
	D3DPRESENT_PARAMETERS d3dpp;
	LPDIRECT3D9 m_pD3d;
	LPDIRECT3DDEVICE9 m_pDevice; 
	DWORD m_dwLength;
	LPD3DXFONT m_pFont;
	LPDIRECTINPUT8 m_pDinput;         
	LPDIRECTINPUTDEVICE8 m_pKeyDevice;
	LPDIRECTINPUTDEVICE8 m_pMouseDevice;
	DWORD m_dwWindowWidth;
	DWORD m_dwWindowHeight;	
	FONT* pA;
	FONT* pB;
	FONT* pC[6];
	FONT* pD;
	FONT* pE;
	FONT* pF[3];
	D3DFORMAT m_BackBufferFormat;
	
	NEGAA();
	HRESULT Init(HINSTANCE hInst,CHAR* szName,DWORD dwWidth,DWORD dwHeight);
	~NEGAA();	
	CAMERA Camera;
	DISTANT_LIGHT Light;
	CAMERA* m_pCamera;
		
	HRESULT LoopStart();
	HRESULT LoopEnd();
	HRESULT Release();	
	HRESULT LightOn(DWORD index);
	HRESULT LightOff(DWORD index);
	HRESULT AllLightOff();
	HRESULT AllLightOn();
	VOID ChangeBackgorundColor(D3DCOLOR col);
	
	HRESULT Fade(float ratio,float r,float g,float b);

	OBJECT_MANAGER m_Manager;

	float m_fFarDistance;
	float m_fNearDistance;

	//Ray
	VOID RenderRay(D3DXVECTOR3* pvStart,D3DXVECTOR3* pvDir,D3DXMATRIX* pmOrientation,float fLength);
	RAY_COLLIDE Collide(D3DXVECTOR3* pvStart,D3DXVECTOR3* pvDir,D3DXMATRIX* pmOrientation,XMESH* pXMesh);
	RAY_COLLIDE CollideEx(D3DXVECTOR3* pvStart,D3DXVECTOR3* pvDir,D3DXMATRIX* pmOrientation,XMESH* pXMesh,D3DXVECTOR3* pvNormal,DWORD* pdwIndex);
	HRESULT FindVerticesOnPoly(LPD3DXMESH pMesh,DWORD dwPolyIndex,D3DXVECTOR3* pvecVertices );


	//Render Que
	RENDER_QUE m_RenderQue;
	

	//AlphaBlend
	HRESULT AlphaOn(int type);
	HRESULT AlphaOff();

	//キーボード
	bool Key(BYTE code);
	BYTE KeyCode();
	BYTE KeyCodeAscii();
	//マウス
	bool MouseLeftButton();
	bool MouseMiddleButton();
	bool MouseRightButton();
	POINT MousePosition();
	POINT MousePositionDelta();	
	//sound
	HRESULT LoadSound(DWORD* pdwIndex,LPSTR pcFileName,DWORD dwNumNotify,DWORD dwSecs,bool boGlobal);
	HRESULT LoadSound3D(DWORD* pdwIndex,LPSTR pcFileName,float fDamping,DWORD dwNumNotify,DWORD dwSecs,bool boVirtual,HRTF_LEVEL HRTFLevel,bool boGlobal);

#ifndef NO_DMUSIC
	HRESULT LoadMIDI(DWORD* pdwIndex,LPSTR pcFileName,bool boGlobal);
#endif
	HRESULT Play(DWORD dwIndex,bool );
	HRESULT Stop(DWORD dwIndex );
	void KillFocus();
	void SetFocus();
	//基本系
	HRESULT SetVolume(DWORD dwIndex,LONG lVolume);
	HRESULT SetSpeed(DWORD dwIndex,float fSpeed);
	//3D系
	HRESULT MoveEar(float x,float y,float z);//耳はアプリにひとつ
	HRESULT RotateEar(float fYaw,float fPitch,float fRoll);//各角は増分。
	HRESULT SetDopplerEffect(float fCoefficient,float fMeasure);
	HRESULT EnableDopplerEffect(bool boEnable);
	HRESULT EnableDopplerEffect2(bool boEnable);
	HRESULT MoveSound(DWORD dwIndex,float x,float y,float z);
	HRESULT SetDamping(DWORD dwIndex,float fDamping);
	//エフェクター系
	HRESULT Add_Effector(DWORD dwIndex,INT* piHandle,INT* piDup,EFFECTOR Effector);
	HRESULT Sub_Effector(DWORD dwIndex,INT iHandle);
	HRESULT Tune_Effector(DWORD dwIndex,INT iHandle,EFFECTOR Effector,VOID* pInit);

	HRESULT Add_Preset(DWORD dwIndex,INT* piHandle,EFFECT_PRESET Preset);
	HRESULT Sub_Preset(DWORD dwIndex,INT iHandle,EFFECT_PRESET Preset);

	//コピー、スワップ
	HRESULT SwapBox(BOX* a,BOX* b);
	HRESULT SwapSphere(SPHERE* a,SPHERE* b);
	HRESULT SwapCylinder(CYLINDER* a,CYLINDER* b);
	HRESULT SwapTorus(TORUS* a,TORUS* b);
	HRESULT SwapTeapot(TEAPOT* a,TEAPOT* b);
	HRESULT SwapFontmesh(FONTMESH* a,FONTMESH* b);
	HRESULT SwapXmesh(XMESH* a,XMESH* b);
	HRESULT SwapBillboard(BILLBOARD* a,BILLBOARD* b);	

	HRESULT SwapSprite2D(SPRITE_2D* a,SPRITE_2D* b);
	HRESULT SwapSprite3D(SPRITE_3D* a,SPRITE_3D* b);
	//カメラ系
	HRESULT SetCamera(CAMERA* pCamera);
	//リソース系
	HRESULT ReLoadAllOBject();
	HRESULT ReleaseVideoResource();
	//フルスクリーン
	HRESULT FullScreen(bool);

	//フィル　シェーディング
	HRESULT FillMode(FILL_MODE);
	HRESULT ShadeMode(SHADE_MODE);
	//フォント系
	//VOID RenderString(LPSTR szStr,INT iX,INT iY);

	//fps
	void ShowFPS(int x,int y);

private:	
	BOX m_boxForSwap;
	SPHERE m_sphereForSwap;
	CYLINDER m_cyForSwap;
	TORUS m_torusForSwap;
	TEAPOT m_tea2ForSwap;
	FONTMESH m_fontForSwap;
	XMESH m_xmeshForSwap;
	BILLBOARD m_billForSwap;	
	SPRITE_2D m_sp2ForSwap;
	SPRITE_3D m_sp3ForSwap;

	bool m_boKill;
	D3DCOLOR m_BackColor;
	BYTE m_diks[256];
	DIMOUSESTATE2 m_dims;
	

	
	
	HRESULT InitWindow(HINSTANCE hInst,CHAR* szName,DWORD dwWidth,DWORD dwHeight);
	HRESULT InitDirect3D();
	HRESULT InitSound();
	static LRESULT MsgProc(HWND,UINT,WPARAM ,LPARAM );
	HRESULT InitDinput();

};
//
//
//派生クラス
class MY_NEGAA : public NEGAA
{
public:
	void OneTime();
	void Main();
};

