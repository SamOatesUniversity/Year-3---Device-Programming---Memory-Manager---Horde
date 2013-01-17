#ifndef ENEMYZOMBIE_H_INCLUDED
#define ENEMYZOMBIE_H_INCLUDED

#include "enemybase.h"
#include "../pickups/pickuphealth.h"
#include "../pickups/pickupshotgun.h"

class CEnemyZombie : public CEnemyBase {
protected:

public:
									//!
									CEnemyZombie();

									//!
									~CEnemyZombie();

									//!
	virtual bool					Create(
										int startX,
										int startY,
										CPlayer *player
									);

									//!
	virtual void					Update(
										int backGroundX,
										int backGroundY
									);

									//! Called when an enemy dies
	virtual void					OnDeath();

};

#endif