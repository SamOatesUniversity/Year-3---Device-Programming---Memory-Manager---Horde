#ifndef ENEMYBASE_H_INCLUDED
#define ENEMYBASE_H_INCLUDED

#include "../../lasagne/lasagne.h"

class CEnemyBase {
protected:

	CLasagneEntity                  *m_entity;					//!<

	TVector<int, 2>					m_moveDirection;			//!< 

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

									//! 
	CLasagneEntity					*GetEntity() const
									{
										return m_entity;
									}

};

#endif