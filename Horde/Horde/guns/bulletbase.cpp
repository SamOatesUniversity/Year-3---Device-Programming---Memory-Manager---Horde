#include "bulletbase.h"

CBulletBase::CBulletBase() : m_enity(NULL), m_speed(1)
{

}

CBulletBase::~CBulletBase()
{

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
	//startPosition.Set(startPosition.x() - (16 * cos(rotation * 0.017f)), startPosition.y() - (16 * sin(rotation * 0.017f)));	// need to offset onto gun somehow

	m_position.Set(startPosition.x(), startPosition.y());
	m_direction.Set(direction.x(), direction.y());

	m_enity->SetRotation(rotation);
	m_enity->SetVisible(true);
}

void CBulletBase::Update()
{
	if (m_enity == NULL)
		return;

	m_position.SetX(m_position.x() + (m_direction.x() * m_speed));
	m_position.SetY(m_position.y() + (m_direction.y() * m_speed));

	m_enity->SetPosition(m_position.x(), m_position.y());
}
