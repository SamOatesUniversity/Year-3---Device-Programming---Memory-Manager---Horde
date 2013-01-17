#include "gunshotgun.h"

CGunShotgun::CGunShotgun()
{
	m_fireRate = 1;
}

CGunShotgun::~CGunShotgun()
{

}

void CGunShotgun::Create()
{
	TVector<int, 2> frameLayout;
	frameLayout.Set(8, 2);
	m_entity = CLasagne::GetInstance()->LoadAnimatedImage("./media/graphics/characters/guns/shotgun/gun.png", frameLayout);
	if (!m_entity)
		return;

	m_entity->SetDepth(5);

	static const int NoofBullets = 9;
	for (int bulletIndex = 0; bulletIndex < NoofBullets; ++bulletIndex)
	{
		CBulletBase *bulletBase = new CBulletShotgun();
		bulletBase->Create();
		m_bullet.push_back(bulletBase);
	}

}

int CGunShotgun::Shoot(
		std::vector<CEnemyBase*> &enemy
	)
{
	if (m_bullet.empty())
		return 0;

	if (m_nextBullet >= m_bullet.size())
		m_nextBullet = 0;

	Uint32 timer = SDL_GetTicks();						
	if (timer - m_lastShot > (1000 / m_fireRate))		
	{
		for (int dir = 0; dir < ShotgunBulletDirection::Noof; ++dir)
		{
			static_cast<CBulletShotgun*>(m_bullet[m_nextBullet])->Fire(m_entity, static_cast<ShotgunBulletDirection::Enum>(dir));
			m_nextBullet++;
		}

		m_lastShot = timer;
	}

	int noofKills = 0;

	static const int NoofBullets = m_bullet.size();
	for (int bulletIndex = 0; bulletIndex < NoofBullets; ++bulletIndex)
	{
		CBulletBase *const bullet = m_bullet[bulletIndex];
		bullet->Update();

		if (!bullet->GetEntity()->IsVisible())
			continue;

		for (unsigned int enemyIndex = 0; enemyIndex < enemy.size(); ++enemyIndex)
		{
			CEnemyBase *const currentEnemy = enemy[enemyIndex];

			// don't check against invisible enemies
			if (!currentEnemy->GetEntity()->IsVisible())
				continue;

			// don't check against dead enemies
			if (currentEnemy->GetHealth() <= 0)
				continue;

			// don't check enemies not on the screen
			if (!currentEnemy->GetEntity()->IsOnScreen())
				continue;

			if (bullet->CheckCollision(currentEnemy->GetEntity()))
			{
				currentEnemy->Damage(bullet->GetPower());
				bullet->Destroy();
				noofKills++;
			}
		}
	}

	return noofKills;
}
