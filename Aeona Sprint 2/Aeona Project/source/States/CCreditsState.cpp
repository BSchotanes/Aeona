////////////////////////////////////////////////////////////////////////
//    File Name				:	"CCreditsState.cpp"
//    Author Name			:	Bryan Schotanes
//    Creation Date			:	02/08/12 - 7:47 PM
//    Purpose				:	Contains the CreditsState
////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "../StdAfx.h"
#include "CCreditsState.h"
#include "CMainMenuState.h"

CCreditsState* CCreditsState::m_pInstance = NULL;
CCreditsState::CCreditsState()
{
	m_fWhatever = 0.0f;
}
CCreditsState::~CCreditsState(){}
CCreditsState* CCreditsState::GetInstance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new CCreditsState;
	}
	return m_pInstance;
}
void CCreditsState::DeleteInstance()
{
	if(m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}
void CCreditsState::Enter()
{
	pFont = new CBitmapFont;
	m_fWhatever = 0.0f;
}
bool CCreditsState::Input()
{
	if(INPUT->KeyPressed(DIK_RETURN))
	{
		GAME->ChangeState(CMainMenuState::GetInstance());
	}
	return true;
}
void CCreditsState::Update(float fElapsedTime)
{
	m_fWhatever -= (int)(fElapsedTime * 60);
	if(m_fWhatever <= -1200.0f)
	{
		GAME->ChangeState(CMainMenuState::GetInstance());
	}
}
void CCreditsState::Render()
{
	pFont->Write("PROGRAMMING BY",		10, (int)(480 + m_fWhatever), D3DCOLOR_XRGB(200, 100, 255));
	pFont->Write("Ari Bodaghee",		10, (int)(510 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
	pFont->Write("Daniel Lima",			10, (int)(540 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
	pFont->Write("Bryan Schotanes",		10, (int)(570 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
	pFont->Write("Phil Sullivan",		10, (int)(600 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
		
	pFont->Write("ART BY",				10, (int)(650 + m_fWhatever), D3DCOLOR_XRGB(200, 100, 255));
	pFont->Write("Chris Jahosky",		10, (int)(680 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
	pFont->Write("Maher Sagrilo",		10, (int)(710 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
	pFont->Write("Daniel Lima",			10, (int)(740 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
	pFont->Write("Phil Sullivan",		10, (int)(770 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
	
	pFont->Write("MUSIC BY",			10, (int)(820 + m_fWhatever), D3DCOLOR_XRGB(200, 100, 255));
	pFont->Write("Aaron Cook",			10, (int)(850 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
	pFont->Write("Kim Stennabb Caesar",	10, (int)(880 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
	pFont->Write("Heather Plunkard",	10, (int)(910 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));

	pFont->Write("TILESET GRAPHICS BY",	10, (int)(960 + m_fWhatever), D3DCOLOR_XRGB(200, 100, 255));
	pFont->Write("Enterbrain (www.enterbrain.co.jp)",			10, (int)(990 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
	pFont->Write("ReinoRPG Community (www.reinorpg.com)",		10, (int)(1020 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
	pFont->Write("First Seed Material (www.tekepon.net/fsm/)",	10, (int)(1050 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
	pFont->Write("Mundo RPG Maker (www.mundorpgmaker.com)",		10, (int)(1080 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));

	pFont->Write("ASSOCIATE PRODUCERS", 10, (int)(1130 + m_fWhatever), D3DCOLOR_XRGB(200, 100, 255));
	pFont->Write("Rob Martinez",		10, (int)(1160 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
	pFont->Write("Shawn Paris",			10, (int)(1190 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
	pFont->Write("Sean Hathaway",		10, (int)(1220 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
	
	pFont->Write("PRODUCER",			10, (int)(1270 + m_fWhatever), D3DCOLOR_XRGB(200, 100, 255));
	pFont->Write("John OLeske",			10, (int)(1300 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
	
	pFont->Write("SPECIAL THANKS",		10, (int)(1350 + m_fWhatever), D3DCOLOR_XRGB(200, 100, 255));
	pFont->Write("Lari Norri",			10, (int)(1380 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
	pFont->Write("David Brown",			10, (int)(1410 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));
	pFont->Write("The RPG Maker Fanbase",10, (int)(1440 + m_fWhatever), D3DCOLOR_XRGB(255, 255, 255));

	pFont->Write("Press Enter",			600, 550, D3DCOLOR_XRGB(255, 0, 0));
}
void CCreditsState::Exit()
{
	//	Delete font
	if(pFont)
	{
		delete pFont;
		pFont = NULL;
	}
	m_fWhatever = 0.0f;
}