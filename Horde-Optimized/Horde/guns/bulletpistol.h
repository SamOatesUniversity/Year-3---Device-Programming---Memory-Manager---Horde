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
								CLasagneEntity *other					//!< The thing to check against
							);

							//! The power of the bullet (represents the damage done to an enemy on contact)
	virtual int				GetPower() const
							{
								return 35;
							}

};

#endif
