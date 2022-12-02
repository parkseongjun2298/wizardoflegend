#include "stdafx.h"
#include "Stage.h"
#include "Terrain.h"
#include "Player.h"
#include "Mouse.h"
#include "ScrollMgr.h"
#include"TimeMgr.h"
#include "NormalEffect.h"
#include "BuffEffect.h"
#include"GaiaEffect.h"
#include "AnimateIMP.h"
#include "NoAnimateIMP.h"
#include"PlayerAtt.h"
#include"FireBall.h"
#include"ObjectMgr.h"

//Monster
#include"SwordMan.h"
#include"Wizard.h"
#include"Archer.h"
#include"PlayerSummon.h"
#include"ArcherSummon.h"
#include"WarriorSummon.h"
#include"WizardSummon.h"
#include"SummonAni.h"
#include"CollisionMgr.h"

//UI
#include"Teleport.h"
#include"SkillBar.h"
#include"PlayerHp.h"
#include"PlayerMp.h"
#include"PlayerBar.h"
#include"Money.h"
#include"FireCard.h"
#include"BlastCard.h"
#include"KrystalCard.h"


//Object
#include"PrisonHor.h"
#include"PrisonVer.h"
//shop
#include"Potion.h"
#include"GaiaCard.h"
#include"ShopTable.h"
#include"Npc.h"
//boss
#include"Boss.h"
#include"BossName.h"
#include"BossHp.h"
CStage::CStage()
{
}


CStage::~CStage()
{
	Release();
}

