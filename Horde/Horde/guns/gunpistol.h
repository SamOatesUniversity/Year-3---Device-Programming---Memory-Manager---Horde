#ifndef GUNPISTOL_H_INCLUDED
#define GUNPISTOL_H_INCLUDED

#include "gunbase.h"
#include "bulletpistol.h"

class CGunPistol : public CGunBase
{
public:
									//!
									CGunPistol();

									//!
									~CGunPistol();

									//! 
	virtual void					Create();

									//!
    virtual int 					Shoot(
										std::vector<CEnemyBase*> &enemy
									);
};

#endif // GUNPISTOL_H_INCLUDED
