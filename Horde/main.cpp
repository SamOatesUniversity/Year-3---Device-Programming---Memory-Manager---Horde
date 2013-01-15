#include "main.h"
#include "lasagne/lasagne.h"
#include "Horde/scenebase.h"
#include "Horde/player.h"
#include "Horde/enemy/enemyzombie.h"

const bool isLeftPressed();
const bool isRightPressed();
const bool isUpPressed();
const bool isDownPressed();

int main (int argc, char *argv[])
{
    // if we failed to create (setup sdl ect...) then exit
    CLasagne *const engine = CLasagne::GetInstance();

    if (!engine->Create())
    {
        engine->Release();
        return 0;
    }

    CScene *currentScene = new CScene();
    currentScene->Load("./media/graphics/level-1/");

	CPlayer *player = new CPlayer();
	player->Load("./media/graphics/characters/player.png");

	IVec2 spawnPoint;
	static const int swarmSize = 2;

	int noofEnemies = 30;

	std::vector<CEnemyBase*> enemy;
	for (int zombieIndex = 0; zombieIndex < noofEnemies; ++zombieIndex)
	{
		if (zombieIndex % swarmSize == 0)
			spawnPoint.Set(
				132 + ((rand() % 1024) - 512), 
				96 + ((rand() % 1024) - 512)
			);

		CEnemyZombie *zombieTest = new CEnemyZombie();
		zombieTest->Create(spawnPoint.x() + ((zombieIndex % swarmSize) * 64), spawnPoint.y(), player);
		enemy.push_back(zombieTest);
	}

	CLasagneText *const scoreText = engine->CreateText("Score: 0", 4, 20);
	CLasagneText *const healthText = engine->CreateText("Health: 100", 4, 4);

	CLasagneEntity *const playerDead = engine->LoadImage("./media/graphics/player-dead.png");
	playerDead->SetDepth(9);
	playerDead->SetPosition(96, 56);
	playerDead->SetVisible(false);

	CLasagneEntity *const levelComplete = engine->LoadImage("./media/graphics/level-complete.png");
	levelComplete->SetDepth(9);
	levelComplete->SetPosition(96, 56);
	levelComplete->SetVisible(false);
	
    Uint32 updateTimer = SDL_GetTicks();
	Uint32 scoreTimer = updateTimer;

    // whilst the engine is running loop
	do
	{
		if (gameState == GameState::LoadingLevel)
		{

			continue;
		}

        // update logic at 20 fps
        if (SDL_GetTicks() - updateTimer > 50)
        {
            const int xDiff = engine->GetMousePosition()->x() - 160;
            const int yDiff = engine->GetMousePosition()->y() - 120;

			int moveX = static_cast<int>(xDiff * 0.05f);
			int moveY = static_cast<int>(yDiff * 0.05f);

			if (player->GetHealth() == 0)
			{
				moveX = 0;
				moveY = 0;
			}
			else
			{
				if (yDiff != 0) // stop divide by 0
				{
					const float alpha = static_cast<float>(xDiff) / static_cast<float>(yDiff);
					const float radAngle = atan(alpha);
					int rotation = static_cast<int>(radAngle * 57.0f);

					if (yDiff < 0)
					{
						rotation += 180;
					}

					player->SetRotation(rotation);
				}
			}			

			if (!CSOGI::GetInstance().IsAlmost(sqrt(static_cast<float>(xDiff * xDiff) + static_cast<float>(yDiff * yDiff)), 0.0f, 25.0f))
			{
				player->SetCurrentAnimation("walk");
				if (!currentScene->Move(moveX, moveY))
				{
					player->SetCurrentAnimation("idle");
					moveX = 0;
					moveY = 0;
				}
			}
			else
			{
				// user pressing near the middle of the screen, so stand still
				player->SetCurrentAnimation("idle");
			}

			int noofDead = 0;
			for (unsigned int zombieIndex = 0; zombieIndex < enemy.size(); ++zombieIndex)
			{
				CEnemyBase *const currentEnemy = enemy[zombieIndex];
				currentEnemy->Update(moveX, moveY);
				if (currentEnemy->GetHealth() <= 0)
					noofDead++;
			}

			if (noofEnemies - noofDead == 0 && gameState == GameState::InLevel)
			{
				levelComplete->SetVisible(true);
				gameState = GameState::LevelComplete;
				levelEndTimer = SDL_GetTicks();
			}

			player->Update(enemy);
			
			std::stringstream scoreBuffer;
			scoreBuffer << "Score: " << player->GetScore();
			scoreText->SetText(scoreBuffer.str().c_str());

			std::stringstream healthBuffer;
			healthBuffer << "Health: " << player->GetHealth();
			healthText->SetText(healthBuffer.str().c_str());

			if (player->GetHealth() == 0)
			{
				if (!playerDead->IsVisible())
					playerDead->SetVisible(true);
			}

			if (gameState == GameState::LevelComplete && SDL_GetTicks() - levelEndTimer > 5000)
			{
				gameState = GameState::LoadingLevel;
				levelEndTimer = 0;
				player->SetCurrentAnimation("idle");
			}

            updateTimer = SDL_GetTicks();
        }

		if (player->GetHealth() != 0 && gameState == GameState::InLevel && SDL_GetTicks() - scoreTimer > 500)
		{
			player->IncreaseScore(1);
			scoreTimer = SDL_GetTicks();
		}

	} while (engine->Render());

    // free up resources and allocated memory
	delete currentScene;
	delete player;
	engine->Release();

	return 0;
}

const bool isLeftPressed()
{
    Uint8 *keystate = SDL_GetKeyState(NULL);
    return keystate[SDLK_a] || keystate[SDLK_LEFT];
}

const bool isRightPressed()
{
    Uint8 *keystate = SDL_GetKeyState(NULL);
    return keystate[SDLK_d] || keystate[SDLK_RIGHT];
}

const bool isUpPressed()
{
    Uint8 *keystate = SDL_GetKeyState(NULL);
    return keystate[SDLK_w] || keystate[SDLK_UP];
}

const bool isDownPressed()
{
    Uint8 *keystate = SDL_GetKeyState(NULL);
    return keystate[SDLK_s] || keystate[SDLK_DOWN];
}
