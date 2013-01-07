#include "bulletpistol.h"


CBulletPistol::CBulletPistol()
{
	m_speed = 50;
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
	// if the bullet isnt visible, dont check for collisions
	if (m_enity->IsVisible() == false)
		return false;

	if (m_enity->Intersects(other))
		return true;

	return false;
}

void CBulletPistol::Fire(
		CLasagneEntity *gun
	)
{
	CBulletBase::Fire(gun);
}
