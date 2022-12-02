#pragma once
#include "Scene.h"


class CStage :
	public CScene
{
private:
	CStage();

public:
	virtual ~CStage();

public:
	// CScene을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CStage* Create();

private:
	bool m_bStage0;
	bool m_bStage1;
	bool m_bStage2;
	bool m_bStage3;
	bool m_bStage4;
	bool m_bStage5;
	bool m_bStage6;
	bool m_bStage7;
	bool m_bStage8;
	bool m_bStage9;
	bool m_bStage10;
	bool m_bStage11;
	bool m_bStage12;
	bool m_bStage13;
	bool m_bStage14;
	bool m_bStage15;
	bool m_bStage16;
	bool m_bStage17;
	bool m_bStage18;
	bool m_bStage19;
	bool m_bStage20;
	bool m_bStage21;
	bool m_bStage22;
	bool m_bStage23;
	bool m_bStage24;
	bool m_bStage25;
	bool m_bStage26;

	bool m_FireCard;
	bool m_BlastCard;
	bool m_KrystalCard;
	bool m_GaiaCard;


	bool m_bDead1;
	bool m_bDead2;
	bool m_bDead3;
	bool m_bDead4;
	bool m_bDead5;
	bool m_bDead6;
	bool m_bDead7;
	bool m_bDead8;
	bool m_bDead9;

	bool m_Prison1;
	bool m_Prison2;
	bool m_Prison3;
	bool m_Prison4;
	bool m_Prison5;
	bool m_Prison6;
	bool m_Prison7;


	bool m_ShopTable;
	bool m_Card;
	bool m_Potion;
	bool m_Npc;

	bool	m_bTell;
	bool m_bSkillBar;
	bool m_bHpBar;
	bool m_bMpBar;
	bool m_bPlayerBar;
	bool m_bMoney;
	bool m_bBossName;
	bool m_bBossHpBar;
};

