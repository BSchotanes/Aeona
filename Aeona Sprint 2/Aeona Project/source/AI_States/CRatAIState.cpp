#include "StdAfx.h"
#include "../StdAfx.h"
#include "CRatAIState.h"
#include "../Game Objects/CPlayer.h"

// Singleton's instance
CRatAIState* CRatAIState::sm_pInstance = NULL;

IBaseAIState* CRatAIState::GetInstance(void)
{
	if(!sm_pInstance)
		sm_pInstance = new CRatAIState();
	return sm_pInstance;
}

void CRatAIState::DeleteInstance(void)
{
	if(sm_pInstance != NULL)
	{
		delete sm_pInstance;
		sm_pInstance = NULL;
	}
}

void CRatAIState::Enter(CBaseCharacter* pCharacter)
{
	pCharacter->SetVelY( 0 );
	pCharacter->SetVelX( 0 );
	pCharacter->SetPrevVelX( 0 );
	pCharacter->SetPrevVelY( 1 );

	pCharacter->SetMoveTimer( 0.0f );
	pCharacter->SetMiniState(2);
	pCharacter->SetBehavior(CBaseCharacter::BEHAVIOR_RAT);
	pCharacter->philEnemyColor = D3DCOLOR_XRGB(255, 255, 255);
	
	pCharacter->m_bSpecial = false;
	pCharacter->m_uiSpecialCounter = 0;
	pCharacter->SetSpeed(160);

	//	0 - GetWhacked
	//	1 - WhackedPause
	//	2 - SittingStill
	//	3 - SquirmForward
	//	4 - SquirmPause
	//	5 - Lunge

	m_sndLarvaHiss = AUDIO->SFXLoadSound("resource/sound/LarvaHiss.wav");
}

