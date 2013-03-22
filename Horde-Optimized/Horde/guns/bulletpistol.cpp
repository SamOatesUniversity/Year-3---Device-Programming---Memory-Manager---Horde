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
	m_entity = CLasagne::GetInstance()->LoadImage("./media/graphics/characters/guns/pistol/bullet.png", 5);
	m_entity->SetPosition(132, 96);
	m_entity->SetVisible(false);
}

bool CBulletPistol::CheckCollision( 
		CLasagneEntity *other 
	)
{
	// if the bullet isnt visible, dont check for collisions
	if (m_entity->IsVisible() == false)
		return false;

	if (m_entity->Intersects(other))
		return true;

	return false;
}
