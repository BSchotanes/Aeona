////////////////////////////////////////////////////////////////////////
//    File Name				:	"COptionsState.h"
//    Author Name			:	Bryan Schotanes
//    Creation Date			:	02/08/12 - 7:47 PM
//    Purpose				:	Contains the COptionsState
////////////////////////////////////////////////////////////////////////

#ifndef _COPTIONSSTATE_H_
#define _COPTIONSSTATE_H_
#include "CBaseMenu.h"
#include "../CGame.h"

enum {OPTN_VIDEO = 0, OPTN_AUDIO, OPTN_BACK, OPTN_MAX};

class COptionsState : public CBaseMenu
{
private:
	//	Instance
	static COptionsState* m_pInstance;

	//	Constructor & Trilogy
	COptionsState();
	COptionsState(const COptionsState&);
	COptionsState& operator=(const COptionsState&);
	~COptionsState();
	
	//	Assets

	//	Members

public:
	//	Singleton
	static COptionsState* GetInstance();
	static void DeleteInstance();

	void Enter();
	void Exit();
	bool Input();
	void Update(float fElapsedTime);
	void Render();
};

#endif