#include "enemybase.h"

CEnemyBase::CEnemyBase() :
	m_entity(NULL),
	m_speed(1.0f),
	m_health(100),
	m_pickup(NULL),
	m_dieTimer(0)
{
	for (int audioIndex = 0; audioIndex < EnemyAudio::Noof; ++audioIndex)
		m_audio[audioIndex] = NULL;
}

CEnemyBase::~CEnemyBase()
{
	SafeDelete(m_pickup);
	CLasagne::GetInstance()->Destroy(&m_entity);

	for (int audioIndex = 0; audioIndex < EnemyAudio::Noof; ++audioIndex)
		CLasagne::GetInstance()->Destroy(&m_audio[audioIndex]);
}

void CEnemyBase::Damage( 
		int amount								/*!< The amount of damage to do */ 
	)
{
	m_health -= amount;
	if (m_health <= 0)
	{
		OnDeath();
		m_dieTimer = SDL_GetTicks();
		if (m_audio[EnemyAudio::Die] != NULL)
			m_audio[EnemyAudio::Die]->Play();
	}
	else
	{
		if (m_audio[EnemyAudio::Hurt] != NULL)
			m_audio[EnemyAudio::Hurt]->Play();
	}
}
