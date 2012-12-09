#include "scenebase.h"

CScene::CScene()
{

}

CScene::~CScene()
{

}

const bool CScene::Load(
        const std::string& folderLocation
    )
{
    const std::string backgroundLocation = folderLocation + "background.jpg";

    for (int backgroundIndex = 0; backgroundIndex < 9; ++backgroundIndex)
        m_background[backgroundIndex] = CLasagne::GetInstance()->LoadImage(backgroundLocation.c_str());

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            m_background[((x+1) * 3) + (y+1)]->SetPosition(x * 320.0f, y * 240.0f);
        }
    }

    return true;
}

void CScene::Move(
        const float xmove,
        const float ymove
    )
{
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            TVector<float, 2> pos = m_background[((x+1) * 3) + (y+1)]->GetPosition();
            float newX = pos.x() - xmove;
            float newY = pos.y() - ymove;
            if (x == -1 && newX > 0.0f) return;
            if (x == 1 && newX < 0.0f) return;
            if (y == -1 && newY > 0.0f) return;
            if (y == 1 && newY < 0.0f) return;
        }
    }

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            TVector<float, 2> pos = m_background[((x+1) * 3) + (y+1)]->GetPosition();
            m_background[((x+1) * 3) + (y+1)]->SetPosition(pos.x() - xmove, pos.y() - ymove);
        }
    }
}
