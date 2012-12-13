#ifndef CTEXTOUTPUT_H
#define CTEXTOUTPUT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

// Handles text output to the screen
class CTextOutput
{
    private:

        SDL_Surface **m_textSurfaces;
        TTF_Font *m_font;
        int m_numLines;
        int m_nextLine;
        int m_ystart;

        SDL_Surface * RenderToSurface(const std::string &status,const SDL_Color &colour);
    public:
        bool Initialise(int numLines,int startLineNumber,const std::string &titleText);
        CTextOutput();
        ~CTextOutput();

        void SetStatusText(const std::string &status);
        void AddLine(const std::string &line,bool fromMM=false);
        void Render(SDL_Surface *screen);

};

#endif // CTEXTOUTPUT_H
