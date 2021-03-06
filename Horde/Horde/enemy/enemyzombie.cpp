#include "enemyzombie.h"


CEnemyZombie::CEnemyZombie()
{
	m_speed = 1.0f + (((rand() % 1000) + 1) / 250);
}

CEnemyZombie::~CEnemyZombie()
{

}

bool CEnemyZombie::Create(
		int startX,
		int startY,
		CPlayer *player
	)
{
	TVector<int, 2> frameLayout;
	frameLayout.Set(8, 3);
	m_entity = CLasagne::GetInstance()->LoadAnimatedImage("./media/graphics/characters/zombie.png", frameLayout);
	if (!m_entity)
		return false;

	m_audio[EnemyAudio::Hurt] = CLasagne::GetInstance()->LoadAudioFile("./media/sound/enemy/hurt.wav");
	m_audio[EnemyAudio::Die] = CLasagne::GetInstance()->LoadAudioFile("./media/sound/enemy/die.wav");

	m_entity->SetDepth(7);

	m_entity->AddAnimation(const_cast<char*>("walk"), 0, 9);
	m_entity->AddAnimation(const_cast<char*>("dead"), 10, 16, false);
	m_entity->AddAnimation(const_cast<char*>("fade"), 17, 24, false);
	m_entity->SetCurrentAnimation(const_cast<char*>("walk"));

	m_entity->SetPosition(static_cast<float>(startX), static_cast<float>(startY));

	if (rand() % 5 == 0)
		m_pickup = new CPickupHealth(player);
	else if (rand() % 5 == 0)
		m_pickup = new CPickupShotgun(player);
	else if (rand() % 5 == 0)
		m_pickup = new CPickupMachineGun(player);

	return true;
}

void CEnemyZombie::Update(
		int backGroundX,
		int backGroundY
	)
{
	if (m_pickup != NULL)
		m_pickup->Update(backGroundX, backGroundY);

	if (!m_entity->IsVisible())
		return;

	if (m_health <= 0)
	{
		m_speed = 0.0f;

		static const Uint32 fadeAfterDeath = 3000; 
		if (SDL_GetTicks() - m_dieTimer > fadeAfterDeath)
		{
			if (SDL_GetTicks() - m_dieTimer > (fadeAfterDeath + 2000))
			{
				m_entity->SetVisible(false);
				return;
			}
			m_entity->SetCurrentAnimation(const_cast<char*>("fade"));
			m_entity->SetFPS(10);
		}
	}

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
	m_entity->SetCurrentAnimation(const_cast<char*>("dead"));

	if (m_pickup != NULL)
	{
		m_pickup->Drop(m_entity->GetPosition().x() + 16, m_entity->GetPosition().y() + 16);
	}
}
