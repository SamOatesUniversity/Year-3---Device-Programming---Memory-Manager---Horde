#include "bulletpistol.h"


CBulletPistol::CBulletPistol()
{
	m_speed = 10;
}

CBulletPistol::~CBulletPistol()
{

}

void CBulletPistol::Create()
{
	m_enity = CLasagne::GetInstance()->LoadImage("./media/graphics/characters/guns/pistol/bullet.png");
	m_enity->SetPosition(132, 96);
	m_enity->SetVisible(false);
}

bool CBulletPistol::CheckCollision( 
		CLasagneEntity *other 
	)
{
	return false;
}

void CBulletPistol::Fire(
		CLasagneEntity *gun
	)
{
	CBulletBase::Fire(gun);
}
