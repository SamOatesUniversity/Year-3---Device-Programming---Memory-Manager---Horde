#include "pickuphealth.h"

CPickupHealth::CPickupHealth(
		CPlayer *player
	) : 
	CPickupBase(player)
{
	TVector<int, 2> frameLayout;
	frameLayout.Set(4, 1);
	m_entity = CLasagne::GetInstance()->LoadAnimatedImage("./media/graphics/pickups/health.png", frameLayout);
	m_entity->SetDepth(4);
	m_entity->SetVisible(false);
}

CPickupHealth::~CPickupHealth()
{

}

void CPickupHealth::OnPickup()
{
	if (!m_entity->IsVisible())
		return;

	m_player->IncreaseHealth(10);
	m_entity->SetVisible(false);
}
