#include "pickupmachinegun.h"

CPickupMachineGun::CPickupMachineGun(
		CPlayer *player
	) : 
	CPickupBase(player)
{
	TVector<int, 2> frameLayout;
	frameLayout.Set(8, 1);
	m_entity = CLasagne::GetInstance()->LoadAnimatedImage("./media/graphics/pickups/machinegun.png", frameLayout, 4);
	m_entity->SetFPS(5);
	m_entity->SetVisible(false);
}

CPickupMachineGun::~CPickupMachineGun()
{

}

void CPickupMachineGun::OnPickup()
{
	if (!m_entity->IsVisible())
		return;

	m_player->SetGun(GunType::Machinegun);

	m_entity->SetVisible(false);
}
