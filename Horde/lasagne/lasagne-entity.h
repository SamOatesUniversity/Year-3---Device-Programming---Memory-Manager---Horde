#ifndef LASAGNE_ENTITY_H_INCLUDED
#define LASAGNE_ENTITY_H_INCLUDED

#include "../SOGI/SOGI.h"
#include "lasagne-surface.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>

#include <map>

class CLasagneEntity {
private:

    SDL_Surface                             *m_image;                       //!<
    SDL_Surface                             *m_frame;

    TVector<float, 2>                       m_screenPosition;               //!<
    int                                     m_rotation;                     //!<

    int                                     m_noofFramesX;
    int                                     m_noofFramesY;
    int                                     m_currentFrame;
    Uint32                                  m_lastFrameTime;
    std::string                             m_currentAnimation;

    typedef TVector<int, 2>                 IVec2;
    std::map<std::string, IVec2>            m_animation;

    IVec2                                   m_frameSize;

public:
                            //!
                            CLasagneEntity(
                                const char* imagePath,
                                const TVector<int, 2> screenSize
                            );

                            //!
                            CLasagneEntity(
                                const char* imagePath,
                                const TVector<int, 2> screenSize,
                                const TVector<int, 2> noofFrames
                            );

                            //!
                            ~CLasagneEntity();

                            //!
    void                    Render(
                                SDL_Surface *screen
                            );

    TVector<float, 2>       GetPosition() const
                            {
                                return m_screenPosition;
                            }

    void                    SetPosition(
                                const float x,
                                const float y
                            )
                            {
                                m_screenPosition.Set(x, y);
                            }

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
                                const int frameEnd              //!<
                            );

                            //!
    void                    SetCurrentAnimation(
                                char *animation
                            );

};

#endif // LASAGNE_ENTITY_H_INCLUDED
