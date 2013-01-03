#ifndef BULLETPISTOL_H_INCLUDED
#define BULLETPISTOL_H_INCLUDED

#include "bulletbase.h"

class CBulletPistol : public CBulletBase 
{
public:
							//! 
							CBulletPistol();

							//! 
							~CBulletPistol();

							//! 
	virtual void			Create();

							//! 
	virtual bool			CheckCollision(
								CLasagneEntity *other
							);

							//!
	virtual void			Fire(
								CLasagneEntity *gun
							);
};

#endif