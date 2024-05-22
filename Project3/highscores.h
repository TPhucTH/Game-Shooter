#pragma once

#include "common.h"

extern void doBackground(void);
extern void doStarfield(void);
extern void drawBackground(void);
extern void drawStarfield(void);
extern void drawText(int x, int y, int r, int g, int b, int align, const char *format, ...);
extern void initStage(void);
extern void initTitle(void);

extern App app;
extern Highscores highscores;

void initHighscoreTable(void);
void initHighscores(void);
void addHighscore(int score);
void loadHighscores(void);
void saveHighscores(void);  // Ensure this line is present