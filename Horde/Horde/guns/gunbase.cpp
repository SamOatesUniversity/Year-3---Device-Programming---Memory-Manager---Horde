#include "gunbase.h"

CGunBase::CGunBase() :
	m_nextBullet(0),
	m_entity(0),
	m_lastShot(0),
	m_fireRate(1)
{

}

CGunBase::~CGunBase()
{
	std::vector<CBulletBase*>::iterator iter = m_bullet.begin();
	std::vector<CBulletBase*>::iterator endIter = m_bullet.end();

	for (iter; iter != endIter; iter++)
	{
		SafeDelete(*iter);
	}
	m_bullet.clear();

	CLasagne::GetInstance()->Destroy(&m_entity);
}

int CGunBase::Shoot(
		std::vector<CEnemyBase*> &enemy
	)
{
	if (m_bullet.empty())
		return 0;

	if (m_nextBullet >= m_bullet.size())
		m_nextBullet = 0;

	#pragma message(__FILE__" - SOates: Requires Optimization")

	Uint32 timer = SDL_GetTicks();						// #RANT
	if (timer - m_lastShot > (1000 / m_fireRate))		// can be optimized but I'm not aloud too in this part of the assessment.
														// What a great thing to be teaching us... "Don't bother optimizing your code as you go...
														// just hope you find it later after spending ages profiling and debugging, making your
														// employers pretty pissed off, that you didn't just write the code properly the first time".
	{
		m_bullet[m_nextBullet]->Fire(m_entity);
		m_lastShot = timer;
		m_nextBullet++;
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
