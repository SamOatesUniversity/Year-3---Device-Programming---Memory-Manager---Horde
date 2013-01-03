#ifndef BULLETBASE_H_INCLUDED
#define BULLETBASE_H_INCLUDED

#include "../../lasagne/lasagne.h"

class CBulletBase {

protected:

	CLasagneEntity			*m_enity;				//!< 

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

	virtual void			Update();

};

#endif