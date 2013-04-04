#ifndef LASAGNE_DEBUGSTATS_HEADER
#define LASAGNE_DEBUGSTATS_HEADER

#include <vector>
#include <ProFy.h>
#include <SDL.h>
#include <SDL_gfxPrimitives.h>

class CLasagneDebugStats {

private:

	std::vector<ProFy::TimerID>			m_timers;

public:
										//! Class constructor
										CLasagneDebugStats();

										//! Class destructor
										~CLasagneDebugStats();

										//!
	void								AddTimer(
											ProFy::TimerID timer							//!<
										);

										//!
	void								Render(
											SDL_Surface *surface						   //!< The surface to blit our text surface too
										);

};

#endif
