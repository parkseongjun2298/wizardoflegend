#include "stdafx.h"
#include "WizardSummon.h"
#include "EffectIMP.h"
#include"Wizard.h"

CWizardSummon::CWizardSummon()
	: m_pObjectMgr(CObjectMgr::GetInstance()), m_fAngle(0.f), m_fCount(0.f),
	m_fLifeTime(0.f), m_fTimeCount(0.f), m_bCheck(false)
{
}


CWizardSummon::~CWizardSummon()
{
	Release();
}

int CWizardSummon::Update()
{
	if (m_fLifeTime <= m_fTimeCount)
		return DEAD_OBJ;

	//CWizard* pWizard = dynamic_cast<CWizard*>(m_pObjectMgr->GetMonster());
	//NULL_CHECK_RETURN(pWizard, DEAD_OBJ);


	D3DXMATRIX matScale, matRotZ, matTrans, matRevZ;

	D3DXMatrixScaling(&matScale,
		m_tInfo.vSize.x,
		m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fAngle)); // ������ �ݴ�� ������Ű�� ������ ���ȴ�. ���⼭ +m_fAngle�ϸ� ���̾ƹ���
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x,
		m_tInfo.vPos.y, 0.f);
	D3DXMatrixRotationZ(&matRevZ, D3DXToRadian(-m_fAngle));

	m_tInfo.matWorld = matScale  * matTrans  * m_pOwner->GetInfo().matWorld;

	// �ʴ� 90�� ȸ��
	m_fAngle += m_fCount * m_pTimeMgr->GetDeltaTime();



	return m_pImp->Update();
}

void CWizardSummon::LateUpdate()
{
	m_pImp->LateUpdate();
}

void CWizardSummon::Render()
{
	m_pImp->Render(m_tInfo.matWorld);
}

HRESULT CWizardSummon::Initialize()
{
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 2.7f, 2.7f, 0.f };
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	m_fCount = -100;

	return S_OK;
}

void CWizardSummon::Release()
{
}

CWizardSummon * CWizardSummon::Create(CGameObject* pOwner, CEffectIMP * pImp, float fAngle, float Time)
{
	NULL_CHECK_RETURN(pImp, nullptr);

	CWizardSummon* pInstance = new CWizardSummon;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_fAngle = fAngle;
	pInstance->m_pImp = pImp;
	pInstance->m_fLifeTime = Time;
	pInstance->m_pOwner = pOwner;
	return pInstance;
}
