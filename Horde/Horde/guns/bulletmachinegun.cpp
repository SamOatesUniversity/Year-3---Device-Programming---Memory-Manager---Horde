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
	m_entity = CLasagne::GetInstance()->LoadImage("./media/graphics/characters/guns/machinegun/bullet.png");
	m_entity->SetPosition(132, 96);
	m_entity->SetVisible(false);
	m_entity->SetDepth(5);
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
