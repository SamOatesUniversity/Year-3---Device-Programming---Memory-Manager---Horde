#include "bulletbase.h"

CBulletBase::CBulletBase() : m_enity(NULL)
{

}

CBulletBase::~CBulletBase()
{

}

bool CBulletBase::CheckCollision( 
		CLasagneEntity *other 
	)
{
	return false;
}

void CBulletBase::Fire()
{

}
