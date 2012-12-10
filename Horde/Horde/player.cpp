#include "player.h"

CPlayer::CPlayer() :
    m_entity(NULL)
{

}

CPlayer::~CPlayer()
{

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

    m_entity->SetPosition(132, 96);

    m_entity->AddAnimation("idle", 0, 9);
    m_entity->AddAnimation("walk", 10, 16);

    m_entity->SetCurrentAnimation("idle");

    return true;
}
