#include "enemybase.h"

CEnemyBase::CEnemyBase() :
	m_entity(NULL),
	m_speed(1.0f),
	m_health(100),
	m_pickup(NULL)
{
}

CEnemyBase::~CEnemyBase()
{
	SafeDelete(m_pickup);
}

void CEnemyBase::Damage( 
		int amount								/*!< The amount of damage to do */ 
	)
{
	m_health -= amount;
	if (m_health <= 0)
	{
		OnDeath();
	}
}
