#include "gunbase.h"

CGunBase::CGunBase() :
	m_nextBullet(0),
	m_noofBullets(1),
	m_entity(NULL),
	m_audio(NULL),
	m_fireRate(1),
	m_lastShot(0)
{

}

CGunBase::~CGunBase()
{
	std::vector<CBulletBase*>::iterator iter = m_bullet.begin();
	std::vector<CBulletBase*>::iterator endIter = m_bullet.end();
	for (iter = iter; iter != endIter; ++iter)
	{
		SafeDelete(*iter);
	}
	m_bullet.clear();

	CLasagne::GetInstance()->Destroy(&m_entity);
	CLasagne::GetInstance()->Destroy(&m_audio);
}

void CGunBase::Shoot()
{
	if (m_bullet.empty())
		return;

	if (m_nextBullet >= m_bullet.size())
		m_nextBullet = 0;

	Uint32 timer = SDL_GetTicks();
	if (timer - m_lastShot > (1000 / m_fireRate))
	{
		if (m_audio != NULL)
			m_audio->Play();

		m_bullet[m_nextBullet]->Fire(m_entity);
		m_lastShot = timer;
		m_nextBullet++;

		if (m_noofBullets != 0)
			m_noofBullets--;
	}
}

int CGunBase::Update(
		std::vector<CEnemyBase*> &enemy
	)
{
	if (m_bullet.empty())
		return 0;

	int noofKills = 0;

	const int NoofBullets = m_bullet.size();
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
