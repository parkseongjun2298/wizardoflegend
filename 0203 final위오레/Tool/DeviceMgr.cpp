#include "stdafx.h"
#include "DeviceMgr.h"

IMPLEMENT_SINGLETON(CDeviceMgr)

CDeviceMgr::CDeviceMgr()
	: m_pSDK(nullptr), m_pGraphicDev(nullptr)
{
}


CDeviceMgr::~CDeviceMgr()
{
	Release();
}

LPDIRECT3DDEVICE9 CDeviceMgr::GetDevice() const
{
	return m_pGraphicDev;
}

LPD3DXSPRITE CDeviceMgr::GetSprite() const
{
	return m_pSprite;
}

LPD3DXFONT CDeviceMgr::GetFont() const
{
	return m_pFont;
}

HRESULT CDeviceMgr::InitDevice(DISPLAY_MODE eMode)
{
	// ��ġ �ʱ�ȭ ����.
	// 1. SDK ��ü ���� (IDirect3D9)
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	NULL_CHECK_MSG_RETURN(m_pSDK, L"Direct3DCreate9 Failed", E_FAIL);

	// 2. SDK ��ü�� ���� ���� ����� ��ġ�� ����.
	D3DCAPS9 d3dcaps; // ��ġ ����
	ZeroMemory(&d3dcaps, sizeof(D3DCAPS9));

	// HAL(Hardware Abstraction Layer, �ϵ���� �߻� ����)
	// ���� ��ġ ������ HAL���� ���� �Լ�.
	HRESULT hr = m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps);
	FAILED_CHECK_MSG_RETURN(hr, L"GetDeviceCaps Failed", E_FAIL);

	// 2-1. ������ �� ��ġ ������ ���� ���� ��ġ�� "���ؽ� ���μ���"�� �����ϴ��� Ȯ��.
	// ���ؽ� ���μ���: ���� ��ȯ + ���� ����

	DWORD vp = 0; // ��ġ �ʱ�ȭ �ɼ�
	
	// ��ġ�� ���ؽ� ���μ����� �����ϴ���?
	if (d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING; // �����Ѵٸ� ��ġ�� ���ؽ� ���μ����� ���.
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// 3. SDK ��ü�� ���� ��ġ ��ü(IDirect3DDevice9) ����.
	D3DPRESENT_PARAMETERS d3dpp; // ������ ȯ�� ����.
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth = WINCX;	// �ĸ���� ����ũ��
	d3dpp.BackBufferHeight = WINCY;	// �ĸ���� ����ũ��
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; // �ĸ���� �ȼ�����
	d3dpp.BackBufferCount = 1; // �ĸ���� ����
	
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE; // ��Ƽ ���ø�
	d3dpp.MultiSampleQuality = 0;	// ǰ��
	
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// ����ü�� ����� ����Ѵ�.
	d3dpp.hDeviceWindow = g_hWnd; // �⺻ ��� ���.
 	d3dpp.Windowed = eMode; // TRUE: â ���, FALSE: ��üȭ�� ���
	d3dpp.EnableAutoDepthStencil = TRUE; // ��ġ�� ���̹���(Z����)�� ���ٽǹ��۸� �ڵ����� �����Ѵ�.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	/* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // ��üȭ���� �� ����� �ֻ���
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	hr = m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		g_hWnd, vp, &d3dpp, &m_pGraphicDev);
	FAILED_CHECK_MSG_RETURN(hr, L"CreateDevice Failed", E_FAIL);

	hr = D3DXCreateSprite(m_pGraphicDev, &m_pSprite);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateSprite Failed", E_FAIL);

	D3DXFONT_DESC	tFontInfo;	// ��Ʈ ����
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESC));

	tFontInfo.Width = 10;
	tFontInfo.Height = 20;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"�ü�"); // ��ü

	hr = D3DXCreateFontIndirect(m_pGraphicDev, &tFontInfo, &m_pFont);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateFontIndirect Failed", E_FAIL);

	return S_OK;
}

void CDeviceMgr::Render_Begin()
{
	// ������ ����
	// 1. �ĸ���۸� ����.
	m_pGraphicDev->Clear(0, nullptr,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);

	// 2. �ĸ���ۿ� �׸��⸦ �����Ѵ�.
	m_pGraphicDev->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);	// ���� ���� ���·� �׸���.
}

void CDeviceMgr::Render_End(HWND hWnd/* = nullptr*/)
{
	// 3. �ĸ���ۿ� �׸��⸦ ��ģ��.
	m_pSprite->End();
	m_pGraphicDev->EndScene();

	// 4. �ĸ���۸� ������۷� ����Ѵ�.
	m_pGraphicDev->Present(nullptr, nullptr, hWnd, nullptr);
}

void CDeviceMgr::Release()
{
	// �� COM��ü���� AddRef�� Release ��� �Լ��� ���ϰ� �ִ�.
	// AddRef: Reference Count ����
	// Release: Reference Count ����
	// ���� COM��ü�� ���� �ִ� Reference Count�� ���� 0�� �� �Ҹ�ȴ�.
	if (m_pFont->Release())
		::MessageBox(0, L"m_pFont Release Failed", L"System Error", MB_OK);
	if(m_pSprite->Release())
		::MessageBox(0, L"m_pSprite Release Failed", L"System Error", MB_OK);
	if (m_pGraphicDev->Release())
		::MessageBox(0, L"m_pGraphicDev Release Failed", L"System Error", MB_OK);
	if(m_pSDK->Release())
		::MessageBox(0, L"m_pSDK Release Failed", L"System Error", MB_OK);
}
