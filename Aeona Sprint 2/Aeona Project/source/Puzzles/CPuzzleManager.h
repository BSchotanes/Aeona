////////////////////////////////////////////////////////////////////////
//    File Name				:	"CPuzzleManager.h"
//
//    Author Name			:	Daniel Lima
//    Creation Date			:	02/09 - 12:43 AM
//    Purpose				:	Holds all game puzzles
//
//    Last Updater Name		:	Name of last person who updated it
//    Last Update Date		:	month/day - hour:min (AM/PM)
//    Last Update Notes		:	Notes from the last person who updated it
//
//    Other Notes			:   Notes from other updates are moved here
//								when the file is updated again
////////////////////////////////////////////////////////////////////////

// Standard header protection
#ifndef CPUZZLEMANAGER_H_
#define CPUZZLEMANAGER_H_

#include "CPuzzle.h"

class CPuzzleManager
{
	vector<IBasePuzzle*> m_vpPuzzles;

public:
	void InitPuzzleManager(void);
	static CPuzzleManager* GetInstance(void);
	void UpdatePuzzles(float fElapsedTime);
	void RenderPuzzles(void);
	void ShutdownPuzzleManager(void);
};

#endif // CPUZZLEMANAGER_H_