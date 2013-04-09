#include "bulletbase.h"

CBulletBase::CBulletBase() : m_entity(NULL), m_speed(1)
{

}

CBulletBase::~CBulletBase()
{
	CLasagne::GetInstance()->Destroy(&m_entity);
}

bool CBulletBase::CheckCollision( 
		CLasagneEntity *other 
	)
{
	return false;
}

void CBulletBase::Fire(
		CLasagneEntity *gun
	)
{
	TVector<float, 2> startPosition = gun->GetPosition();
	TVector<float, 2> direction = gun->GetDirection();
	const int rotation = gun->GetRotation();

	startPosition.Set(startPosition.x() + 24, startPosition.y() + 24);	// position at center of player

	m_position.Set(startPosition.x(), startPosition.y());
	m_direction.Set(direction.x(), direction.y());

	m_entity->SetRotation(rotation);
	m_entity->SetVisible(true);
	//CLasagne::GetInstance()->EnableEntity(&m_entity);
}

void CBulletBase::Update()
{
	if (!m_entity->IsVisible())
		return;

	m_position.SetX(m_position.x() + (m_direction.x() * m_speed));
	m_position.SetY(m_position.y() + (m_direction.y() * m_speed));

	m_entity->SetPosition(m_position.x(), m_position.y());

	if (!m_entity->IsOnScreen())
		Destroy();
}

void CBulletBase::Destroy()
{
	m_entity->SetVisible(false);
	//CLasagne::GetInstance()->DisableEntity(&m_entity);
}
