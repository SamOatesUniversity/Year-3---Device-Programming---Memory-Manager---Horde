#include "pickupshotgun.h"

CPickupShotgun::CPickupShotgun(
		CPlayer *player
	) : 
	CPickupBase(player)
{
	TVector<int, 2> frameLayout;
	frameLayout.Set(8, 1);
	m_entity = CLasagne::GetInstance()->LoadAnimatedImage("./data/graphics/pickups/shotgun.png", frameLayout, 4);
	m_entity->SetFPS(5);
	m_entity->SetVisible(false);
}

CPickupShotgun::~CPickupShotgun()
{

}

void CPickupShotgun::OnPickup()
{
	if (!m_entity->IsVisible())
		return;

	m_player->SetGun(GunType::Shotgun);

	m_entity->SetVisible(false);
}
