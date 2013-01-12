#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

#include "../lasagne/lasagne.h"
#include <string>

class CScene
{
private:

	#define				NOOF_BACKGROUNDS	9
    CLasagneEntity      *m_background[NOOF_BACKGROUNDS];

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
    bool                Move(
                            const int x,
                            const int y
                        );
};

#endif // SCENE_H_INCLUDED
