#include "enemyzombie.h"


CEnemyZombie::CEnemyZombie()
{
	m_speed = 2.0f;
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

	m_entity->SetDepth(7);

	m_entity->AddAnimation("walk", 0, 9);
	m_entity->AddAnimation("dead", 10, 16, false);
	m_entity->SetCurrentAnimation("walk");

	m_entity->SetPosition(static_cast<float>(startX), static_cast<float>(startY));

	return true;
}

void CEnemyZombie::Update(
		int backGroundX,
		int backGroundY
	)
{
	if (!m_entity->IsVisible())
		return;

	if (m_health <= 0)
		m_speed = 0.0f;

	TVector<float, 2> moveDirection;
	moveDirection.Set(132 - m_entity->GetPosition().x(), 96 - m_entity->GetPosition().y());
	moveDirection.Normalize2D();
	moveDirection.Scale(m_speed);

	if (m_health > 0)
	{
		const float alpha = static_cast<float>(132 - m_entity->GetPosition().x()) / static_cast<float>(96 - m_entity->GetPosition().y());
		const float radAngle = atan(alpha);
		int rotation = static_cast<int>(radAngle * 57.0f);

		if (m_entity->GetPosition().y() > 96)
			rotation += 180;

		m_entity->SetRotation(rotation);
	}

	m_entity->SetPosition(
		(m_entity->GetPosition().x() + moveDirection.x()) - backGroundX, 
		(m_entity->GetPosition().y() + moveDirection.y()) - backGroundY
	);

}

void CEnemyZombie::OnDeath()
{
	m_entity->SetDepth(1);
	m_entity->SetCurrentAnimation("dead");
}
