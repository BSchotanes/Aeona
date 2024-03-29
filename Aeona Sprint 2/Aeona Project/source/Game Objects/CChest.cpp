////////////////////////////////////////////////////////////////////////
//    File Name				:	"CChest.cpp"
//    Author Name			:	Bryan Schotanes
//    Creation Date			:	02/09/12 - 3:57 AM
//    Purpose				:	CChest
////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "CChest.h"
#include "../Messaging/CMessageSystem.h"
#include "../Game Objects/CPlayer.h"
#include "../Input Manager/CInputManager.h"
CChest::CChest(const char* szName, bool bActiveTalk, double dRange, int sndNPC,double dPositionX, double dPositionY, unsigned int uiSpeed,
	int nImageID, unsigned int uiWidth, unsigned int uiHeight, bool bActive,unsigned int uiMaxHealth, unsigned int uiAttackDamage) 
	: CNPC(szName,bActiveTalk,dRange,sndNPC,dPositionX,dPositionY,uiSpeed,nImageID,uiWidth,uiHeight,bActive,uiMaxHealth,uiAttackDamage)
{
	m_bOpened = false;
	m_sndChest = AUDIO->SFXLoadSound("resource/sound/GetItem.wav");
}
void CChest::LoadAnimations(char const * const szFilename)
{
	CNPC::LoadAnimations(szFilename);
	GetAnimationPlayer(GetCurrentAnimation())->SetLooping(false);
	GetAnimationPlayer(GetCurrentAnimation())->Pause();
}
void CChest::Render(void)
{
	if(GetAnimationPlayer(GetCurrentAnimation())->GetFrameNumber() == 3 && m_bOpened == false )
	{
		m_bOpened = true;
		CPlayer::GetInstance()->AcquirePotion();
		GetAnimationPlayer(GetCurrentAnimation())->Pause();
	}
	CNPC::Render();
}
void CChest::Input(void)
{
	CBaseCharacter::Input();

	if(CInputManager::GetInstance()->GetPressedA() && m_bOpened == false)
	{
		double distance = CPlayer::GetInstance()->GetPosition().GetDistanceUntil(GetPosition());

		if(distance < 75)
			OpenChest();
	}

}
CChest::~CChest()
{
	if(AUDIO->SFXIsSoundPlaying(m_sndChest))
	{
		AUDIO->SFXStopSound(m_sndChest);
		AUDIO->SFXUnloadSound(m_sndChest);
	}
}
void CChest::OpenChest(void)
{
	if(!AUDIO->SFXIsSoundPlaying(m_sndChest))
		AUDIO->SFXPlaySound(m_sndChest);

	GetAnimationPlayer(GetCurrentAnimation())->Play();

	//TEMPORARY EFFECT TODO
	CPlayer::GetInstance()->AcquirePotion();
}