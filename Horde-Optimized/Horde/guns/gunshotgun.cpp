#include "gunshotgun.h"

CGunShotgun::CGunShotgun()
{
	m_fireRate = 1;
}

CGunShotgun::~CGunShotgun()
{

}

void CGunShotgun::Create()
{
	TVector<int, 2> frameLayout;
	frameLayout.Set(8, 2);
	m_entity = CLasagne::GetInstance()->LoadAnimatedImage("./media/graphics/characters/guns/shotgun/gun.png", frameLayout, 5);
	if (!m_entity)
		return;

	m_audio = CLasagne::GetInstance()->LoadAudioFile("./media/sound/guns/shotgun.wav");

	static const int NoofBullets = 9;
	for (int bulletIndex = 0; bulletIndex < NoofBullets; ++bulletIndex)
	{
		CBulletBase *bulletBase = new CBulletShotgun();
		bulletBase->Create();
		m_bullet.push_back(bulletBase);
	}

}

void CGunShotgun::Shoot()
{
	if (m_bullet.empty())
		return;

	if (m_nextBullet >= m_bullet.size())
		m_nextBullet = 0;

	Uint32 timer = SDL_GetTicks();						
	if (timer - m_lastShot > (1000 / m_fireRate))		
	{
		if (m_audio != NULL)
			m_audio->Play();
		
		for (int dir = 0; dir < ShotgunBulletDirection::Noof; ++dir)
		{
			static_cast<CBulletShotgun*>(m_bullet[m_nextBullet])->Fire(m_entity, static_cast<ShotgunBulletDirection::Enum>(dir));
			m_nextBullet++;
		}

		if (m_noofBullets != 0)
			m_noofBullets--;
		
		m_lastShot = timer;
	}
}
