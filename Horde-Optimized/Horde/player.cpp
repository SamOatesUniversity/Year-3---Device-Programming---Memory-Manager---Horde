#include "player.h"
#include "guns/gunpistol.h"
#include "guns/gunshotgun.h"
#include "guns/gunmachinegun.h"

CPlayer::CPlayer() :
    m_entity(NULL),
    m_hurtHUD(NULL),
	m_score(0),
	m_health(100)
{
	for (int gunType = 0; gunType < GunType::Noof; ++gunType)
		m_gun[gunType] = NULL;

	for (int audioIndex = 0; audioIndex < PlayerAudio::Noof; ++audioIndex)
		m_audio[audioIndex] = NULL;

}

CPlayer::~CPlayer()
{
	CLasagne::GetInstance()->Destroy(&m_entity);
	CLasagne::GetInstance()->Destroy(&m_hurtHUD);

	for (int gunType = 0; gunType < GunType::Noof; ++gunType)
		SafeDelete(m_gun[gunType]);

	for (int audioIndex = 0; audioIndex < PlayerAudio::Noof; ++audioIndex)
		CLasagne::GetInstance()->Destroy(&m_audio[audioIndex]);
}

const bool CPlayer::Load(
        const char* spriteSheet
    )
{
    TVector<int, 2> frameLayout;
    frameLayout.Set(8, 2);
    m_entity = CLasagne::GetInstance()->LoadAnimatedImage(spriteSheet, frameLayout, 6);
    if (!m_entity)
        return false;

    m_entity->SetPosition(132, 96);

	m_hurtHUD = CLasagne::GetInstance()->LoadImage("./data/graphics/characters/hurt.png", 9);
	m_hurtHUD->SetVisible(false);

	m_audio[PlayerAudio::Hurt] = CLasagne::GetInstance()->LoadAudioFile("./data/sound/character/hurt.wav");
	m_audio[PlayerAudio::Die] = CLasagne::GetInstance()->LoadAudioFile("./data/sound/character/die.wav");

    m_entity->AddAnimation(const_cast<char*>("idle"), 0, 9);
    m_entity->AddAnimation(const_cast<char*>("walk"), 10, 16);
    m_entity->SetCurrentAnimation(const_cast<char*>("walk"));

	m_gun[GunType::Pistol] = new CGunPistol();
	m_gun[GunType::Pistol]->Create();

	m_gun[GunType::Shotgun] = new CGunShotgun();
	m_gun[GunType::Shotgun]->Create();

	m_gun[GunType::Machinegun] = new CGunMachinegun();
	m_gun[GunType::Machinegun]->Create();

	for (int gunType = 0; gunType < GunType::Noof; ++gunType)
	{
		CLasagneEntity *const gun = m_gun[gunType]->GetEntity();
		gun->SetPosition(132, 96);
		gun->AddAnimation(const_cast<char*>("idle"), 0, 9);
		gun->AddAnimation(const_cast<char*>("walk"), 10, 16);
		gun->SetCurrentAnimation(const_cast<char*>("walk"));
		gun->SetVisible(false);
	}

	m_currentGun = GunType::Pistol;
	SetGun(m_currentGun);

    return true;
}

void CPlayer::SetRotation(
		const int alpha
	)
{
	m_entity->SetRotation(alpha);
	m_gun[m_currentGun]->GetEntity()->SetRotation(alpha);
}

void CPlayer::SetCurrentAnimation(
		char *animation
	)
{
	m_entity->SetCurrentAnimation(animation);
	m_gun[m_currentGun]->GetEntity()->SetCurrentAnimation(animation);
}

void CPlayer::Update(
		std::vector<CEnemyBase*> &enemy
	)
{
	if (m_health == 0)
		return;

	m_gun[m_currentGun]->Shoot();

	int noofKills = 0;
	for (int gunType = 0; gunType < GunType::Noof; ++gunType)
		noofKills += m_gun[gunType]->Update(enemy);

	m_score += (noofKills * 10);

	if (m_gun[m_currentGun]->GetNoofBullets() <= 0)
		SetGun(GunType::Pistol);

	for (unsigned int enemyIndex = 0; enemyIndex < enemy.size(); ++enemyIndex)
	{
		CEnemyBase *const currentEnemy = enemy[enemyIndex];
		if (currentEnemy->GetHealth() <= 0)
			continue;

		const float xDiff = currentEnemy->GetEntity()->GetPosition().x() - m_entity->GetPosition().x();
		const float yDiff = currentEnemy->GetEntity()->GetPosition().y() - m_entity->GetPosition().y();

		if (CSOGI::GetInstance().IsAlmost(sqrt(static_cast<float>(xDiff * xDiff) + static_cast<float>(yDiff * yDiff)), 0.0f, 12.0f))
		{
			m_health--;
			if (m_health <= 0)
			{
				m_health = 0;
				m_audio[PlayerAudio::Die]->Play();
				return;
			}

			m_hurtHUD->SetVisible(true);
			m_audio[PlayerAudio::Hurt]->Play();
			return;
		}
	}

	m_hurtHUD->SetVisible(false);
}

void CPlayer::SetGun(
		GunType::Enum gunType
	)
{
	m_gun[m_currentGun]->GetEntity()->SetVisible(false);
	m_currentGun = gunType;

	m_gun[m_currentGun]->GetEntity()->SetVisible(true);

	if (m_currentGun == GunType::Shotgun)
		m_gun[m_currentGun]->SetNoofBullets(6);
	else if (m_currentGun == GunType::Machinegun)
		m_gun[m_currentGun]->SetNoofBullets(30);
}
