#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "GraphicsResources.h"
#include <map>
#include <unordered_map>
#include <vector>

#include "Game.h"
#include "Maths.h"
#include "Textbox.h"
#include "UtilityClass.h"

#include <algorithm>
#include <optional>
#include <string>
#include <string_view>

#include "GraphicsUtil.h"
#include "Screen.h"

class Graphics {
  public:
	void init();
	~Graphics();

	GraphicsResources grphx;

	int bfontlen(uint32_t ch);
	int font_idx(uint32_t ch);
	int strwidth(std::string_view s);
	int strheight(std::string_view s);

	void Makebfont();
	void load_font(const char* path, SDL_Surface* img, int char_w, int char_h, int& pos);

	std::string assetdir;

	void drawhuetile(int x, int y, int t);
	void huetilesetcol(int t);
	Uint32 bigchunkygetcol(int t);

	void drawgravityline(int t);

	void MakeTileArray();

	void MakeSpriteArray();

	void maketelearray();

	void makecustomtilearray();

	void makecustomspritearray();

	std::vector<SDL_Surface*>* selectspritesheet();

	void drawcoloredtile(int x, int y, int t, int r, int g, int b);

	void drawmenu(int cr, int cg, int cb, bool levelmenu = false);

	void processfade();

	void drawfade();

	void setwarprect(int a, int b, int c, int d);

	void createtextbox(std::string t, int xp, int yp, int r = 255, int g = 255, int b = 255, bool allowspecial = false);

	void textboxcenter();

	void textboxcenterx(int centerline = 160);

	int textboxwidth();

	void textboxmove(int xo, int yo);

	void textboxmoveto(int xo);

	void textboxcentery(int centerline = 120);

	void textboxadjust();

	void addline(std::string t);

	void textboxtimer(int t);

	void textboxremove();

	void textboxremovefast();

	void textboxactive();

	void drawtextbox(int x, int y, int w, int h, int r, int g, int b);

	void drawpixeltextbox(int x, int y, int w, int h, int w2, int h2, int r, int g, int b, int xo, int yo);
	void drawcustompixeltextbox(int x, int y, int w, int h, int w2, int h2, int r, int g, int b, int xo, int yo);

	void drawcrewman(int x, int y, int t, bool act, bool noshift = false);

	int crewcolour(const int t);

	void cutscenebars();
	void cutscenebarstimer();

	void drawpartimage(int t, int xp, int yp, int wp, int hp);

	void drawscriptimage(Game& game, int t, int xp, int yp, bool cent = false, int alpha = 255, SDL_BlendMode blend = SDL_BLENDMODE_BLEND);

	void drawscriptimagemasked(Game& game, int t, int xp, int yp, int t2, int xp2, int yp2);

	void drawimage(int t, int xp, int yp, bool cent = false);

	void drawimagecol(int t, int xp, int yp, int r, int g, int b, bool cent = false);

	void updatetextboxes();
	void drawgui();

	void drawsprite(int x, int y, int t, int r, int g, int b);
	void drawsprite(int x, int y, int t, Uint32 c);

	void printcrewname(int x, int y, int t);

	void printcrewnamestatus(int x, int y, int t);

	void printcrewnamedark(int x, int y, int t);

	void map_tab(int opt, const std::string& text, bool selected = false);

	void map_option(int opt, int num_opts, const std::string& text, bool selected = false);

	bool Print(int _x, int _y, std::string _s, int r, int g, int b, bool cen = false);

	bool PrintAlpha(int _x, int _y, std::string _s, int r, int g, int b, int a, bool cen = false);

	void RPrint(int _x, int _y, std::string _s, int r, int g, int b, bool cen = false);

	void PrintOff(int _x, int _y, std::string _s, int r, int g, int b, bool cen = false);

	void PrintOffAlpha(int _x, int _y, std::string _s, int r, int g, int b, int a, bool cen = false);

	void bprint(int x, int y, std::string t, int r, int g, int b, bool cen = false);

	void bprintalpha(int x, int y, std::string t, int r, int g, int b, int a, bool cen = false);

	int len(std::string t);
	void bigprint(int _x, int _y, std::string _s, int r, int g, int b, bool cen = false, int sc = 2);
	void drawspritesetcol(int x, int y, int t, int c, int flipped = 0);

	void flashlight();
	void screenshake();
	void updatescreenshake();

	int screenshake_x;
	int screenshake_y;

	void render();
	void renderwithscreeneffects();

	bool Hitest(SDL_Surface* surface1, point p1, SDL_Surface* surface2, point p2);

	void drawentities();

	void drawtrophytext();

	void bigrprint(int x, int y, std::string& t, int r, int g, int b, bool cen = false, float sc = 2);

	void drawtele(int x, int y, int t, Uint32 c);

	void drawtelepart(int x, int y, int t, int c);

	Uint32 getRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	Uint32 getRGB(Uint8 r, Uint8 g, Uint8 b);

	Uint32 getBGR(Uint8 r, Uint8 g, Uint8 b);

	Uint32 getRGB(Uint32 _col);

