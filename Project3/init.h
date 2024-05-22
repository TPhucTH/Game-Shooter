#pragma once

#include "common.h"
//extern void initFirstImage(void);
extern void initBackground(void);
extern void initFonts(void);
extern void initHighscoreTable(void);
extern void initSounds(void);
extern void initStarfield(void);
extern void loadMusic(const char* filename);
extern void playMusic(int loop);
extern  int showMenu(SDL_Renderer* renderer, TTF_Font* font, const std::string& title, const std::vector<std::string>& options);
extern int Guide(SDL_Renderer* renderer, TTF_Font* font, const std::string& title);
int Setting_sound(SDL_Renderer* renderer, TTF_Font* font, const std::string& title);

extern App app;


