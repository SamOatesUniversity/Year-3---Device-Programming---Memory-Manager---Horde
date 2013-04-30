#include "gunpistol.h"

CGunPistol::CGunPistol()
{
	m_fireRate = 2;
}

CGunPistol::~CGunPistol()
{

}

void CGunPistol::Create()
{
	TVector<int, 2> frameLayout;
	frameLayout.Set(8, 2);
	m_entity = CLasagne::GetInstance()->LoadAnimatedImage("./data/graphics/characters/guns/pistol/gun.png", frameLayout, 5);
	if (!m_entity)
		return;

	CLasagne::GetInstance()->DisableEntity(&m_entity);

	m_audio = CLasagne::GetInstance()->LoadAudioFile("./data/sound/guns/pistol.wav");

	static const int NoofBullets = 1;
	for (int bulletIndex = 0; bulletIndex < NoofBullets; ++bulletIndex)
	{
		CBulletBase *bulletBase = new CBulletPistol();
		bulletBase->Create();
		m_bullet.push_back(bulletBase);
	}

}
