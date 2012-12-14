#ifndef BULLETBASE_H_INCLUDED
#define BULLETBASE_H_INCLUDED

class CBulletBase {

protected:

public:
							//! 
							CBulletBase();

							//! 
							~CBulletBase();

							//! 
	virtual bool			CheckCollision(
		
							);

							//!
	virtual void			Render();

};

#endif