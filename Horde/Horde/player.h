#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "../lasagne/lasagne.h"

#include "enemy/enemybase.h"

#include "guns/gunbase.h"
#include "guns/gunpistol.h"

#include <map>

class CPlayer {

private:

    CLasagneEntity                          *m_entity;
	CGunBase								*m_gun;
	int										m_score;
	int										m_health;

public:

											//! Class constructor
											CPlayer();

											//! Class destructor
											~CPlayer();

											//! Load the player graphics
    const bool								Load(
											    const char* spriteSheet
											);

											//! Get the render-able entity of the player
    CLasagneEntity							*GetEntity() const
											{
												return m_entity;
											}

	const int								GetScore() const
											{
												return m_score;
											}

	const int								GetHealth() const
											{
												return m_health;
											}

	void									IncreaseScore(
												const int score
											)
											{
												m_score += score;
											}

	void									SetRotation(
												const int alpha
											);

	void									SetCurrentAnimation(
												char *animation
											);

	void									Update(
												std::vector<CEnemyBase*> &enemy
											);

};

#endif // PLAYER_H_INCLUDED
