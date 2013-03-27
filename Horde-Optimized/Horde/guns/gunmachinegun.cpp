#include "gunmachinegun.h"

CGunMachinegun::CGunMachinegun()
{
	m_fireRate = 10;
	m_noofBullets = 45;
}

CGunMachinegun::~CGunMachinegun()
{

}

void CGunMachinegun::Create()
{
	TVector<int, 2> frameLayout;
	frameLayout.Set(8, 2);
	m_entity = CLasagne::GetInstance()->LoadAnimatedImage("./data/graphics/characters/guns/machinegun/gun.png", frameLayout, 5);
	if (!m_entity)
		return;

	m_audio = CLasagne::GetInstance()->LoadAudioFile("./data/sound/guns/machinegun.wav");

	static const int NoofBullets = 45;
	for (int bulletIndex = 0; bulletIndex < NoofBullets; ++bulletIndex)
	{
		CBulletBase *bulletBase = new CBulletMachinegun();
		bulletBase->Create();
		m_bullet.push_back(bulletBase);
	}

}