	Uint32 RGBflip(Uint8 r, Uint8 g, Uint8 b);

	Uint32 RGBf(int r, int g, int b);

	void setcolreal(long long t);

	void drawbackground(int t);
	void updatebackground(int t);
	void drawtile3(int x, int y, int t, int off);
	void drawentcolours(int x, int y, int t);
	void drawtile3(int x, int y, int t);
	void drawtile2(int x, int y, int t);
	void drawtile(int x, int y, int t);
	void drawtowertile(int x, int y, int t);
	void drawtowertile3(int x, int y, int t, int off);

	void drawmap();

	void drawforetile(int x, int y, int t);

	void drawforetile2(int x, int y, int t);

	void drawforetile3(int x, int y, int t, int off);

	void drawrect(int x, int y, int w, int h, int r, int g, int b);

	void drawtowermap();

	void drawtowerspikes();

	bool onscreen(int t);

	void menuoffrender();

	void drawtowerbackground();
	void updatetowerbackground();

	void setcol(int t);
	void drawfinalmap();

	void reloadresources(bool fast = false);

	void textboxcreatefast();

	colourTransform ct = {0};

	int bcol, bcol2, rcol = 0;

	int m = 0;

	std::vector<SDL_Surface*> images;

	std::vector<SDL_Surface*> tele;
	std::vector<SDL_Surface*> tiles;
	std::vector<SDL_Surface*> tiles2;
	std::vector<SDL_Surface*> tiles3;
	std::vector<SDL_Surface*> entcolours;
	std::vector<SDL_Surface*> sprites;
	std::vector<SDL_Surface*> flipsprites;
	std::vector<SDL_Surface*> bfont;
	std::vector<SDL_Surface*> flipbfont;
	std::map<int, std::vector<SDL_Surface*>> customtiles;
	std::map<int, std::vector<SDL_Surface*>> customsprites;

	bool flipmode = false;
	bool setflipmode = false;
	bool notextoutline = false;
	point tl = {0};
	//buffer objects. //TODO refactor buffer objects
	SDL_Surface* backBuffer;
	Screen* screenbuffer;
	SDL_Surface* menubuffer;
	SDL_Surface* towerbuffer;
	SDL_Surface* towerbuffer_lerp;
	SDL_Surface* foregroundBuffer;
	SDL_Surface* tempBuffer;

	SDL_Rect bfont_rect = {0};
	SDL_Rect tiles_rect = {0};
	SDL_Rect sprites_rect = {0};
	SDL_Rect images_rect = {0};
	SDL_Rect bg_rect = {0};
	SDL_Rect line_rect = {0};
	SDL_Rect tele_rect = {0};

	SDL_Rect foot_rect = {0};
	SDL_Rect prect = {0};
	SDL_Rect footerrect = {0};
	SDL_Surface* footerbuffer;

	SDL_Surface* ghostbuffer;

	int linestate, linedelay = 0;
	int backoffset = 0;
	bool backgrounddrawn, foregrounddrawn = false;

	int menuoffset = 0;
	int oldmenuoffset = 0;
	bool resumegamemode = false;

	SDL_Rect warprect = {0};

	int crewframe = 0;
	int crewframedelay = 0;

	int fademode = 0;
	int fadeamount = 0;
	int oldfadeamount = 0;
	int fadebars[15];

	bool trinketcolset = false;
	int trinketr, trinketg, trinketb = 0;

	std::vector<textboxclass> textbox;

	bool showcutscenebars = false;
	int cutscenebarspos = 0;
	int oldcutscenebarspos = 0;

	static const int numstars = 50;
	SDL_Rect stars[numstars];
	int starsspeed[numstars];

	int spcol, spcoldel = 0;
	static const int numbackboxes = 18;
	SDL_Rect backboxes[numbackboxes];
	int backboxvx[numbackboxes];
	int backboxvy[numbackboxes];
	float backboxint[numbackboxes];

	int warpskip, warpfcol, warpbcol = 0;

	std::unordered_map<int, int> font_positions;
	std::optional<std::string> mapimage;

	bool noclear = false;
	bool translucentroomname = false;
	;

	bool showmousecursor = false;

	float inline lerp(const float v0, const float v1)
	{
		return v0 + alpha * (v1 - v0);
	}
	float alpha;

	Uint32 col_crewred = 0;
	Uint32 col_crewyellow = 0;
	Uint32 col_crewgreen = 0;
	Uint32 col_crewcyan = 0;
	Uint32 col_crewblue = 0;
	Uint32 col_crewpurple = 0; //actually pink
	Uint32 col_crewinactive = 0;
	Uint32 col_clock = 0;
	Uint32 col_trinket = 0;
	int col_tr = 0;
	int col_tg = 0;
	int col_tb = 0;
	void updatetitlecolours();

	bool kludgeswnlinewidth = false;

	Uint32 crewcolourreal(int t);

	SDL_BlendMode blendmode = SDL_BLENDMODE_BLEND;
};

extern Graphics graphics;

#endif /* GRAPHICS_H */
