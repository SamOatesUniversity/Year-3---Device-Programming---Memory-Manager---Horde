#ifndef GAMEMAIN_HEADER
#define GAMEMAIN_HEADER

struct GameState {
	enum Enum {
		LoadingLevel,
		InLevel,
		LevelComplete
	};
};

GameState::Enum gameState = GameState::InLevel;

unsigned int levelEndTimer = 0;

#endif