#include "Emitter.h"
#include "CGame.h"

int RandomInt( int a, int b )
{
	if( a > b )
	{
		int t = b;
		b = a;
		a = t;
	}
	return (int)(((float)rand() / ((float)RAND_MAX + 1.0f)) * (b - a)) + a;
}
float RandomFloat( float a, float b )
{
	if( a > b )
	{
		float t = b;
		b = a;
		a = t;
	}
	return (float)(((float)rand() / ((float)RAND_MAX + 1.0)) * (b - a)) + a;
}

Emitter::Emitter()
{
	Data = new FlyWeight();
	reAnimate = false;
}
Emitter::Emitter(const Emitter& E)
{
	for(unsigned int i = 0; i < E.Particle_List.size(); i++)
		Particle_List.push_back(E.Particle_List[i]);

	Data = E.Data;


}
Emitter& Emitter::operator=(const Emitter& E)
{
	if(this == &E)
		return *this;
	else
		*this = E;

	return *this;
}
Emitter::~Emitter()
{
	for(unsigned int i = 0; i < Particle_List.size(); i++)
		delete (Particle*)(Particle_List[i]);

	delete Data;

	Particle_List.clear();
}

void Emitter::Init()
{
	Particle* tempParticle;

	for (int i = 0; i < Data->MaxParticles; i++)
	{
		tempParticle = new Particle();
		tempParticle->Life = -1;
		Particle_List.push_back(tempParticle);

		//if(Continuous == false)
		//ReSpwn(tempParticle);
	}
}
void Emitter::Update( float DT )
{
	for (unsigned int i = 0; i < Particle_List.size(); i++)
	{
		if (Particle_List[i]->Life > 0)
		{
			if (Data->FadeEffect == 2)
				Particle_List[i]->A -= Data->FadeValue * DT;
			else if (Data->FadeEffect == 1)
				Particle_List[i]->A += Data->FadeValue * DT;

			if (Data->ScaleEffect == 2)
				Particle_List[i]->Size_p += (Data->ScaleValue / 100.0f) * DT;
			else if (Data->ScaleEffect == 1)
				Particle_List[i]->Size_p -= (Data->ScaleValue / 100.0f) * DT;

			if (Data->RedEffect == 2)
				Particle_List[i]->R += Data->RedValue * DT;
			else if (Data->RedEffect == 1)
				Particle_List[i]->R -= Data->RedValue * DT;

			if (Data->GreenEffect == 2)
				Particle_List[i]->G += Data->GreenValue * DT;
			else if (Data->GreenEffect == 1)
				Particle_List[i]->G -= Data->GreenValue * DT;

			if (Data->BlueEffect == 2)
				Particle_List[i]->B += Data->BlueValue * DT;
			else if (Data->BlueEffect == 1)
				Particle_List[i]->B -= Data->BlueValue * DT;

			Particle_List[i]->PosX += Particle_List[i]->VelX * DT;
			Particle_List[i]->PosY += Particle_List[i]->VelY * DT;
			Particle_List[i]->Life -= 10000 * DT;
		}
		else if(Continuous)
		{
			ReSpwn(Particle_List[i]);
		}
		else if( reAnimate )
		{
			ReSpwn(Particle_List[i]);

			if ( i >= Particle_List.size() - 1 )
			{
				reAnimate = false;
			}
		}
		Data->EmitterPosX += Data->EmitterVelX * DT;
		Data->EmitterPosY += Data->EmitterVelY * DT;
	}

}
void Emitter::ReSpwn(Particle* P)
{
	P->Life = (float)(rand() % (int)(Data->MaxLife * 1000));
	P->Size_p = Data->fSize;

	P->Rotation = 0.0f;

	P->VelX = RandomFloat( Data->MinVelX , Data->MaxVelX );
	P->VelY = RandomFloat( Data->MinVelY , Data->MaxVelY );

	P->PosX = Data->EmitterPosX - (rand() % (int)(Data->SpwnAreaWidth * 2 + 1) - Data->SpwnAreaWidth) / 2;
	P->PosY = Data->EmitterPosY - (rand() % (int)(Data->SpwnAreaHeight * 2 + 1) - Data->SpwnAreaHeight) / 2;

}
void Emitter::Render()
{
	DWORD tempBlends[2] = {0};
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->GetRenderState(D3DRS_SRCBLEND, &tempBlends[0]);
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->GetRenderState(D3DRS_DESTBLEND, &tempBlends[1]);

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
	int image;
	Emitter* tempE = this;

	if (tempE->Image == -1)
		return;
	else
		image = tempE->Image;

	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, tempE->Data->Source);
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, tempE->Data->Dest);

	for (int i = tempE->Particle_List.size() - 1; i >= 0; i--)
	{
		Particle* tempP = tempE->Particle_List[i];

		if (tempP->Life <= 0)
			continue;

		int OffSetX = (int)(tempP->PosX - (float)CSGD_TextureManager::GetInstance()->GetTextureWidth(image) * tempP->Size_p / 2.0f);
		int OffsetY = (int)(tempP->PosY - (float)CSGD_TextureManager::GetInstance()->GetTextureHeight(image) * tempP->Size_p / 2.0f);

		if(tempP->Rotation == 0)
			CSGD_TextureManager::GetInstance()->Draw(image, OffSetX, OffsetY, tempP->Size_p, tempP->Size_p,NULL,0,0,0,D3DCOLOR_ARGB((int)tempP->A,(int)tempP->R,(int)tempP->G,(int)tempP->B)); 
		else
			CSGD_TextureManager::GetInstance()->Draw(image, (int)tempP->PosX, (int)tempP->PosY, tempP->Size_p, tempP->Size_p,NULL,0,0,0,D3DCOLOR_ARGB((int)tempP->A,(int)tempP->R,(int)tempP->G,(int)tempP->B));         

	}
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();


	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, tempBlends[0]);
	CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, tempBlends[1]);
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
}