#ifndef PICKUPMACHINEGUN_H_INCLUDED
#define PICKUPMACHINEGUN_H_INCLUDED

#include "pickupbase.h"

class CPickupMachineGun : public CPickupBase
{
private:

public:
											//! Class constructor
											CPickupMachineGun(
												CPlayer *player
											);

											//! Class destructor
											~CPickupMachineGun();

											//! Called when a pickup is picked up
		virtual void						OnPickup();
};

#endif