void CRatAIState::Update(CBaseCharacter* pCharacter, float fElapsedTime)
{
	switch( pCharacter->GetMiniState() )
	{
		case 0:	// 0 - Get Whacked!
		{
			if( pCharacter->GetMoveTimer() == 0.0f )	//	We JUST got whacked!
			{
				pCharacter->SetMoveTimer( pCharacter->GetMoveTimer() + fElapsedTime );	//	Increment timer.
				if(pCharacter->GetPrevVelX() < 0.0f)	//	We got whacked, so set our velocity flying backward
					pCharacter->SetVelX( 350.0f );
				else if(pCharacter->GetPrevVelX() > 0.0f)
					pCharacter->SetVelX( -350.0f );
				if(pCharacter->GetPrevVelY() < 0.0f)
					pCharacter->SetVelY( 350.0f );
				else if(pCharacter->GetPrevVelY() > 0.0f)
					pCharacter->SetVelY( -350.0f );
			}
			else if( pCharacter->GetMoveTimer() < 0.15f )	//	Throw us backward for a bit
			{
				pCharacter->SetMoveTimer( pCharacter->GetMoveTimer() + fElapsedTime );	//	Increment timer.
			}
			else
			{
				pCharacter->SetMoveTimer( 0.0f );
				pCharacter->SetMiniState( 1 );	// Go to the little pause state after being whacked.
			}
			break;
		}
		case 1:	// 1 - Pause after being whacked.
		{
			if( pCharacter->GetMoveTimer() == 0.0f )
			{
				pCharacter->SetMoveTimer( pCharacter->GetMoveTimer() + fElapsedTime );	//	Increment timer.
				pCharacter->SetVelX( 0.0f );
				pCharacter->SetVelY( 0.0f );
			}
			else if( pCharacter->GetMoveTimer() < 0.4f )	//	Owww I'm still hurting, for 0.4 sec
			{
				pCharacter->SetMoveTimer( pCharacter->GetMoveTimer() + fElapsedTime );	//	Increment timer.
			}
			else
			{
				pCharacter->SetMoveTimer( 0.0f );
				pCharacter->philEnemyColor = D3DCOLOR_XRGB(255, 255, 255);
				pCharacter->SetMiniState( 3 );	// Immediately squirm forward.
			}
			break;
		}
		case 2:	// 2 - Sitting still because the player is not near.
		{
			//	NEARBY PLAYER CHECK!!
			if( (CPlayer::GetInstance()->GetPosY() >= pCharacter->GetPosY() - 256 && CPlayer::GetInstance()->GetPosY() <= pCharacter->GetPosY() + 256) && (CPlayer::GetInstance()->GetPosX() >= pCharacter->GetPosX() - 256 && CPlayer::GetInstance()->GetPosX() <= pCharacter->GetPosX() + 256) )
			{
				pCharacter->SetMoveTimer( 0.0f );
				pCharacter->SetMiniState( 3 );	// set to crouching, ready to pounce, when we find player
				break;	// i don't want to continue 'stopped' if the player is instantly nearby.
			}
			//	NEARBY PLAYER CHECK!!
			else if( pCharacter->GetMoveTimer() == 0.0f )
			{
				pCharacter->SetMoveTimer( pCharacter->GetMoveTimer() + fElapsedTime );
				pCharacter->SetVelX( 0.0f );
				pCharacter->SetVelY( 0.0f );
			}
			else if( pCharacter->GetMoveTimer() < 2.0f )
			{
				pCharacter->SetMoveTimer( pCharacter->GetMoveTimer() + fElapsedTime );
			}
			else
			{
				pCharacter->SetMoveTimer( 0.0f );
				pCharacter->SetMiniState( 2 );
			}
			break;
		}
		case 3:	// 3 - Squirm toward the player!
		{
			if( pCharacter->GetMoveTimer() == 0.0f )
			{
				pCharacter->m_bWalkCycle = !pCharacter->m_bWalkCycle;
				pCharacter->SetMoveTimer( pCharacter->GetMoveTimer() + fElapsedTime );	//	Increment timer.
				//	HACK directly check whether the player is nearby or not, below.
				if( (CPlayer::GetInstance()->GetPosY() >= pCharacter->GetPosY() - 512 && CPlayer::GetInstance()->GetPosY() <= pCharacter->GetPosY() + 512) && (CPlayer::GetInstance()->GetPosX() >= pCharacter->GetPosX() - 512 && CPlayer::GetInstance()->GetPosX() <= pCharacter->GetPosX() + 512) )
				{
					//pCharacter->m_bWalkCycle = 0;	//	Set to the one that looks most like an attack.
					//find the player direction!
					double temp_y_dist = pCharacter->GetPosY() - CPlayer::GetInstance()->GetPosY();
					double temp_x_dist = pCharacter->GetPosX() - CPlayer::GetInstance()->GetPosX();
					if(fabs(temp_y_dist) > fabs(temp_x_dist))
					{
						//then we are going north-south
						if(CPlayer::GetInstance()->GetPosY() < pCharacter->GetPosY())
						{
							pCharacter->SetVelX( 0.0f );
							pCharacter->SetVelY( -1.0f * pCharacter->GetSpeed() );
						}
						else
						{
							pCharacter->SetVelX( 0.0f );
							pCharacter->SetVelY( (float) pCharacter->GetSpeed() );
						}
						pCharacter->SetPrevVelX( 0.0f );
						pCharacter->SetPrevVelY( pCharacter->GetVelY() );
					}
					else
					{
						//then we are going east-west
						if(CPlayer::GetInstance()->GetPosX() < pCharacter->GetPosX())
						{
							pCharacter->SetVelX( -1.0f * pCharacter->GetSpeed() );
							pCharacter->SetVelY( 0.0f );
						}
						else
						{
							pCharacter->SetVelX( (float) pCharacter->GetSpeed() );
							pCharacter->SetVelY( 0.0f );
						}
						pCharacter->SetPrevVelX( pCharacter->GetVelX() );
						pCharacter->SetPrevVelY( 0.0f );
					}
					pCharacter->SetPhilDirection();	//	Fix our direction, for animation purpose, here.
				}
			}
			else if( pCharacter->GetMoveTimer() < 0.06f )
			{
				pCharacter->SetMoveTimer( pCharacter->GetMoveTimer() + fElapsedTime );	//	Increment timer.
			}
			else
			{
				//	Before leaving, let's see if the player is close enough for us to lunge at him!
				if( (CPlayer::GetInstance()->GetPosY() >= pCharacter->GetPosY() - 150 && CPlayer::GetInstance()->GetPosY() <= pCharacter->GetPosY() + 150) && (CPlayer::GetInstance()->GetPosX() >= pCharacter->GetPosX() - 150 && CPlayer::GetInstance()->GetPosX() <= pCharacter->GetPosX() + 150) )
				{
					pCharacter->SetMoveTimer( 0.0f );
					pCharacter->SetMiniState( 5 );
				}
				else
				{
					pCharacter->SetMoveTimer( 0.0f );
					pCharacter->SetMiniState( 4 );
				}
			}
			break;
		}
		case 4:	//	Pause a little, after moving, like a freakin' caterpillar, bro-dawg.
		{
			if( pCharacter->GetMoveTimer() == 0.0f )
			{
				pCharacter->SetMoveTimer( pCharacter->GetMoveTimer() + fElapsedTime );	//	Increment timer.
				pCharacter->SetVelX( 0.0f );
				pCharacter->SetVelY( 0.0f );
			}
			else if( pCharacter->GetMoveTimer() < 0.3f )	//	I will take a break for this long.
			{
				pCharacter->SetMoveTimer( pCharacter->GetMoveTimer() + fElapsedTime );	//	Increment timer.
			}
			else
			{
				pCharacter->SetMoveTimer( 0.0f );
				pCharacter->SetMiniState( 3 );	// Immediately squirm forward.
			}
			break;
		}
		case 5:		// LUNGE OF DEATH
		{
			if( pCharacter->GetMoveTimer() == 0.0f )
			{
				AUDIO->SFXPlaySound(m_sndLarvaHiss);
				pCharacter->SetMoveTimer( pCharacter->GetMoveTimer() + fElapsedTime );	//	Increment timer.
				pCharacter->SetVelX( pCharacter->GetVelX() * 1.8f );
				pCharacter->SetVelY( pCharacter->GetVelY() * 1.8f );
				pCharacter->m_bWalkCycle = 1;	//	Looks the most like an attack.
			}
			else if( pCharacter->GetMoveTimer() < 0.11f )	//	I will lunge for this long
			{
				pCharacter->SetMoveTimer( pCharacter->GetMoveTimer() + fElapsedTime );	//	Increment timer.
			}
			else
			{
				pCharacter->SetMoveTimer( 0.0f );
				pCharacter->SetMiniState( 4 );	// Immediately squirm forward.
			}
			break;
		}
	}





	//		if( pCharacter->GetMoveTimer() == 0.0f )
	//		{
	//			pCharacter->SetMoveTimer( pCharacter->GetMoveTimer() + fElapsedTime );
	//			unsigned int tempDir = rand()%8;
	//			if( tempDir == 0 )
	//			{
	//				pCharacter->SetVelX( 0.0f );
	//				pCharacter->SetVelY( -1.0 * pCharacter->GetSpeed() );
	//				pCharacter->SetCurrentAnimation(CBaseCharacter::ANM_WALK_UP);
	//			}
	//			else if( tempDir == 1 )
	//			{
	//				pCharacter->SetVelX( pCharacter->GetSpeed() );
	//				pCharacter->SetVelY( -1.0f * pCharacter->GetSpeed() );
	//				pCharacter->SetCurrentAnimation(CBaseCharacter::ANM_WALK_UP);
	//				float meh = pCharacter->GetVelX();
	//				float meh2= pCharacter->GetVelY();
	//				int x = 5;
	//			}
	//			else if( tempDir == 2 )
	//			{
	//				pCharacter->SetVelX( pCharacter->GetSpeed() );
	//				pCharacter->SetVelY( 0.0f );
	//				pCharacter->SetCurrentAnimation(CBaseCharacter::ANM_WALK_RIGHT);
	//			}
	//			else if( tempDir == 3 )
	//			{
	//				pCharacter->SetVelX( pCharacter->GetSpeed() );
	//				pCharacter->SetVelY( pCharacter->GetSpeed() );
	//				pCharacter->SetCurrentAnimation(CBaseCharacter::ANM_WALK_DOWN);
	//			}
	//			else if( tempDir == 4 )
	//			{
	//				pCharacter->SetVelX( 0.0f );
	//				pCharacter->SetVelY( pCharacter->GetSpeed() );
	//				pCharacter->SetCurrentAnimation(CBaseCharacter::ANM_WALK_DOWN);
	//			}
	//			else if( tempDir == 5 )
	//			{
	//				pCharacter->SetVelX( -1.0f * pCharacter->GetSpeed() );
	//				pCharacter->SetVelY( pCharacter->GetSpeed() );
	//				pCharacter->SetCurrentAnimation(CBaseCharacter::ANM_WALK_DOWN);
	//			}
	//			else if( tempDir == 6 )
	//			{
	//				pCharacter->SetVelX( -1.0f * pCharacter->GetSpeed() );
	//				pCharacter->SetVelY( 0 );
	//				pCharacter->SetCurrentAnimation(CBaseCharacter::ANM_WALK_LEFT);
	//			}
	//			else if( tempDir == 7 )
	//			{
	//				pCharacter->SetVelX( -1.0 * pCharacter->GetSpeed() );
	//				pCharacter->SetVelY( -1.0 * pCharacter->GetSpeed() );
	//				pCharacter->SetCurrentAnimation(CBaseCharacter::ANM_WALK_UP);
	//			}
	//		}
	//		else if( pCharacter->GetMoveTimer() < 1.0f )
	//		{
	//			pCharacter->SetMoveTimer( pCharacter->GetMoveTimer() + fElapsedTime );
	//		}
	//		else
	//		{
	//			pCharacter->SetVelX(0);
	//			pCharacter->SetVelY(0);
	//			pCharacter->SetMoveTimer( 0.0f );
	//			pCharacter->SetMiniState( 2 );	// set to stopped after wandering
	//		}
	//		break;
	//	}
	//	case 4:	// 4 - pause-before
	//	{
	//		if( pCharacter->GetMoveTimer() == 0.0f )
	//		{
	//			pCharacter->SetMoveTimer( pCharacter->GetMoveTimer() + fElapsedTime );
	//			//find the player direction!
	//			int temp_y_dist = pCharacter->GetPosY() - CPlayer::GetInstance()->GetPosY();
	//			int temp_x_dist = pCharacter->GetPosX() - CPlayer::GetInstance()->GetPosX();
	//			if(abs(temp_y_dist) < abs(temp_x_dist))
	//			{
	//				//then we are going north-south
	//				if(CPlayer::GetInstance()->GetPosY() < pCharacter->GetPosY())
	//				{
	//					pCharacter->SetVelX( 0.0f );
	//					pCharacter->SetVelY( -0.5f * pCharacter->GetSpeed() );
	//				}
	//				else
	//				{
	//					pCharacter->SetVelX( 0.0f );
	//					pCharacter->SetVelY( 0.5f * pCharacter->GetSpeed() );
	//				}
	//			}
	//			else
	//			{
	//				//then we are going east-west
	//				if(CPlayer::GetInstance()->GetPosX() < pCharacter->GetPosX())
	//				{
	//					pCharacter->SetVelX( -0.5f * pCharacter->GetSpeed() );
	//					pCharacter->SetVelY( 0.0f );
	//				}
	//				else
	//				{
	//					pCharacter->SetVelX( 0.5f * pCharacter->GetSpeed() );
	//					pCharacter->SetVelY( 0.0f );
	//				}
	//			}
	//		}
	//		else if( pCharacter->GetMoveTimer() < 0.5f )
	//		{
	//			pCharacter->SetMoveTimer( pCharacter->GetMoveTimer() + fElapsedTime );
	//		}
	//		else
	//		{
	//			pCharacter->SetMoveTimer( 0.0f );
	//			pCharacter->SetMiniState( 5 );	// set to pouncing after done crouching
	//		}
	//		break;
	//	}
	//	case 5:	// 5 - jump
	//	{
	//		if( pCharacter->GetMoveTimer() == 0.0f )
	//		{
	//			pCharacter->SetMoveTimer( pCharacter->GetMoveTimer() + fElapsedTime );
	//			//find the player direction!
	//			int temp_y_dist = pCharacter->GetPosY() - CPlayer::GetInstance()->GetPosY();
	//			int temp_x_dist = pCharacter->GetPosX() - CPlayer::GetInstance()->GetPosX();
	//			if(abs(temp_y_dist) > abs(temp_x_dist))
	//			{
	//				//then we are going north-south
	//				if(CPlayer::GetInstance()->GetPosY() < pCharacter->GetPosY())
	//				{
	//					pCharacter->SetVelX( 0.0f );
	//					pCharacter->SetVelY( -8.0f * pCharacter->GetSpeed() );
	//				}
	//				else
	//				{
	//					pCharacter->SetVelX( 0.0f );
	//					pCharacter->SetVelY( 8.0f * pCharacter->GetSpeed() );
	//				}
	//			}
	//			else
	//			{
	//				//then we are going east-west
	//				if(CPlayer::GetInstance()->GetPosX() < pCharacter->GetPosX())
	//				{
	//					pCharacter->SetVelX( -8.0f * pCharacter->GetSpeed() );
	//					pCharacter->SetVelY( 0.0f );
	//				}
	//				else
	//				{
	//					pCharacter->SetVelX( 8.0f * pCharacter->GetSpeed() );
	//					pCharacter->SetVelY( 0.0f );
	//				}
	//			}
	//		}
	//		else if( pCharacter->GetMoveTimer() < 0.6f )
	//		{
	//			pCharacter->SetMoveTimer( pCharacter->GetMoveTimer() + fElapsedTime );
	//		}
	//		else
	//		{
	//			pCharacter->SetVelX(0.0f);
	//			pCharacter->SetVelY(0.0f);
	//			pCharacter->SetMoveTimer( 0.0f );
	//			pCharacter->SetMiniState( 3 );
	//		}
	//		break;
	//	}
	//}




	//if(fMoveTimer < 0.0f)
	//	fMoveTimer = 0.0f;
	//if(pCharacter->GetVelX() != 0 && pCharacter->GetVelY() != 0)
	//{
	//	fMoveTimer += fElapsedTime;
	//	if(fMoveTimer >= 0.5f)
	//	{
	//		fMoveTimer = 0.0f;
	//		pCharacter->SetVelX( 0.0f );
	//		pCharacter->SetVelY( 0.0f );
	//	}
	//}
	//else
	//{
	//	fMoveTimer += fElapsedTime;
	//	if(fMoveTimer >= 3.0f)
	//	{
	//		fMoveTimer = 0.0f;

	//		if( CPlayer::GetInstance()->GetPosY() > pCharacter->GetPosY() )
	//		{
	//			pCharacter->SetVelY( 5.0f * pCharacter->GetSpeed() );
	//		}
	//		else if( CPlayer::GetInstance()->GetPosY() < pCharacter->GetPosY() )
	//		{
	//			pCharacter->SetVelY( -5.0f * pCharacter->GetSpeed() );
	//		}
	//		else
	//		{
	//			pCharacter->SetVelY( 0.0f );
	//		}

	//		if( CPlayer::GetInstance()->GetPosX() > pCharacter->GetPosX() )
	//		{
	//			pCharacter->SetVelX( 5.0f * pCharacter->GetSpeed() );
	//		}
	//		else if( CPlayer::GetInstance()->GetPosX() < pCharacter->GetPosX() )
	//		{
	//			pCharacter->SetVelX( -5.0f * pCharacter->GetSpeed() );
	//		}
	//		else
	//		{
	//			pCharacter->SetVelX( 0.0f );
	//		}
	//	}
	//	else
	//	{
	//		// nothing; we are waiting still.
	//		int x = 0;
	//	}
	//}

	//pCharacter->SetMoveTimer(fMoveTimer);
}

void CRatAIState::Exit(CBaseCharacter* pCharacter)
{

}
