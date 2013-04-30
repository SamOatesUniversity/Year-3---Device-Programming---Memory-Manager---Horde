#ifndef PICKUPHEALTH_H_INCLUDED
#define PICKUPHEALTH_H_INCLUDED

#include "pickupbase.h"

class CPickupHealth : public CPickupBase
{
private:

public:
											//! Class constructor
											CPickupHealth(
												CPlayer *player
											);

											//! Class destructor
											~CPickupHealth();

											//! Called when a pickup is picked up
		virtual void						OnPickup();
};

#endif
