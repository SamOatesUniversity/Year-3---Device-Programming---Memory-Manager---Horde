#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "../lasagne/lasagne.h"
#include <map>

class CPlayer {

private:

    CLasagneEntity                          *m_entity;

public:

                        //! Class constructor
                        CPlayer();

                        //! Class destructor
                        ~CPlayer();

                        //! Load the player graphics
    const bool          Load(
                            const char* spriteSheet
                        );

};

#endif // PLAYER_H_INCLUDED