int CStage::Update()
{
	m_pObjectMgr->Update();
	if(m_bTell)
	{ 
		CSoundMgr::GetInstance()->PlaySound(L"teleport.mp3", CSoundMgr::UI);
		CTeleport* pTel = CTeleport::Create(2600.f,1920.f);
		m_pObjectMgr->AddObject(TELPO, pTel);
		m_bTell = false;
	}
	if (m_bSkillBar)
	{

		CSkillBar* pTel = CSkillBar::Create();
		m_pObjectMgr->AddObject(UI, pTel);
		m_bSkillBar = false;
	}
	if (m_bHpBar)
	{

		CPlayerHp* pTel = CPlayerHp::Create();
		m_pObjectMgr->AddObject(PLAYERHP, pTel);
		m_bHpBar = false;
	}
	if (m_bMpBar)
	{

		CPlayerMp* pTel = CPlayerMp::Create();
		m_pObjectMgr->AddObject(PLAYERMP, pTel);
		m_bMpBar = false;
	}
	if (m_bPlayerBar)
	{

		CPlayerBar* pTel = CPlayerBar::Create();
		m_pObjectMgr->AddObject(UI, pTel);
		m_bPlayerBar = false;
	}

	if (m_bMoney)
	{

		CMoney* pTel = CMoney::Create();
		m_pObjectMgr->AddObject(UI, pTel);
		m_bMoney = false;
	}
	//cout << "x:" << CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x << endl;
	//cout << "Y:" << CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y << endl;

	

	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x == 2600 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y <= 2000 && m_bStage0 == true)
	{ 
	CEffectIMP* pPlay = CSummonAni::Create(L"Player", L"Summon", 0.f, 8.f);
	NULL_CHECK_RETURN(pPlay, NO_EVENT);

	CEffect* pEffect = CPlayerSummon::Create(pPlay, 0.f, 1.f);
	m_pObjectMgr->AddObject(PLAYEREFFECT, pEffect);
	pPlay = CNoAnimateIMP::Create(L"Player", L"Summon", 1.f);
	m_bStage0 = false;

	}
	

	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 1400 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y <= 2200 && m_bStage1 == true)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI); 

		CSwordMan* pMon = CSwordMan::Create(1150,1750);
		m_pObjectMgr->AddObject(MONSTER, pMon);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"WarriorSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CWarriorSummon::Create(pMon, pImp, 0.f, 1.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"WarriorSummon", 1.f);

		//감옥생성
		CSoundMgr::GetInstance()->PlaySound(L"PRISON_START.mp3", CSoundMgr::UI);
		CPrisonVer* PPrison = CPrisonVer::Create(1730, 1960);
		m_pObjectMgr->AddObject(OBJECT, PPrison);


		m_bStage1 = false;

		

		//if줘서 m_bDead같은거 하나 해서 죽으면 false 해서 false 일때 아래 소환 ㄱ
	}

	

	if(!m_bStage1 && m_pObjectMgr->m_ObjectList[MONSTER].empty())
	{ 
		
		m_bDead1 = true;
	}

	if (!m_FireCard&&m_bDead1)
	{
		CFireCard* pFireCard = CFireCard::Create(1400, 1920);
		m_pObjectMgr->AddObject(CARD, pFireCard);
		//CObjectMgr::GetInstance()->GetPlayer()->GetInfo().iMoney += 11;
		m_FireCard = true;
	}

	if (m_bDead1&& m_bStage2)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CWizard* pMon2 = CWizard::Create(1200, 1920);
		m_pObjectMgr->AddObject(MONSTER, pMon2);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"WizardSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CWizardSummon::Create(pMon2, pImp, 0.f, 1.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"WizardSummon", 1.f);

		m_bStage2 = false;
	}

	if (!m_bStage2 && m_pObjectMgr->m_ObjectList[MONSTER].empty())
		m_bDead2 = true;

	if (!m_BlastCard&&m_bDead2)
	{
		CBlastCard* pBlastCard = CBlastCard::Create(1500, 1920);
		m_pObjectMgr->AddObject(CARD, pBlastCard);
		//CObjectMgr::GetInstance()->GetPlayer()->GetInfo().iMoney += 14;
		m_BlastCard = true;
	}


	if (m_bDead2&& m_bStage3)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CArcher* pMon3 = CArcher::Create(1200, 1920);
		m_pObjectMgr->AddObject(MONSTER, pMon3);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"ArcherSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CArcherSummon::Create(pMon3, pImp, 0.f, 2.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"ArcherSummon", 1.f);



		m_bStage3 = false;


	}
	if (!m_bStage3 && m_pObjectMgr->m_ObjectList[MONSTER].empty())
		m_bDead3 = true;
	if (!m_Prison1&&m_bDead3)
	{
		//감옥지우기
		CSoundMgr::GetInstance()->PlaySound(L"PRISON_END.mp3", CSoundMgr::UI); 
		m_pObjectMgr->m_ObjectList[OBJECT].clear(); 

		m_Prison1 = true;
	}
	if (!m_KrystalCard&&m_bDead3)
	{
		CKrystalCard* pBlastCard = CKrystalCard::Create(1300, 1920);
		m_pObjectMgr->AddObject(CARD, pBlastCard);

		m_KrystalCard = true;
	}

	//==================================================================다음스테이지=========================================


	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x >= 3900 &&
		CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y <= 2100
		&& CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y >= 1900
		&& m_bStage5 == true)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CSwordMan* pMon = CSwordMan::Create(4200, 2050);
		m_pObjectMgr->AddObject(MONSTER, pMon);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"WarriorSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CWarriorSummon::Create(pMon, pImp, 0.f, 1.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"WarriorSummon", 1.f);

		CPrisonVer* PPrison = CPrisonVer::Create(3620, 2000);
		m_pObjectMgr->AddObject(OBJECT, PPrison);
		CSoundMgr::GetInstance()->PlaySound(L"PRISON_START.mp3", CSoundMgr::UI);
		CPrisonVer* PPrison2 = CPrisonVer::Create(4229, 1840);
		m_pObjectMgr->AddObject(OBJECT, PPrison2);

		CPrisonHor* PPrison3 = CPrisonHor::Create(3980, 1430);
		m_pObjectMgr->AddObject(OBJECT, PPrison3);

		CPrisonHor* PPrison4 = CPrisonHor::Create(3750, 2430);
		m_pObjectMgr->AddObject(OBJECT, PPrison4);

		m_bStage5 = false;
	}

	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x >= 3900
		&& CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y <= 2100
		&& CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y >= 1900
		&& m_bStage6 == true)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CSwordMan* pMon = CSwordMan::Create(3680, 1680);
		m_pObjectMgr->AddObject(MONSTER, pMon);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"WarriorSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CWarriorSummon::Create(pMon, pImp, 0.f, 1.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"WarriorSummon", 1.f);



		m_bStage6 = false;
	}
	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x >= 3900
		&& CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y <= 2100 
		&& CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y >= 1900
		&& m_bStage7 == true)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CArcher* pMon3 = CArcher::Create(4050, 2050);
		m_pObjectMgr->AddObject(MONSTER, pMon3);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"ArcherSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CArcherSummon::Create(pMon3, pImp, 0.f, 2.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"ArcherSummon", 1.f);



		m_bStage7 = false;
	}
	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x >= 3900
		&& CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y <= 2100 
		&& CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y >= 1900
		&& m_bStage8 == true)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CArcher* pMon3 = CArcher::Create(3680, 1640);
		m_pObjectMgr->AddObject(MONSTER, pMon3);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"ArcherSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CArcherSummon::Create(pMon3, pImp, 0.f, 2.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"ArcherSummon", 1.f);



		m_bStage8 = false;
	}
	if (!m_bStage8 && m_pObjectMgr->m_ObjectList[MONSTER].empty())
		m_bDead4 = true;
	if (!m_Prison2&&m_bDead4)
	{
		//감옥지우기
		CSoundMgr::GetInstance()->PlaySound(L"PRISON_END.mp3", CSoundMgr::UI);
		m_pObjectMgr->m_ObjectList[OBJECT].clear();

		m_Prison2 = true;
	}

	
	//궁수6명
	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 3700&& CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y <= 1075 && m_bStage9 == true)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CArcher* pMon3 = CArcher::Create(3600, 1125);
		m_pObjectMgr->AddObject(MONSTER, pMon3);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"ArcherSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CArcherSummon::Create(pMon3, pImp, 0.f, 2.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"ArcherSummon", 1.f);

		CSoundMgr::GetInstance()->PlaySound(L"PRISON_START.mp3", CSoundMgr::UI);
		CPrisonVer* PPrison = CPrisonVer::Create(3364, 1038);
		m_pObjectMgr->AddObject(OBJECT, PPrison);

		CPrisonHor* PPrison3 = CPrisonHor::Create(3980, 1430);
		m_pObjectMgr->AddObject(OBJECT, PPrison3);

		m_bStage9 = false;
	}
	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 3700 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y <= 1075 && m_bStage10 == true)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CArcher* pMon3 = CArcher::Create(3800, 1125);
		m_pObjectMgr->AddObject(MONSTER, pMon3);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"ArcherSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CArcherSummon::Create(pMon3, pImp, 0.f, 2.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"ArcherSummon", 1.f);



		m_bStage10 = false;
	}
	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 3700 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y <= 1075 && m_bStage11 == true)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CArcher* pMon3 = CArcher::Create(3600, 925);
		m_pObjectMgr->AddObject(MONSTER, pMon3);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"ArcherSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CArcherSummon::Create(pMon3, pImp, 0.f, 2.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"ArcherSummon", 1.f);



		m_bStage11 = false;
	}
	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 3700 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y <= 1075 && m_bStage12 == true)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CArcher* pMon3 = CArcher::Create(3800, 925);
		m_pObjectMgr->AddObject(MONSTER, pMon3);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"ArcherSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CArcherSummon::Create(pMon3, pImp, 0.f, 2.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"ArcherSummon", 1.f);



		m_bStage12 = false;
	}
	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 3700 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y <= 1075 && m_bStage13 == true)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CArcher* pMon3 = CArcher::Create(3600, 1025);
		m_pObjectMgr->AddObject(MONSTER, pMon3);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"ArcherSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CArcherSummon::Create(pMon3, pImp, 0.f, 2.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"ArcherSummon", 1.f);



		m_bStage13 = false;
	}
	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 3700 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y <= 1075 && m_bStage14 == true)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CArcher* pMon3 = CArcher::Create(3800, 1025);
		m_pObjectMgr->AddObject(MONSTER, pMon3);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"ArcherSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CArcherSummon::Create(pMon3, pImp, 0.f, 2.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"ArcherSummon", 1.f);



		m_bStage14 = false;
	}

	if (!m_bStage14 && m_pObjectMgr->m_ObjectList[MONSTER].empty())
		m_bDead5 = true;
	if (!m_Prison3&&m_bDead5)
	{
		//감옥지우기
		CSoundMgr::GetInstance()->PlaySound(L"PRISON_END.mp3", CSoundMgr::UI);
		m_pObjectMgr->m_ObjectList[OBJECT].clear();

		m_Prison3 = true;
	}
	//======================================================================================

	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 3100 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y >= 950 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y <= 1150 && m_bStage15)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CWizard* pMon2 = CWizard::Create(2900, 1050);
		m_pObjectMgr->AddObject(MONSTER, pMon2);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"WizardSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CWizardSummon::Create(pMon2, pImp, 0.f, 1.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"WizardSummon", 1.f);

		CSoundMgr::GetInstance()->PlaySound(L"PRISON_START.mp3", CSoundMgr::UI);
		CPrisonVer* PPrison = CPrisonVer::Create(3364, 1038);
		m_pObjectMgr->AddObject(OBJECT, PPrison);

		m_bStage15= false;
	}
	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 3100 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y >= 950 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y <= 1150 && m_bStage16)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CWizard* pMon2 = CWizard::Create(3300, 1050);
		m_pObjectMgr->AddObject(MONSTER, pMon2);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"WizardSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CWizardSummon::Create(pMon2, pImp, 0.f, 1.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"WizardSummon", 1.f);

		m_bStage16 = false;
	}
	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 3100 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y >= 950 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y <= 1150 && m_bStage17)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CWizard* pMon2 = CWizard::Create(2900, 850);
		m_pObjectMgr->AddObject(MONSTER, pMon2);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"WizardSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CWizardSummon::Create(pMon2, pImp, 0.f, 1.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"WizardSummon", 1.f);

		m_bStage17 = false;
	}

	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 3100 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y >= 950 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y <= 1150 && m_bStage18)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CWizard* pMon2 = CWizard::Create(3300, 850);
		m_pObjectMgr->AddObject(MONSTER, pMon2);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"WizardSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CWizardSummon::Create(pMon2, pImp, 0.f, 1.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"WizardSummon", 1.f);

		m_bStage18 = false;
	}

	if (!m_bStage18 && m_pObjectMgr->m_ObjectList[MONSTER].empty())
		m_bDead6 = true;
	if (!m_Prison4&&m_bDead6)
	{
		//감옥지우기
		CSoundMgr::GetInstance()->PlaySound(L"PRISON_END.mp3", CSoundMgr::UI);
		m_pObjectMgr->m_ObjectList[OBJECT].clear();

		m_Prison4 = true;
	}
	//=============================================================================================================================


	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x >= 3600 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y >= 3000  && m_bStage19)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CWizard* pMon2 = CWizard::Create(3428, 3006);
		m_pObjectMgr->AddObject(MONSTER, pMon2);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"WizardSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CWizardSummon::Create(pMon2, pImp, 0.f, 1.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"WizardSummon", 1.f);

		CSoundMgr::GetInstance()->PlaySound(L"PRISON_START.mp3", CSoundMgr::UI);
		CPrisonHor* PPrison = CPrisonHor::Create(3740, 2811);
		m_pObjectMgr->AddObject(OBJECT, PPrison);
		CPrisonVer* PPrison2 = CPrisonVer::Create(2018, 3156);
		m_pObjectMgr->AddObject(OBJECT, PPrison2);
		m_bStage19 = false;
	}
	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 3600 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y >= 3000  && m_bStage20)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CArcher* pMon3 = CArcher::Create(3463, 3406);
		m_pObjectMgr->AddObject(MONSTER, pMon3);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"ArcherSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CArcherSummon::Create(pMon3, pImp, 0.f, 2.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"ArcherSummon", 1.f);

		
		m_bStage20 = false;
	}
	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 3600 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y >= 3000  && m_bStage21)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CSwordMan* pMon = CSwordMan::Create(3148, 3256);
		m_pObjectMgr->AddObject(MONSTER, pMon);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"WarriorSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CWarriorSummon::Create(pMon, pImp, 0.f, 1.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"WarriorSummon", 1.f);

		
		m_bStage21 = false;
	}
	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 3600 && CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y >= 3000 &&  m_bStage22)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CWizard* pMon2 = CWizard::Create(2888, 3426);
		m_pObjectMgr->AddObject(MONSTER, pMon2);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"WizardSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CWizardSummon::Create(pMon2, pImp, 0.f, 1.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"WizardSummon", 1.f);

	
		m_bStage22 = false;
	}


	if (!m_bStage19 && m_pObjectMgr->m_ObjectList[MONSTER].empty())
		m_bDead7 = true;


	if (!m_Prison5&&m_bDead7)
	{
		//감옥지우기
		CSoundMgr::GetInstance()->PlaySound(L"PRISON_END.mp3", CSoundMgr::UI);
		m_pObjectMgr->m_ObjectList[OBJECT].clear();

		m_Prison5 = true;
	}

	/////////////////////////////////////////////////////////////////////////
	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x >= 2200 
		&& CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 2700 
		&& CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y >= 2400 
		&& m_bStage23)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CSwordMan* pMon = CSwordMan::Create(2220, 2500);
		m_pObjectMgr->AddObject(MONSTER, pMon);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"WarriorSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CWarriorSummon::Create(pMon, pImp, 0.f, 1.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"WarriorSummon", 1.f);
		CSoundMgr::GetInstance()->PlaySound(L"PRISON_START.mp3", CSoundMgr::UI);

		CPrisonHor* PPrison = CPrisonHor::Create(2530, 2235);
		m_pObjectMgr->AddObject(OBJECT, PPrison);
		CPrisonHor* PPrison2 = CPrisonHor::Create(2130, 2250);
		m_pObjectMgr->AddObject(OBJECT, PPrison2);
		CPrisonVer* PPrison3 = CPrisonVer::Create(2940, 2390);
		m_pObjectMgr->AddObject(OBJECT, PPrison3);

		m_bStage23 = false;
	}
	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x >= 2200
		&& CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 2700
		&& CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y >= 2400
		&& m_bStage24)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CSwordMan* pMon = CSwordMan::Create(2100, 2745);
		m_pObjectMgr->AddObject(MONSTER, pMon);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"WarriorSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CWarriorSummon::Create(pMon, pImp, 0.f, 1.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"WarriorSummon", 1.f);
		
		m_bStage24 = false;

	}
	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x >= 2200
		&& CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 2700
		&& CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y >= 2400
		&& m_bStage25)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CSwordMan* pMon = CSwordMan::Create(2655, 2775);
		m_pObjectMgr->AddObject(MONSTER, pMon);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"WarriorSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CWarriorSummon::Create(pMon, pImp, 0.f, 1.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"WarriorSummon", 1.f);
		m_bStage25 = false;
		
	}
	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x >= 2200
		&& CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 2700
		&& CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y >= 2400
		&& m_bStage26)
	{
		CSoundMgr::GetInstance()->PlaySound(L"CARD_SUMMON.mp3", CSoundMgr::UI);
		CSwordMan* pMon = CSwordMan::Create(2800, 2430);
		m_pObjectMgr->AddObject(MONSTER, pMon);

		CEffectIMP* pImp = CSummonAni::Create(L"Effect", L"WarriorSummon", 0.f, 32.f);
		NULL_CHECK_RETURN(pImp, NO_EVENT);

		CEffect* pEffect = CWarriorSummon::Create(pMon, pImp, 0.f, 1.f);
		m_pObjectMgr->AddObject(MONSTEREFFECT, pEffect);
		pImp = CNoAnimateIMP::Create(L"Effect", L"WarriorSummon", 1.f);
		m_bStage26 = false;
	}

	if (!m_bStage23 && m_pObjectMgr->m_ObjectList[MONSTER].empty())
		m_bDead9 = true;


	if (!m_Prison7 &&m_bDead9)
	{
		//감옥지우기
		CSoundMgr::GetInstance()->PlaySound(L"PRISON_END.mp3", CSoundMgr::UI);
		m_pObjectMgr->m_ObjectList[OBJECT].clear();

		m_Prison7 = true;
	}












	//상점
	if(m_ShopTable)
	{ 
		CShopTable* pMon3 = CShopTable::Create(4525, 652);
		m_pObjectMgr->AddObject(UI, pMon3);

		m_ShopTable = false;
	}
	if (m_Potion)
	{
		CPotion* pMon3 = CPotion::Create(4575, 632);
		m_pObjectMgr->AddObject(SHOP, pMon3);

		m_Potion = false;
	}
	if (m_Card)
	{
		CGaiaCard* pMon3 = CGaiaCard::Create(4475, 632);
		m_pObjectMgr->AddObject(SHOP, pMon3);

		m_Card = false;
	}

	if (m_Npc)
	{
		CNpc* pMon3 = CNpc::Create(4525, 570);
		m_pObjectMgr->AddObject(UI, pMon3);

		m_Npc = false;
	}







	////보스
	if (CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.x <= 1600
		&&CObjectMgr::GetInstance()->GetPlayer()->GetInfo().vPos.y >= 3050 
		&& m_bStage4 == true)
	{
		CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
		CSoundMgr::GetInstance()->PlayBGM(L"BOSS_BGM.mp3");
		CSoundMgr::GetInstance()->SetVolume(CSoundMgr::BGM, 0.4f);


		CBoss* pMon = CBoss::Create(1200,2950);
		m_pObjectMgr->AddObject(BOSS, pMon);

		
		CPrisonVer* PPrison2 = CPrisonVer::Create(1750, 3156);
		m_pObjectMgr->AddObject(OBJECT, PPrison2);


		if (m_bBossName)
		{

			CBossName* pTel = CBossName::Create();
			m_pObjectMgr->AddObject(BOSSHP, pTel);
			m_bBossName = false;
		}
		
			if (m_bBossHpBar)
			{

				CBossHp* pTel = CBossHp::Create();
				m_pObjectMgr->AddObject(BOSSHP, pTel);
				m_bBossHpBar = false;
			}
		m_bStage4 = false;
	}
	
	if (!m_bStage4 && m_pObjectMgr->m_ObjectList[BOSS].empty())
		m_bDead8 = true;
	if (!m_Prison6&&m_bDead8)
	{
		//감옥지우기
		CSoundMgr::GetInstance()->PlaySound(L"PRISON_END.mp3", CSoundMgr::UI);
		m_pObjectMgr->m_ObjectList[OBJECT].clear();

		m_Prison6 = true;
	}



	return NO_EVENT;
}

