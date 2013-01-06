#include "enemyzombie.h"


CEnemyZombie::CEnemyZombie()
{

}

CEnemyZombie::~CEnemyZombie()
{

}

bool CEnemyZombie::Create(
		int startX,
		int startY
	)
{

	TVector<int, 2> frameLayout;
	frameLayout.Set(8, 2);
	m_entity = CLasagne::GetInstance()->LoadAnimatedImage("./media/graphics/characters/zombie.png", frameLayout);
	if (!m_entity)
		return false;

	m_entity->AddAnimation("walk", 0, 9);
	m_entity->SetCurrentAnimation("walk");

	m_entity->SetPosition(static_cast<float>(startX), static_cast<float>(startY));

	m_moveDirection.Set(static_cast<int>((132 - m_entity->GetPosition().x()) * 0.01f), static_cast<int>((96 - m_entity->GetPosition().y()) * 0.01f));

	return true;
}

void CEnemyZombie::Update(
		int backGroundX,
		int backGroundY
	)
{

	const float alpha = static_cast<float>(132 - m_entity->GetPosition().x()) / static_cast<float>(96 - m_entity->GetPosition().y());
	const float radAngle = atan(alpha);
	int rotation = static_cast<int>(radAngle * 57.0f);

	if (m_entity->GetPosition().y() > 96)
		rotation += 180;

	m_entity->SetRotation(rotation);

	m_entity->SetPosition(
		(m_entity->GetPosition().x() + m_moveDirection.x()) - backGroundX, 
		(m_entity->GetPosition().y() + m_moveDirection.y()) - backGroundY
	);

}
