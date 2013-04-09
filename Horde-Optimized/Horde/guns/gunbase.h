#ifndef GUNBASE_H_INCLUDED
#define GUNBASE_H_INCLUDED

#include <vector>
#include "bulletbase.h"
#include "../../lasagne/lasagne.h"

class CGunBase {
protected:

	std::vector<CBulletBase*>		m_bullet;					//!< 
	unsigned int					m_nextBullet;				//!< 
	unsigned int					m_noofBullets;				//!< 

	CLasagneEntity                  *m_entity;					//!<
	CLasagneAudioFile				*m_audio;

	unsigned int					m_fireRate;					//!< How many bullets can be shot per second
	Uint32							m_lastShot;					//!<

public:
									//!
									CGunBase();

									//!
	virtual							~CGunBase();

	CLasagneEntity					*GetEntity() const 
									{
										return m_entity;
									}

									//! 
	virtual void					Create() = 0;

									//!
    virtual void					Shoot();

									//!
    virtual int						Update(
										std::vector<CEnemyBase*> &enemy
									);

									//! 
	virtual unsigned int			GetNoofBullets() const
									{
										return m_noofBullets;
									}

	virtual void					SetNoofBullets(
										unsigned int noofBullets
									)
									{
										m_noofBullets = noofBullets;

										CLasagne *const engine = CLasagne::GetInstance();
										for (unsigned int bulletIndex = 0; bulletIndex < m_bullet.size(); ++bulletIndex) 
										{
											CLasagneEntity *bullet = m_bullet[bulletIndex]->GetEntity();
											engine->EnableEntity(&bullet);
										}
									}

};

#endif // GUNBASE_H_INCLUDED
