#include "enemybase.h"

CEnemyBase::CEnemyBase() :
	m_entity(NULL),
	m_speed(1.0f),
	m_health(100),
	m_pickup(NULL)
{
	for (int audioIndex = 0; audioIndex < EnemyAudio::Noof; ++audioIndex)
		m_audio[audioIndex] = NULL;
}

CEnemyBase::~CEnemyBase()
{
	SafeDelete(m_pickup);
	CLasagne::GetInstance()->Destroy(&m_entity);
}

void CEnemyBase::Damage( 
		int amount								/*!< The amount of damage to do */ 
	)
{
	m_health -= amount;
	if (m_health <= 0)
	{
		OnDeath();
		if (m_audio[EnemyAudio::Die] != NULL)
			m_audio[EnemyAudio::Die]->Play();
	}
	else
	{
		if (m_audio[EnemyAudio::Hurt] != NULL)
			m_audio[EnemyAudio::Hurt]->Play();
	}
}
