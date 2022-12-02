#pragma once
class CDeviceMgr
{
	DECLARE_SINGLETON(CDeviceMgr)

private:
	CDeviceMgr();
	~CDeviceMgr();

public:
	LPDIRECT3DDEVICE9 GetDevice() const;
	LPD3DXSPRITE GetSprite() const;
	LPD3DXFONT GetFont() const;

public:
	HRESULT InitDevice(DISPLAY_MODE eMode);
	void Render_Begin();
	void Render_End(HWND hWnd = nullptr);

private:
	void Release();

private:
	//IDirect3D9 COM ��ü: ���̷�Ʈ ȯ�濡�� ����� ��ġ�� �����ϰ� ��ġ ������Ʈ�� �����Ѵ�.
	//IDirect3D9�� ������	
	LPDIRECT3D9	m_pSDK;

	// IDirect3DDevice9 COM ��ü: �׷��� ī�� ���� ���� ��ɵ�(������ ��)�� �����Ѵ�.
	//IDirect3DDevice9�� ������
	LPDIRECT3DDEVICE9	m_pGraphicDev;

	// ID3DXSprite COM ��ü: ���̷�Ʈ ȯ�濡�� 2D �ؽ�ó �������� �����Ѵ�.
	// ID3DXSprite�� ������
	LPD3DXSPRITE	m_pSprite;

	// ���̷�Ʈ ��Ʈ: 2D ��Ʈ ������ COM��ü
	LPD3DXFONT		m_pFont;

	// COM (Component Object Model)
	// ����ũ�μ���Ʈ�翡�� ����� ���α׷��� ��.
	// ���α׷��� ���� ��ǰ �����ϵ� �ϼ��س� �� �ִ�! -> ������ �ּ�ȭ! �ɷ� ����!
	// COM��ü Ÿ�Ե��� ���� �빮�� I�� ���۵�. Interface�� ����.
};

