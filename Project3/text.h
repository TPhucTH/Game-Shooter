#pragma once

#include "common.h"

extern void blitRect(SDL_Texture* texture, SDL_Rect* src, int x, int y);
extern SDL_Texture* loadTexture(const char* filename);

void initFonts(SDL_Renderer* rend);
void drawText(int x, int y, int r, int g, int b, int align, const char* format, ...);
void cleanupFonts(void);
