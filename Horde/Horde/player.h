#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "../lasagne/lasagne.h"
#include "guns/gunbase.h"
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

};

#endif // PLAYER_H_INCLUDED
