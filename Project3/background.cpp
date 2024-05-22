#include "background.h"

static int backgroundX;
static Star stars[MAX_STARS];
static SDL_Texture* background;
static SDL_Texture* init_background;
static SDL_Texture* backgroundMenu;

//void initFirstImage(void)
//{
//	// Tải texture cho ảnh đầu tiên
//	init_background = loadTexture("gfx/init_background.png");
//
//	// Đảm bảo texture đã được tải thành công
//	if (init_background == NULL)
//	{
//		printf("Unable to load first image: %s\n", SDL_GetError());
//		return;
//	}
//
//	// Hiển thị ảnh đầu tiên
//	SDL_Rect dest = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; // Đặt kích thước và vị trí mong muốn cho ảnh
//	SDL_RenderCopy(app.renderer, init_background, NULL, &dest); // Vẽ ảnh đầu tiên lên màn hình
//
//	// Đợi một khoảng thời gian trước khi chuyển tiếp (tùy chọn)
//	SDL_Delay(200000); // Đợi 2000 milliseconds (2 giây)
//}
void initBackground(void)
{
	background = loadTexture("gfx/background_02.png");

	backgroundX = 0;
}

void initStarfield(void)
{
	int i;

	for (i = 0; i < MAX_STARS; i++)
	{
		stars[i].x = rand() % SCREEN_WIDTH;
		stars[i].y = rand() % SCREEN_HEIGHT;
		stars[i].speed = 1 + rand() % 8;
	}
}

void doBackground(void)
{
	if (--backgroundX < -SCREEN_WIDTH)
	{
		backgroundX = 0;
	}
}

void doStarfield(void)
{
	int i;

	for (i = 0; i < MAX_STARS; i++)
	{
		stars[i].x -= stars[i].speed;

		if (stars[i].x < 0)
		{
			stars[i].x = SCREEN_WIDTH + stars[i].x;
		}
	}
}

void drawStarfield(void)
{
	int i, c;

	for (i = 0; i < MAX_STARS; i++)
	{
		c = 32 * stars[i].speed;

		SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);

		SDL_RenderDrawLine(app.renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y);
	}
}

void drawBackground(void)
{

	SDL_Rect dest;
	for (int x = backgroundX; x < SCREEN_WIDTH; x += SCREEN_WIDTH)
	{
		dest.x = x;
		dest.y = 0;
		dest.w = SCREEN_WIDTH;
		dest.h = SCREEN_HEIGHT;
		SDL_RenderCopy(app.renderer, background, NULL, &dest);
	}
}


void choseBackground(const char* g)
{
	backgroundX = 0;
	backgroundMenu = loadTexture(g); // Tải texture mới

	// Cập nhật vị trí của background
	if (--backgroundX < -SCREEN_WIDTH)
	{
		backgroundX = 0;
	}

	SDL_Rect dest;
	for (int x = backgroundX; x < SCREEN_WIDTH; x += SCREEN_WIDTH)
	{
		dest.x = x;
		dest.y = 0;
		dest.w = SCREEN_WIDTH;
		dest.h = SCREEN_HEIGHT;
		SDL_RenderCopy(app.renderer, backgroundMenu, NULL, &dest);
	}
}

void renderTexture(SDL_Texture* tex, SDL_Renderer* renderer, int x, int y) {
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, tex, NULL, &dst);
}
void start_game()
{
	const char* s = "gfx/init_background.jpg";
	choseBackground(s);
	SDL_Event event;
	bool running = true;
	int i = 1; // Increment for loading bar width

	// Load the texture once outside the loop

	SDL_Texture* loadBar = loadTexture("gfx/thanh_load.jpg");
	int loadBarX = 300;
	int loadBarY = 650;

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
		}

		// Define the portion of the load bar to display
		SDL_Rect srcRect = { 0, 0, 123, 50 }; // Consider showing a part of the load bar
		SDL_Rect destRect = { loadBarX, loadBarY, 10 * i, 20 }; // Position and size of the load bar on the window

		// Clear the screen (necessary to avoid visual artifacts)
		SDL_RenderClear(app.renderer);

		// Draw the background again for each frame
		//drawBackground();
		choseBackground(s);
		// Draw the selected part of the load bar
		SDL_RenderCopy(app.renderer, loadBar, &srcRect, &destRect);

		SDL_RenderPresent(app.renderer);

		SDL_Delay(50); // Add a delay to let the user see the load bar update
		i++; // Increase the control variable for load bar length
		if (i > 70) running = false; // Reset when reaching the limit
	}

	// Clean up the texture after the loop
	SDL_DestroyTexture(loadBar);
}