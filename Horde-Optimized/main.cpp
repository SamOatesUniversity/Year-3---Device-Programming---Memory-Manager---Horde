#include "main.h"

/*
*	\brief The main entry point of the application
*/
int main (int argc, char *argv[])
{
    // grab access to the engine
    CLasagne *const engine = CLasagne::GetInstance();

	// if we failed to create (setup sdl ect...) then exit
    if (!engine->Create())
    {
        engine->Release();
        return 0;
    }

	engine->ShowTimers(false);

	// show the one time startup splash screen
	Uint32 updateTimer = SDL_GetTicks();
	CLasagneEntity *splashScreen = engine->LoadImage("./data/graphics/splash-screen.png", 9);
	gameState = GameState::SplashScreen;

	while (SDL_GetTicks() - updateTimer < 3000)
	{
		engine->Render();
	}

	ProcessInfo *processInfo = new ProcessInfo();
	processInfo->engine = engine;

	// Create a sampling thread
	SDL_Thread *samplingThread = SDL_CreateThread(SamplingThreadFunction, processInfo);

	gameState = GameState::LoadingLevel;
	// Load the first level into memory
	LoadLevel(currentLevel);

	// store the game start time
    updateTimer = SDL_GetTicks();
	Uint32 scoreTimer = updateTimer;

	// load the ambient music and play it
	CLasagneMusicFile *ambiantMusic = engine->LoadMusicFile("./data/sound/ambiant.ogg");
	ambiantMusic->Play();

	ProFy::TimerID updateTime;
	ProFy::GetInstance().CreateTimer(updateTime, ProFy::TimerType::CPU, "Update Loop Time");

	CLasagneEntity *pausedScreen = engine->LoadImage("./data/graphics/paused-menu.png", 9);
	pausedScreen->SetVisible(false);

	engine->Destroy(&splashScreen);
	engine->ShowTimers(true);

	gameState = GameState::InLevel;

    // whilst the engine is running loop
	do
	{
		if (engine->IsPaused())
		{
			// show the paused menu
			if (gameState != GameState::Paused)
			{
				gameState = GameState::Paused;
				pausedScreen->SetVisible(true);
				engine->ShowTimers(false);
			}
		}
		else
		{
			if (gameState == GameState::Paused)
			{
				gameState = GameState::InLevel;
				pausedScreen->SetVisible(false);
				engine->ShowTimers(true);
			}

			ProFy::GetInstance().StartTimer(updateTime);

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
					player->SetCurrentAnimation(const_cast<char*>("walk"));
					if (!currentScene->Move(moveX, moveY))
					{
						currentScene->UpdateClouds(0, 0);
						player->SetCurrentAnimation(const_cast<char*>("idle"));
						moveX = 0;
						moveY = 0;
					}
				}
				else
				{
					// user pressing near the middle of the screen, so stand still
					player->SetCurrentAnimation(const_cast<char*>("idle"));
					currentScene->UpdateClouds(0, 0);
				}

				// Update enemies, counting how many have died on the way
				int noofDead = 0;
				for (unsigned int zombieIndex = 0; zombieIndex < enemy.size(); ++zombieIndex)
				{
					CEnemyBase *const currentEnemy = enemy[zombieIndex];
					currentEnemy->Update(moveX, moveY);
					if (currentEnemy->GetHealth() <= 0)
						noofDead++;
				}

				// If there are no enemies left, and the player isn't dead, level has been completed
				if (enemy.size() - noofDead == 0 && gameState == GameState::InLevel && player->GetHealth() != 0)
				{
					levelComplete->SetVisible(true);
					gameState = GameState::LevelComplete;
					levelEndTimer = SDL_GetTicks();
				}

				// Update the player
				player->Update(enemy);

				// Out put text to the screen
				std::stringstream scoreBuffer;
				scoreBuffer << "Score: " << (player->GetScore() + totalScore);
				scoreText->SetText(scoreBuffer.str().c_str());

				std::stringstream healthBuffer;
				healthBuffer << "Health: " << player->GetHealth();
				healthText->SetText(healthBuffer.str().c_str());

				// if player health is zero, the player has died
				if (player->GetHealth() == 0 && gameState == GameState::InLevel)
				{
					if (!playerDead->IsVisible())
						playerDead->SetVisible(true);

					gameState = GameState::Dead;
					levelEndTimer = SDL_GetTicks();
				}

				// restart the game after 5 seconds after dying
				if (gameState == GameState::Dead && SDL_GetTicks() - levelEndTimer > 5000)
				{
					gameState = GameState::LoadingLevel;
					levelEndTimer = 0;
					totalScore = 0;
					ReleaseLevel();
					currentLevel = 1;
					wave = 0;
					LoadLevel(currentLevel);

					gameState = GameState::InLevel;
					continue;
				}

				// Load the next level after 5 seconds of level completing
				if (gameState == GameState::LevelComplete && SDL_GetTicks() - levelEndTimer > 5000)
				{
					gameState = GameState::LoadingLevel;
					levelEndTimer = 0;
					totalScore += (player->GetScore() + 250);
					ReleaseLevel();
					currentLevel++;
					wave++;
					LoadLevel(currentLevel);

					// loop the levels
					if (currentLevel == lastLevel)
						currentLevel = 0;

					gameState = GameState::InLevel;
					continue;
				}

				updateTimer = SDL_GetTicks();
				ProFy::GetInstance().EndTimer(updateTime);
			}

			// Update the players score
			if (player->GetHealth() != 0 && gameState == GameState::InLevel && SDL_GetTicks() - scoreTimer > 500)
			{
				player->IncreaseScore(1);
				scoreTimer = SDL_GetTicks();
			}
		}

	} while (engine->Render());

	//ProFy::GetInstance().OutputTimer(updateTime, true);
	SDL_WaitThread(samplingThread, NULL);

    // free up resources and allocated memory
	ReleaseLevel();
	engine->Release();

