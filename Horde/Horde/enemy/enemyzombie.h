#ifndef ENEMYZOMBIE_H_INCLUDED
#define ENEMYZOMBIE_H_INCLUDED

#include "enemybase.h"

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
										int startY
									);

									//!
	virtual void					Update(
										int backGroundX,
										int backGroundY
									);

};

#endif