
#include "init.h"

TTF_Font* g_font_text = NULL;
TTF_Font* g_font_MENU = NULL;

void initSDL(void)
{
	int rendererFlags, windowFlags;

	rendererFlags = SDL_RENDERER_ACCELERATED;

	windowFlags  = SDL_WINDOW_FULLSCREEN_DESKTOP; // Thay vì 0, để kích hoạt chế độ toàn màn hình giả
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	if (TTF_Init() == -1) {
		printf("Couldn't initialize SDL TTF: %s\n", TTF_GetError());
		exit(1);
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) 
	{
		printf("Couldn't initialize SDL Mixer\n");
		exit(1);
	}
	g_font_text = TTF_OpenFont("font/ARCADE.ttf", 38);
	if (g_font_text == NULL)
	{
		printf("Failed to load font: %s\n", TTF_GetError());
		exit(1);
	}

	g_font_MENU = TTF_OpenFont("font/ARCADE.ttf", 80);
	if (g_font_MENU == NULL)
	{
		printf("Failed to load font: %s\n", TTF_GetError());
		exit(1);
	}
	Mix_AllocateChannels(MAX_SND_CHANNELS);

	app.window = SDL_CreateWindow("Jet Fighter", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

	app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);
	

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	SDL_RenderSetLogicalSize(app.renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	SDL_ShowCursor(0);

	//SDL_SetWindowFullscreen(app.window, SDL_TRUE);
}

void initGame(void) 
{
	/*initFirstImage();*/

	initBackground();

	initStarfield();

	initSounds();

	initFonts();

	initHighscoreTable();
	
	/* loadMusic("music/UndertaleMegalovania.mp3");*/
	loadMusic("music/TheFatFat.mp3");

	playMusic(1);
}
int init_menu() {
	const std::vector<std::string>& options = { "Play game ","Guide ","Change Music","Exit" };
	return showMenu(app.renderer, g_font_MENU, "Main menu", options);
}
int Let_Guide()
{
	return Guide(app.renderer, g_font_text, "Guide");
}
int Setup_Sound()
{
	return Setting_sound(app.renderer, g_font_text, "Setting Sound");
}
void cleanup(void)
{
	SDL_DestroyRenderer(app.renderer);

	SDL_DestroyWindow(app.window);

	SDL_Quit();
}
