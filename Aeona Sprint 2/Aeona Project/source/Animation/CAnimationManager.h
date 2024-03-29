////////////////////////////////////////////////////////////////////////
//    File Name				:	"CAnimationManager.h"
//
//    Author Name			:	Bryan Schotanes
//    Creation Date			:	02/08/12 - 7:47 PM
//    Purpose				:	Contains the Animation Manager
////////////////////////////////////////////////////////////////////////
#ifndef _CANIMATIONMANAGER_H_
#define _CANIMATIONMANAGER_H_

#include "CAnimation.h"
#include "..\tinyxml\tinyxml.h"
class CAnimationManager
{
private:
	vector<CAnimation*> m_Animations;
public:
	bool UnloadAnimation(int nAnimation);
	int LoadAnimation(const char* szFilename, int* pnFirstAnmID = NULL, int* pnLastAnmID = NULL);
	bool UnloadAll();
	static CAnimationManager* GetInstance();

	CAnimation* GetAnimation(int animationNumber) {return m_Animations[animationNumber] ;}

};
#endif