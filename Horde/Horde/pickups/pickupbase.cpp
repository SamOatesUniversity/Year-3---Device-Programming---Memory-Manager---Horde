#include "pickupbase.h"

CPickupBase::CPickupBase(
		CPlayer *player
	) :
	m_entity(NULL),
	m_player(player)
{

}

CPickupBase::~CPickupBase()
{
	CLasagne::GetInstance()->Destroy(&m_entity);
}

void CPickupBase::Update(
		int backGroundX,
		int backGroundY
	)
{
	m_entity->SetPosition(
		(m_entity->GetPosition().x()) - backGroundX, 
		(m_entity->GetPosition().y()) - backGroundY
	);

	const float xDiff = m_entity->GetPosition().x() - 132.0f;
	const float yDiff = m_entity->GetPosition().y() - 96.0f;

	if (CSOGI::GetInstance().IsAlmost(sqrt((xDiff * xDiff) + (yDiff * yDiff)), 0.0f, 32.0f))
	{
		OnPickup();
	}
}

void CPickupBase::Drop(
		const float x,
		const float y
	)
{
	if (m_entity == NULL)
		return;

	m_entity->SetVisible(true);
	m_entity->SetPosition(x, y);
}
