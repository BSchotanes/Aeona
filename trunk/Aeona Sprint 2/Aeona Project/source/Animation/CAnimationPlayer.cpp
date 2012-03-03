////////////////////////////////////////////////////////////////////////
//    File Name				:	"CAnimationPlayer.cpp"
//
//    Author Name			:	Bryan Schotanes
//    Creation Date			:	02/08/12 - 7:47 PM
//    Purpose				:	Contains the Animation Player
////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "CAnimationPlayer.h"
#include "../Messaging/CEventSystem.h"
void CAnimationPlayer::Play()
{
		Reset();
		m_bIsPlaying = true;
}
void CAnimationPlayer::Stop()
{
	m_bIsPlaying = false;
}
void CAnimationPlayer::Pause()
{
	if(m_bIsPlaying == true)
		m_bIsPlaying = false;
	else
		m_bIsPlaying = true;
}
void CAnimationPlayer::Reset()
{
	this->m_nFrameNumber = 0;
	this->m_fTimer = 0;
	this->m_fTimer = 0;
}
void CAnimationPlayer::Update(float fElapsedTime)
{
	if(!m_bIsPlaying)
		return;
	m_fTimer += fElapsedTime;
	m_fTimer *=	CAnimationManager::GetInstance()->GetAnimation( m_nAnimationId)->GetSpeed();
	double x = CAnimationManager::GetInstance()->GetAnimation(m_nAnimationId)->GetFrame(m_nFrameNumber)->GetDuration();
	CFrame* tempframe = CAnimationManager::GetInstance()->GetAnimation(m_nAnimationId)->GetFrame(m_nFrameNumber);
	if(((long)x*100000) < ((long)m_fTimer*100000))
	{
		m_fTimer -= CAnimationManager::GetInstance()->GetAnimation(m_nAnimationId)->GetFrame(m_nFrameNumber)->GetDuration();
		if(CAnimationManager::GetInstance()->GetAnimation(m_nAnimationId)->GetFrame(m_nFrameNumber)->GetEvent() != "NONE")
			CEventSystem::GetInstance()->SendEvent(CAnimationManager::GetInstance()->GetAnimation(m_nAnimationId)->GetFrame(m_nFrameNumber)->GetEvent(),this);
		string temp = CAnimationManager::GetInstance()->GetAnimation(m_nAnimationId)->GetFrame(m_nFrameNumber)->GetEvent();
		m_nFrameNumber++;
		if(m_nFrameNumber >= CAnimationManager::GetInstance()->GetAnimation(m_nAnimationId)->GetFrames()->size())
		{
			if(m_bIsLooping)
			{
				Reset();
			}
			else
			{
				Stop();
				m_nFrameNumber = CAnimationManager::GetInstance()->GetAnimation(m_nAnimationId)->GetFrames()->size()-1;
			}
		}
	}
}
void CAnimationPlayer::Render(int nPosX,int nPosY)
{
	if(m_bIsPlaying)
	{
		int nSheet = CAnimationManager::GetInstance()->GetAnimation(m_nAnimationId)->GetImageId();
		RECT drawRect = CAnimationManager::GetInstance()->GetAnimation(m_nAnimationId)->GetFrame(m_nFrameNumber)->GetDrawRect();
		Point anchor = CAnimationManager::GetInstance()->GetAnimation(m_nAnimationId)->GetFrame(m_nFrameNumber)->GetAnchorPoint();
		TEX_MNG->Draw(nSheet,nPosX - anchor.x, nPosY - anchor.y,1,1,&drawRect);
	}
}

RECT CAnimationPlayer::ReturnFrameRect(void)
{
	return CAnimationManager::GetInstance()->GetAnimation(m_nAnimationId)->GetFrame(m_nFrameNumber)->GetDrawRect();
}

RectD CAnimationPlayer::ReturnCollisionRect(void)
{
	return CAnimationManager::GetInstance()->GetAnimation(m_nAnimationId)->GetFrame(m_nFrameNumber)->GetCollisionRect();
}

Point CAnimationPlayer::ReturnAnchorPoint(void)
{
	return CAnimationManager::GetInstance()->GetAnimation(m_nAnimationId)->GetFrame(m_nFrameNumber)->GetAnchorPoint();
}
RECT CAnimationPlayer::ReturnWeaponCollisionRect(void)
{
	return CAnimationManager::GetInstance()->GetAnimation(m_nAnimationId)->GetFrame(m_nFrameNumber)->GetWeaponCollisionRect();
}
//Point CAnimationPlayer::ReturnWeaponPoint(void)
//{
//	return CAnimationManager::GetInstance()->GetAnimation(m_nAnimationId)->GetFrame(m_nFrameNumber)->GetWeaponAnchorPoint();
//}
//float CAnimationPlayer::ReturnWeaponAngle(void)
//{
//	return CAnimationManager::GetInstance()->GetAnimation(m_nAnimationId)->GetFrame(m_nFrameNumber)->GetWeaponAngle();
//}