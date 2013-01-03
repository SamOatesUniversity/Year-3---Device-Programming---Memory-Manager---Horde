#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "../lasagne/lasagne.h"

#include "guns/gunbase.h"
#include "guns/gunpistol.h"

#include <map>

class CPlayer {

private:

    CLasagneEntity                          *m_entity;
	CGunBase								*m_gun;

public:

                            //! Class constructor
                            CPlayer();

                            //! Class destructor
                            ~CPlayer();

                            //! Load the player graphics
    const bool              Load(
                                const char* spriteSheet
                            );


    CLasagneEntity           *GetEntity()
                            {
                                return m_entity;
                            }

	void					SetRotation(
								const int alpha
							);

	void					SetCurrentAnimation(
								char *animation
							);

	void					Update();

};

#endif // PLAYER_H_INCLUDED
