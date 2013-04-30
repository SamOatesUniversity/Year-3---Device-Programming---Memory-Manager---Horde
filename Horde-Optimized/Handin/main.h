#ifndef GAMEMAIN_HEADER
#define GAMEMAIN_HEADER

/************************************************************************/
/* HEADER FILES                                                         */
/************************************************************************/

#include "lasagne/lasagne.h"
#include "Horde/scenebase.h"
#include "Horde/player.h"
#include "Horde/enemy/enemyzombie.h"
#include "profiling/ProFySampler.h"

/************************************************************************/
/* STRUCTURES                                                           */
/************************************************************************/

//! Specifies the current game state
struct GameState {
	enum Enum {
		SplashScreen,																		//!< The games splashscreen is currently showing
		Paused,																				//!< The game is currently paused
		LoadingLevel,																		//!< The current level is loading
		InLevel,																			//!< Currently playing a level
		LevelComplete,																		//!< Level completed, prepare for loading of next level
		Dead,																				//!< Player has died, prepare to restart game
		Unkown																				//!< The game is in an unknown state
	};
};

/************************************************************************/
/* VARIABLES                                                            */
/************************************************************************/

GameState::Enum						gameState = GameState::Unkown;							//!< The current state of the game

CLasagneText						*scoreText;												//!< The text overlay of the players score
CLasagneText						*healthText;											//!< The text overlay of the players health
CLasagneEntity						*playerDead;											//!< The sprite overlay, shown when the player has died
CLasagneEntity						*levelComplete;											//!< The sprite overlay, shown when the player completes a level

CPlayer								*player;												//!< An instance of our player
CScene								*currentScene;											//!< The instance of our currently loaded scene
std::vector<CEnemyBase*>			enemy;													//!< A list of the enemies in the scene

unsigned int						currentLevel = 1;										//!< The current level being played
const unsigned int					lastLevel = 3;											//!< The last level of the game
unsigned int						levelEndTimer = 0;										//!< A timer set when a level has ended 
unsigned int						wave = 0;												//!< The current wave the player is against
unsigned int						totalScore = 0;											//!< The total score the player has

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/

									//! Load a level by a given id
const bool							LoadLevel(
										const unsigned int id								//!< The id of the level to load
									);

									//! Release the current load level and all its assets
const bool							ReleaseLevel();


#endif
