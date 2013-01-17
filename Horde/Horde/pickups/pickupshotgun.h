#ifndef PICKUPSHOTGUN_H_INCLUDED
#define PICKUPSHOTGUN_H_INCLUDED

#include "pickupbase.h"

class CPickupShotgun : public CPickupBase 
{
private:

public:
											//! Class constructor
											CPickupShotgun(
												CPlayer *player	
											);

											//! Class destructor
											~CPickupShotgun();

											//! Called when a pickup is picked up
		virtual void						OnPickup();
};

#endif