#include "bulletshotgun.h"

CBulletShotgun::CBulletShotgun()
{
	m_speed = 30;
}

CBulletShotgun::~CBulletShotgun()
{

}

void CBulletShotgun::Create()
{
	m_entity = CLasagne::GetInstance()->LoadImage("./data/graphics/characters/guns/shotgun/bullet.png", 5);
	m_entity->SetPosition(132, 96);
	m_entity->SetVisible(false);
	CLasagne::GetInstance()->DisableEntity(&m_entity);
}

bool CBulletShotgun::CheckCollision( 
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

void CBulletShotgun::Fire(
		CLasagneEntity *gun,
		ShotgunBulletDirection::Enum direction
	)
{
	TVector<float, 2> startPosition = gun->GetPosition();
	TVector<float, 2> bulletDirection;
	const int rotation = gun->GetRotation();

	static const int SPREAD = 10;

	if (direction == ShotgunBulletDirection::Left)
	{
		bulletDirection.Set(
			sin(static_cast<float>((rotation - SPREAD) * 0.017f)),
			cos(static_cast<float>((rotation - SPREAD) * 0.017f))
		);
	}
	else if (direction == ShotgunBulletDirection::Right)
	{
		bulletDirection.Set(
			sin(static_cast<float>((rotation + SPREAD) * 0.017f)),
			cos(static_cast<float>((rotation + SPREAD) * 0.017f))
		);
	}
	else
	{
		bulletDirection = gun->GetDirection();
	}

	startPosition.Set(startPosition.x() + 24, startPosition.y() + 24);	// position at center of player
	m_position.Set(startPosition.x(), startPosition.y());
	m_direction.Set(bulletDirection.x(), bulletDirection.y());

	m_entity->SetRotation(rotation);
	m_entity->SetVisible(true);
	//CLasagne::GetInstance()->EnableEntity(&m_entity);
}
