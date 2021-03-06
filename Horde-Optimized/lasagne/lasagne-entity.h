#ifndef LASAGNE_ENTITY_H_INCLUDED
#define LASAGNE_ENTITY_H_INCLUDED

#include "../SOGI/SOGI.h"
#include "lasagne-surface.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>

#include <map>

union EntityFlags {
	struct {
		unsigned int animated : 1;
		unsigned int enabled : 1;
	};
	unsigned int allFlags;
};

class CLasagneEntity {
private:

    SDL_Surface                             *m_image;                       //!<
	std::string                             m_name;

    TVector<float, 2>                       m_screenPosition;               //!<
    int                                     m_rotation;                     //!<

    int                                     m_noofFramesX;
    int                                     m_noofFramesY;
    int                                     m_currentFrame;
    Uint32                                  m_lastFrameTime;
    std::string                             m_currentAnimation;

	unsigned int							m_fps;

	struct AnimationData {
		TVector<int, 2> frames;
		bool loop;
	};
    std::map<std::string, AnimationData>    m_animation;

    SDL_Rect								m_frameSize;
	bool									m_visible;
	int										m_depth;

	SDL_Surface								*m_rotateFrame;

	EntityFlags								m_flags;

public:
							//! 
							CLasagneEntity();

                            //!
                            CLasagneEntity(
                                const char* imagePath,
                                const TVector<int, 2> &screenSize
                            );

                            //!
                            CLasagneEntity(
                                const char* imagePath,
                                const TVector<int, 2> &screenSize,
                                const TVector<int, 2> &noofFrames
                            );

                            //!
                            ~CLasagneEntity();

                            //!
    void                    Render(
                                SDL_Surface *screen
                            );

							//! 
    TVector<float, 2>       GetPosition() const
                            {
                                return m_screenPosition;
                            }

							//! 
	TVector<int, 2>			GetFrameSize() const
							{
								TVector<int, 2> coord;
								coord.Set(m_frameSize.w, m_frameSize.h);
								return coord;
							}

							//! 
    TVector<float, 2>       GetDirection() const
                            {
								TVector<float, 2> direction;
								direction.Set(
									sin(static_cast<float>(m_rotation * 0.017f)),
									cos(static_cast<float>(m_rotation * 0.017f))
								);
                                return direction;
                            }

							//! 
    void                    SetPosition(
                                const float x,
                                const float y
                            )
                            {
                                m_screenPosition.Set(x, y);
                            }

							//! 
    void                    SetRotation(
                                const int alpha
                            )
                            {
                                m_rotation = alpha;
                            }

                            //!
    bool                    AddAnimation(
                                const char *name,               //!<
                                const int frameStart,           //!<
                                const int frameEnd,             //!<
								const bool loop = true			//!< 
                            );

                            //!
    void                    SetCurrentAnimation(
                                char *animation
                            );

							//! 
	SDL_Surface				*GetImage() const
							{
								return m_image;
							}

							//!
	void					SetVisible( 
								bool visible 
							);
		
							//! 
	const bool				IsVisible() const
							{
								return m_visible;
							}

							//!
	int						GetRotation() const
							{
								return m_rotation;
							}

							//! 
	const bool				Intersects(
								CLasagneEntity *other
							);

							//! Is the entity within the bounds of the screen
	bool					IsOnScreen() const;

							//! Set the animations frames perseconds
	void					SetFPS(
								unsigned int fps
							)
							{
								m_fps = 1000 / fps;
							}

							//! Get the name of the entity
	const std::string		GetName() const 
							{
								return m_name;
							}

							//! 
	const bool				IsEnabled() const 
							{
								return m_flags.enabled;
							}

							//! 
	void					SetEnabled(
									const bool enabled
								)
							{
								m_flags.enabled = enabled;
							}

	const unsigned int		GetDepth();

	void					SetDepth(unsigned int depth) { m_depth = depth; }
};

#endif // LASAGNE_ENTITY_H_INCLUDED
