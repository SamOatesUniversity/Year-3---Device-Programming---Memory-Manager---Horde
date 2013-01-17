#ifndef BULLETMACHINEGUN_H_INCLUDED
#define BULLETMACHINEGUN_H_INCLUDED

#include "bulletbase.h"

class CBulletMachinegun : public CBulletBase 
{
public:
							//! 
							CBulletMachinegun();

							//! 
							~CBulletMachinegun();

							//! 
	virtual void			Create();

							//! 
	virtual bool			CheckCollision(
								CLasagneEntity *other
							);

							//! The power of the bullet (represents the damage done to an enemy on contact)
	virtual int				GetPower() const
							{
								return 50;
							}
};

#endif