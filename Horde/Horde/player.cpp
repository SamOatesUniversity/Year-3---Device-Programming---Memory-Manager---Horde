#include "player.h"

CPlayer::CPlayer() :
    m_entity(NULL),
	m_gun(NULL),
	m_score(0)
{

}

CPlayer::~CPlayer()
{
	SafeDelete(m_gun);
}

const bool CPlayer::Load(
        const char* spriteSheet
    )
{
    TVector<int, 2> frameLayout;
    frameLayout.Set(8, 2);
    m_entity = CLasagne::GetInstance()->LoadAnimatedImage(spriteSheet, frameLayout);
    if (!m_entity)
        return false;

	m_entity->SetDepth(5);

    m_entity->SetPosition(132, 96);

    m_entity->AddAnimation("idle", 0, 9);
    m_entity->AddAnimation("walk", 10, 16);
    m_entity->SetCurrentAnimation("walk");

	m_gun = new CGunPistol();
	m_gun->Create();

	CLasagneEntity *const gun = m_gun->GetEntity();
	gun->SetPosition(132, 96);
	gun->AddAnimation("idle", 0, 9);
	gun->AddAnimation("walk", 10, 16);
	gun->SetCurrentAnimation("walk");

    return true;
}

void CPlayer::SetRotation( 
		const int alpha 
	)
{
	m_entity->SetRotation(alpha);
	m_gun->GetEntity()->SetRotation(alpha);
}

void CPlayer::SetCurrentAnimation( 
		char *animation 
	)
{
	m_entity->SetCurrentAnimation(animation);
	m_gun->GetEntity()->SetCurrentAnimation(animation);
}

void CPlayer::Update(
		std::vector<CEnemyBase*> &enemy
	)
{
	int noofKills = m_gun->Shoot(enemy);
	m_score += (noofKills * 10);
}
