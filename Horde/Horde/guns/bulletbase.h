#ifndef BULLETBASE_H_INCLUDED
#define BULLETBASE_H_INCLUDED

#include "../../lasagne/lasagne.h"

class CBulletBase {

protected:

	CLasagneEntity			*m_enity;				//!< 

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
	virtual void			Fire();

};

#endif