#include<string>
#include "stdlib.h"
#include "stdio.h"
#include <string>
#include "math.h"
#include "ctype.h"
#include<vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "defs.h"
#include "structs.h"
#include<fstream>

//SDL_Texture* background;
extern void choseBackground(const char* g);
//extern SDL_Texture* loadTexture(const char* filename);
extern SDL_Texture* loadTexture(const char* filename);
extern void drawBackground(void);
extern void drawText(int x, int y, int r, int g, int b, int align, const char* format, ...);