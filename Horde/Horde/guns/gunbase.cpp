#include "gunbase.h"

CGunBase::CGunBase() : m_nextBullet(0), m_entity(0)
{

}

CGunBase::~CGunBase()
{

}

void CGunBase::Shoot()
{
	if (m_bullet.empty())
		return;

	if (m_nextBullet > m_bullet.size())
		m_nextBullet = 0;

	m_bullet[m_nextBullet]->Fire();
}