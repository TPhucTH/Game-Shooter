#pragma once

#include "common.h"

extern void cleanup(void);
extern void doInput(void);
extern void initGame(void);
extern void initSDL(void);
extern void initTitle(void);
extern void prepareScene(void);
extern void presentScene(void);

extern int Setup_Sound();
extern int init_menu();
extern int Let_Guide();
extern void start_game();
extern void loadMusic(const char* filename);
extern void playMusic(int loop);

App app;
Highscores highscores;
Stage stage;

