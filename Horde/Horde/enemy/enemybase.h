#ifndef ENEMYBASE_H_INCLUDED
#define ENEMYBASE_H_INCLUDED

#include "../../lasagne/lasagne.h"

class CEnemyBase {
protected:

	CLasagneEntity                  *m_entity;					//!<
	float							m_speed;					//!< The speed of the enity
	int								m_health;					//!< Howmuch health the enemy has

public:
									//!
									CEnemyBase();

									//!
	virtual							~CEnemyBase();

									//!
	virtual bool					Create(
										int startX,
										int startY
									) = 0;

									//!
	virtual void					Update(
										int backGroundX,
										int backGroundY
									) = 0;

									//! Called when an enemy dies
	virtual void					OnDeath() = 0;

									//! 
	CLasagneEntity					*GetEntity() const
									{
										return m_entity;
									}

									//! Get the enemies health
	const int						GetHealth() const
									{
										return m_health;
									}

									//! Damage an enemy by a given amount
	void							Damage(
										int amount										//!< THe amount of damage to do
									);
};

#endif