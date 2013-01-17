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
	virtual unsigned int			GetNoofBullets() const
									{
										return 1;	// infinite ammo
									}
};

#endif // GUNPISTOL_H_INCLUDED