#ifdef GP2X
	chdir("/usr/gp2x");
	execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
#endif

	return 0;
}

/*
*	\brief Load a level by a given id
*/
const bool LoadLevel(
		const unsigned int id					//!< The id of the level to load
	)
{
	CLasagne *const engine = CLasagne::GetInstance();

	std::stringstream buf;
	buf << "./data/graphics/level-" << id << "/";

	currentScene = new CScene();
	currentScene->Load(buf.str().c_str());

	player = new CPlayer();
	player->Load("./data/graphics/characters/player.png");

	IVec2 spawnPoint;
	static const int swarmSize = 5;

	int noofEnemies = 10 + (wave * 5);

	if (noofEnemies > 50) noofEnemies = 50;

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

	scoreText = engine->CreateText(const_cast<char*>("Score: 0"), 4, 20);
	healthText = engine->CreateText(const_cast<char*>("Health: 100"), 4, 4);

	playerDead = engine->LoadImage("./data/graphics/player-dead.png", 9);
	playerDead->SetPosition(96, 56);
	playerDead->SetVisible(false);

	levelComplete = engine->LoadImage("./data/graphics/level-complete.png", 9);
	levelComplete->SetPosition(96, 56);
	levelComplete->SetVisible(false);

	return true;
}

/*
*	\brief Release the current load level and all its assets
*/
const bool ReleaseLevel()
{
	CLasagne *const engine = CLasagne::GetInstance();
	engine->Destroy(&scoreText);
	engine->Destroy(&healthText);
	engine->Destroy(&playerDead);
	engine->Destroy(&levelComplete);

	SafeDelete(player);
	SafeDelete(currentScene);

	for (unsigned int zombieIndex = 0; zombieIndex < enemy.size(); ++zombieIndex)
	{
		SafeDelete(enemy[zombieIndex]);
	}
	enemy.clear();

	return true;
}
