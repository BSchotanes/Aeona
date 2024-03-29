////////////////////////////////////////////////////////////////////////
//    File Name				:	"CHammer.h"
//
//    Author Name			:	Bryan Schotanes
//    Creation Date			:	02/08/12 - 7:47 PM
//    Purpose				:	Contains the CHammer
//
//    Last Updater Name		:   Bryan Schotanes
//    Last Update Date      :   
//    Last Update Notes     :   
//								
//								
////////////////////////////////////////////////////////////////////////

// Standard header protection
#ifndef CHAMMER_H_
#define CHAMMER_H_

#include "IBaseWeapon.h"
class CHammer : public IBaseWeapon
{
private:
	int m_sndCharged;
public:
	CHammer();
	~CHammer(void);
	void Attack(void);
	void ChargedAttack(void){}
	void Update(float fElapsedTime);
	void Render(PointD nPosition);
	void Render(PointD nPos,DWORD WHICHCOLORYOUWANTHIMTOGLOWBRO);
	unsigned int GetWeaponType(void) const { return TYPE_WEAPON_HAMMER; }
	unsigned int GetType(void) const { return IBaseInterface::TYPE_WEAPON_HAMMER; }
	RectD GetCollisionRect(void);
	//void SetWeaponRotation(float fWeaponRotation);
};

#endif