#include "bulletpistol.h"


CBulletPistol::CBulletPistol()
{

}

CBulletPistol::~CBulletPistol()
{

}

void CBulletPistol::Create()
{
	m_enity = CLasagne::GetInstance()->LoadImage("./media/graphics/characters/guns/pistol/bullet.png");
}

bool CBulletPistol::CheckCollision( 
		CLasagneEntity *other 
	)
{
	return false;
}

void CBulletPistol::Fire()
{

}