void CStage::LateUpdate()
{
	m_pObjectMgr->LateUpdate();
	CSoundMgr::GetInstance()->Update();
}

void CStage::Render()
{
	m_pObjectMgr->Render();
}

HRESULT CStage::Initialize()
{

	CSoundMgr::GetInstance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::GetInstance()->PlayBGM(L"DUNGEON_BGM.mp3");
	CSoundMgr::GetInstance()->SetVolume(CSoundMgr::BGM, 0.4f);

	// Terrain Object
	HRESULT hr = m_pObjectMgr->AddObject(TERRAIN, CTerrain::Create());
	FAILED_CHECK_MSG_RETURN(hr, L"Terrain Create Failed", E_FAIL);

	// Player Object
	hr = m_pObjectMgr->AddObject(PLAYER, CPlayer::Create());
	FAILED_CHECK_MSG_RETURN(hr, L"Player Create Failed", E_FAIL);



	//마우스
	hr = m_pObjectMgr->AddObject(MOUSE, CMouse::Create());
	FAILED_CHECK_MSG_RETURN(hr, L"Mouse Create Failed", E_FAIL);

	m_bStage0 = true;
	m_bStage1 = true;
	m_bStage2 = true;
	m_bStage3 = true;
	m_bStage4 = true;
	m_bStage5 = true;
	m_bStage6 = true;
	m_bStage7 = true;
	m_bStage8 = true;
	m_bStage9 = true;
	m_bStage10 = true;
	m_bStage11 = true;
	m_bStage12 = true;
	m_bStage13 = true;
	m_bStage14 = true;
	m_bStage15 = true;
	m_bStage16 = true;
	m_bStage17 = true;
	m_bStage18 = true;
	m_bStage19 = true;
	m_bStage20 = true;
	m_bStage21 = true;
	m_bStage22 = true;
	m_bStage23 = true;
	m_bStage24 = true;
	m_bStage25 = true;
	m_bStage26 = true;

	m_FireCard = false;
	m_BlastCard = false;
	m_GaiaCard = false;
	m_KrystalCard = false;
	

	m_bDead1 = false;
	m_bDead2 = false;
	m_bDead3 = false;
	m_bDead4 = false;
	m_bDead5 = false;
	m_bDead6 = false;
	m_bDead7 = false;
	m_bDead8 = false;
	m_bDead9 = false;

	m_Prison1 = false;
	m_Prison2 = false;
	m_Prison3 = false;
	m_Prison4 = false;
	m_Prison5 = false;
	m_Prison6 = false;
	m_Prison7 = false;

	m_ShopTable = true;
	m_Npc = true;
	m_Potion = true;
	m_Card = true;


	m_bTell = true;
	m_bSkillBar = true;
	m_bHpBar = true;
	m_bMpBar = true;
	m_bPlayerBar = true;
	m_bMoney = true;
	m_bBossName = true;
	m_bBossHpBar = true;
	return S_OK;
}

void CStage::Release()
{
}

CStage* CStage::Create()
{
	CStage* pInstance = new CStage;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
