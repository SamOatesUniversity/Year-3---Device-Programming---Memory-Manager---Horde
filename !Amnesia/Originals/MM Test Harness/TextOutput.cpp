#include "TextOutput.h"

// Top 2 are status lines

const int kLeftEdge=4;
const int kGapBetweenLines=10;

const SDL_Color black={0,0,0};
const SDL_Color white={255,255,255};
const SDL_Color blue={0,0,255};
const SDL_Color red={255,0,0};
const SDL_Color statusColour={255,100,255};

CTextOutput::CTextOutput() : m_textSurfaces(0), m_font(0), m_numLines(0), m_nextLine(2), m_ystart(0)
{
    //ctor
}

CTextOutput::~CTextOutput()
{
    TTF_CloseFont(m_font);

    for (int i=0;i<m_numLines;i++)
        SDL_FreeSurface(m_textSurfaces[i]);
    delete []m_textSurfaces;
}

bool CTextOutput::Initialise(int numLines,int startLineNumber,const std::string &titleText)
{
    m_numLines=numLines;
    m_ystart=startLineNumber*kGapBetweenLines;

    m_textSurfaces=new SDL_Surface*[m_numLines];
    memset(m_textSurfaces,0,sizeof(SDL_Surface*)*m_numLines);

	/* Initialize the TTF library */
	if ( TTF_Init() < 0 ) {
		fprintf(stderr, "Couldn't initialize TTF: %s\n",SDL_GetError());
		return false;
	}

    // Open font
    m_font= TTF_OpenFont("ariblk.ttf",10);
	if ( m_font == NULL ) {
		fprintf(stderr, "Couldn't load font %s\n",SDL_GetError());
		return false;
	}
	//TTF_SetFontStyle(font, renderstyle);
	//TTF_SetFontOutline(font, outline);
	//TTF_SetFontKerning(font, kerning);

    TTF_SetFontStyle(m_font, TTF_STYLE_UNDERLINE);
	m_textSurfaces[0]=RenderToSurface(titleText.c_str(),black);
    TTF_SetFontStyle(m_font, TTF_STYLE_NORMAL);

	return true;
}

void CTextOutput::SetStatusText(const std::string &status)
{
    SDL_FreeSurface(m_textSurfaces[1]);

   // TTF_SetFontStyle(m_font, TTF_STYLE_BOLD);
    m_textSurfaces[1]=RenderToSurface(status,statusColour);
    //TTF_SetFontStyle(m_font, TTF_STYLE_NORMAL);
}

void CTextOutput::AddLine(const std::string &line, bool fromMM)
{
    SDL_FreeSurface(m_textSurfaces[m_nextLine]);

    if (fromMM)
        m_textSurfaces[m_nextLine]=RenderToSurface(line,black);
    else
        m_textSurfaces[m_nextLine]=RenderToSurface(line,blue);

    m_nextLine++;
    if (m_nextLine>=m_numLines)
        m_nextLine=2;
}

SDL_Surface * CTextOutput::RenderToSurface(const std::string &text,const SDL_Color &colour)
{
    SDL_Surface *textSurface=TTF_RenderText_Solid(m_font, text.c_str(), colour);
    //if (textSurface==0)
    return textSurface;
}

void CTextOutput::Render(SDL_Surface *screen)
{
    int yPosition=m_ystart;

    // Draw title and status first
    for (int i=0;i<2;i++)
    {
        if ( m_textSurfaces[i] != NULL )
        {
            SDL_Rect dstrect;
            dstrect.x = kLeftEdge;
            dstrect.y = yPosition;
            dstrect.w = m_textSurfaces[i]->w;
            dstrect.h = m_textSurfaces[i]->h;
            SDL_BlitSurface(m_textSurfaces[i], NULL, screen, &dstrect);
        }

        yPosition+=kGapBetweenLines;
    }

    int line=m_nextLine; // oldest line drawn first

    for (int i=2;i<m_numLines;i++)
    {
        if (line>=m_numLines)
            line=2;

        if ( m_textSurfaces[line] != NULL )
        {
            SDL_Rect dstrect;
            dstrect.x = kLeftEdge;
            dstrect.y = yPosition;
            dstrect.w = m_textSurfaces[line]->w;
            dstrect.h = m_textSurfaces[line]->h;
            SDL_BlitSurface(m_textSurfaces[line], NULL, screen, &dstrect);
        }

        line++;
        yPosition+=kGapBetweenLines;
    }

}

