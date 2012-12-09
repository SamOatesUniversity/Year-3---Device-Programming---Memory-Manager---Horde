#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

#include "../lasagne/lasagne.h"
#include <string>

class CScene
{
private:

    CLasagneEntity      *m_background[4];

public:
                        //! Class constructor
                        CScene();

                        //! Class destruct
                        ~CScene();

                        //! Load a scene
    const bool          Load(
                            const std::string& folderLocation                    //!< The location of the scene
                        );

                        //! Render the scene
    void                Render();

                        //! Move the player around the scene (technically move the scene keep player centered)
    void                Move(
                            const float x,
                            const float y
                        );
};

#endif // SCENE_H_INCLUDED
