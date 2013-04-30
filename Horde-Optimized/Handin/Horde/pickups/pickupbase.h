#ifndef PICKUPBASE_H_INCLUDED
#define PICKUPBASE_H_INCLUDED

#include "../../lasagne/lasagne.h"
#include "../player.h"

class CPickupBase {

protected:

	CLasagneEntity						*m_entity;
	CPlayer								*m_player;

public:
										//! Class constructor
										CPickupBase(
											CPlayer *player
										);

										//! Class destructor
										~CPickupBase();

										//! Drop this pickup
	virtual void						Drop(
											const float x,
											const float y
										);

										//! Update the pickup
	virtual void						Update(
											int backGroundX,
											int backGroundY
										);

										//! Called when a pickup is picked up
	virtual void						OnPickup() = 0;
};

#endif
