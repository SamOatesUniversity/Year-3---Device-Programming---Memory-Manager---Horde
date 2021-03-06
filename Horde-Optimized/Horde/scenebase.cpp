#include "scenebase.h"

CScene::CScene()
{
	for (int backgroundIndex = 0; backgroundIndex < NOOF_BACKGROUNDS; ++backgroundIndex)
		m_background[backgroundIndex] = NULL;

	m_cloud = NULL;
}

CScene::~CScene()
{
	for (int backgroundIndex = 0; backgroundIndex < NOOF_BACKGROUNDS; ++backgroundIndex)
		CLasagne::GetInstance()->Destroy(&m_background[backgroundIndex]);

	CLasagne::GetInstance()->Destroy(&m_cloud);
}

const bool CScene::Load(
        const std::string& folderLocation
    )
{
    const std::string backgroundLocation = folderLocation + "background.png";

	CLasagne *const engine = CLasagne::GetInstance();

    for (int backgroundIndex = 0; backgroundIndex < NOOF_BACKGROUNDS; ++backgroundIndex)
	{
        m_background[backgroundIndex] = engine->LoadImage(backgroundLocation.c_str(), 0);
		//m_background[backgroundIndex]->SetVisible(false);
	}

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            m_background[((x+1) * 3) + (y+1)]->SetPosition(x * 320.0f, y * 240.0f);
        }
    }

	m_cloud = CLasagne::GetInstance()->LoadImage("./data/graphics/cloud.png", 9);
	m_cloud->SetPosition(-320, 0);

    return true;
}

bool CScene::Move(
        const int xmove,
        const int ymove
    )
{
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            TVector<float, 2> pos = m_background[((x+1) * 3) + (y+1)]->GetPosition();
            float newX = pos.x() - xmove;
            float newY = pos.y() - ymove;
            if (x == -1 && newX > 0.0f) return false;
            if (x == 1 && newX < 0.0f) return false;
            if (y == -1 && newY > 0.0f) return false;
            if (y == 1 && newY < 0.0f) return false;
        }
    }

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
			const unsigned int bgrIndex = ((x+1) * 3) + (y+1);

			CLasagneEntity *bgr = m_background[bgrIndex];
            TVector<float, 2> pos = bgr->GetPosition();
            bgr->SetPosition(pos.x() - xmove, pos.y() - ymove);

			if(bgr->IsEnabled() && !bgr->IsOnScreen())
			{
				CLasagne::GetInstance()->DisableEntity(&bgr);
			}

			if(!bgr->IsEnabled() && bgr->IsOnScreen())
			{
				CLasagne::GetInstance()->EnableEntity(&bgr);
			}

        }
    }

	UpdateClouds(xmove, ymove);

    return true;
}

void CScene::UpdateClouds(int xmove, int ymove)
{
	TVector<float, 2> pos = m_cloud->GetPosition();

	if (m_cloud->IsEnabled() && !m_cloud->IsOnScreen()) {
		CLasagne::GetInstance()->DisableEntity(&m_cloud);
	}

	if (!m_cloud->IsEnabled() && m_cloud->IsOnScreen()) {
		CLasagne::GetInstance()->EnableEntity(&m_cloud);
	}

	if (pos.x() > 960)
		m_cloud->SetPosition(-320, 0);
	else
		m_cloud->SetPosition((pos.x() + 4) - xmove, pos.y() - ymove);
}
