#include "gunpistol.h"

CGunPistol::CGunPistol()
{
	m_fireRate = 1;
}

CGunPistol::~CGunPistol()
{

}

void CGunPistol::Create()
{
	TVector<int, 2> frameLayout;
	frameLayout.Set(8, 2);
	m_entity = CLasagne::GetInstance()->LoadAnimatedImage("./media/graphics/characters/guns/pistol/gun.png", frameLayout);
	if (!m_entity)
		return;

	static const int NoofBullets = 2;
	for (int bulletIndex = 0; bulletIndex < NoofBullets; ++bulletIndex)
	{
		CBulletBase *bulletBase = new CBulletPistol();
		bulletBase->Create();
		m_bullet.push_back(bulletBase);
	}

}

void CGunPistol::Shoot(
		std::vector<CEnemyBase*> &enemy
	)
{
	CGunBase::Shoot(enemy);
}
