#ifndef BULLETSHOTGUN_H_INCLUDED
#define BULLETSHOTGUN_H_INCLUDED

#include "bulletbase.h"

struct ShotgunBulletDirection {
	enum Enum {
		Left,
		Center,
		Right,
		Noof
	};
};

class CBulletShotgun : public CBulletBase 
{
public:
							//! 
							CBulletShotgun();

							//! 
							~CBulletShotgun();

							//! 
	virtual void			Create();

							//! 
	virtual bool			CheckCollision(
								CLasagneEntity *other
							);

							//!
	virtual void			Fire(
								CLasagneEntity *gun,
								ShotgunBulletDirection::Enum direction
							);

							//! The power of the bullet (represents the damage done to an enemy on contact)
	virtual int				GetPower() const
							{
								return 100;
							}
};

#endif