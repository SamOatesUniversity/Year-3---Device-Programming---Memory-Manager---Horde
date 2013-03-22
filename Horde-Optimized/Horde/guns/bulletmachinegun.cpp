#include "bulletmachinegun.h"

CBulletMachinegun::CBulletMachinegun()
{
	m_speed = 30;
}

CBulletMachinegun::~CBulletMachinegun()
{

}

void CBulletMachinegun::Create()
{
	m_entity = CLasagne::GetInstance()->LoadImage("./media/graphics/characters/guns/machinegun/bullet.png", 5);
	m_entity->SetPosition(132, 96);
	m_entity->SetVisible(false);
}

bool CBulletMachinegun::CheckCollision( 
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
