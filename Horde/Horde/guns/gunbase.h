#ifndef GUNBASE_H_INCLUDED
#define GUNBASE_H_INCLUDED

#include <vector>
#include "bulletbase.h"

class CGunBase {
protected:

	std::vector<CBulletBase>		m_bullet;

public:
									//!
									CGunBase() {};

									//!
									~CGunBase() {};

									//!
    virtual void					Shoot();

									//!
    virtual void					Render();

};

#endif // GUNBASE_H_INCLUDED
