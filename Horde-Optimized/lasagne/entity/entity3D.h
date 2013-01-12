#ifndef LASAGNE_ENTITY3D_HEADER
#define LASAGNE_ENTITY3D_HEADER

#include "../../SOGI/SOGITypes.h"

class CEntity3D {

protected:

    TVector<float, 3>           m_position;                 //!< The position of the entity

public:
                                //! Class constructor
                                CEntity3D();

                                //! Class destructor
                                ~CEntity3D();
};

#endif
