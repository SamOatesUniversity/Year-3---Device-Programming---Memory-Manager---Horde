#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "../lasagne/lasagne.h"
#include <map>

class CEnemyBase;
class CGunBase;
class CGunPistol;

struct GunType {
	enum Enum {
		Pistol,
		Shotgun,
		Machinegun,
		Noof
	};
};

class CPlayer {

private:

    CLasagneEntity                          *m_entity;

	GunType::Enum							m_currentGun;
	CGunBase								*m_gun[GunType::Noof];

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

	void									IncreaseHealth(
												const int health
											)
											{
												m_health += health;
												if (m_health > 100)
													m_health = 100;
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

	void									SetGun(
												GunType::Enum gunType
											);

};

#endif // PLAYER_H_INCLUDED
