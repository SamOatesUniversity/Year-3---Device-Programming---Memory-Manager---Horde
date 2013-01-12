#ifndef BULLETBASE_H_INCLUDED
#define BULLETBASE_H_INCLUDED

#include "../../lasagne/lasagne.h"
#include "../enemy/enemybase.h"

class CBulletBase {

protected:

	CLasagneEntity			*m_entity;				//!< 

	TVector<float, 2>		m_position;				//!< 
	TVector<float, 2>		m_direction;			//!< 

	int						m_speed;				//!< 

public:
							//! 
							CBulletBase();

							//! 
							~CBulletBase();

							//! 
	virtual void			Create() = 0;

							//! 
	virtual bool			CheckCollision(
								CLasagneEntity *other
							);

							//!
	virtual void			Fire(
								CLasagneEntity *gun
							);

							//!
	virtual void			Update();

							//! 
	virtual void			Destroy();

							//! The power of the bullet (represents the damage done to an enemy on contact)
	virtual int				GetPower() const = 0;

};

#endif