#ifndef GUNSHOTGUN_H_INCLUDED
#define GUNSHOTGUN_H_INCLUDED

#include "gunbase.h"
#include "bulletshotgun.h"

class CGunShotgun : public CGunBase
{
public:
									//!
									CGunShotgun();

									//!
									~CGunShotgun();

									//! 
	virtual void					Create();

									//!
    virtual int 					Shoot(
										std::vector<CEnemyBase*> &enemy
									);
};

#endif // GUNSHOTGUN_H_INCLUDED