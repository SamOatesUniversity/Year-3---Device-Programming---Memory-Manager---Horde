#ifndef GAMEMAIN_HEADER
#define GAMEMAIN_HEADER

#include <vld.h>

#include "lasagne/lasagne.h"
#include "Horde/scenebase.h"
#include "Horde/player.h"
#include "Horde/enemy/enemyzombie.h"

struct GameState {
	enum Enum {
		LoadingLevel,
		InLevel,
		LevelComplete
	};
};

GameState::Enum gameState = GameState::InLevel;
unsigned int levelEndTimer = 0;

CLasagneText *scoreText;
CLasagneText *healthText;

CPlayer *player;
CScene *currentScene;
std::vector<CEnemyBase*> enemy;

CLasagneEntity *playerDead;
CLasagneEntity *levelComplete;

bool LoadLevel(int id);
bool ReleaseLevel();

#endif