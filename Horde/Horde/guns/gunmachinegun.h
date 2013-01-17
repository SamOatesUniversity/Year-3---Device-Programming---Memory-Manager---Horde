#ifndef GUNMACHINEGUN_H_INCLUDED
#define GUNMACHINEGUN_H_INCLUDED

#include "gunbase.h"
#include "bulletmachinegun.h"

class CGunMachinegun : public CGunBase
{
public:
									//!
									CGunMachinegun();

									//!
									~CGunMachinegun();

									//! 
	virtual void					Create();
};

#endif // GUNMACHINEGUN_H_INCLUDED