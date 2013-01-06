#include "enemybase.h"

CEnemyBase::CEnemyBase() :
	m_entity(NULL)
{
	m_moveDirection.Set(0, 0);
}

CEnemyBase::~CEnemyBase()
{

}
