﻿#include "stage.h"

static void logic(void);
static void draw(void);
static void initPlayer(void);
static void fireBullet(void);
static void doPlayer(void);
static void doFighters(void);
static void doBullets(void);
static void drawFighters(void);
static void drawBullets(void);
static void spawnEnemies(void);
static int bulletHitFighter(Entity *b);
static void doEnemies(void);
static void fireAlienBullet(Entity* e);
static void clipPlayer(void);
static void resetStage(void);
static void drawExplosions(void);
static void doExplosions(void);
static void addExplosions(int x, int y, int num);
static void addDebris(Entity* e);
static void doDebris(void);
static void drawDebris(void);
static void drawHud(void);
static void doPointsPods(void);
static void drawPointsPods(void);
static void addPointsPod(int x, int y);

static Entity* player;
static SDL_Texture* bulletTexture;
static SDL_Texture* enemyTexture;
static SDL_Texture* playerTexture;
static SDL_Texture* alienBulletTexture;
static SDL_Texture* explosionTexture;
static SDL_Texture* pointsTexture;
static int enemySpawnTimer;
static int stageResetTimer;

std::vector<const char*> TexturePath = {
	"gfx/enemy.png","gfx/enemy_game.png","gfx/playerBullet.png","gfx/01.png","gfx/play_game.png",
	"gfx/explosion.png","gfx/points.png"
};
void initStage(void) 
{
	app.delegate.logic = logic;
	app.delegate.draw = draw;

	memset(&stage, 0, sizeof(Stage));
	stage.fighterTail = &stage.fighterHead;
	stage.bulletTail = &stage.bulletHead;
	stage.explosionTail = &stage.explosionHead;
	stage.debrisTail = &stage.debrisHead;
	stage.pointsTail = &stage.pointsHead;

	int i = rand() % 2;
	bulletTexture = loadTexture(TexturePath[2]);
	enemyTexture = loadTexture(TexturePath[i]);
	alienBulletTexture = loadTexture(TexturePath[3]);
	playerTexture = loadTexture(TexturePath[4]);
	explosionTexture = loadTexture(TexturePath[5]);
	pointsTexture = loadTexture(TexturePath[6]);

	memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);

	resetStage();

	stage.score = 0;

	initPlayer();

	enemySpawnTimer = 0;

	stageResetTimer = FPS * 3;
}

static void resetStage(void)
{
	Entity* e;
	Explosion* ex;
	Debris* d;

	while (stage.fighterHead.next) 
	{
		e = stage.fighterHead.next;
		stage.fighterHead.next = e->next;
		free(e);
	}

	while (stage.bulletHead.next) 
	{
		e = stage.bulletHead.next;
		stage.bulletHead.next = e->next;
		free(e);
	}

	while (stage.explosionHead.next)
	{
		ex = stage.explosionHead.next;
		stage.explosionHead.next = ex->next;
		free(ex);
	}

	while (stage.debrisHead.next)
	{
		d = stage.debrisHead.next;
		stage.debrisHead.next = d->next;
		free(d);
	}
	
	while (stage.pointsHead.next) 
	{
		e = stage.pointsHead.next;
		stage.pointsHead.next = e->next;
		free(e);
	}

	stage.fighterTail = &stage.fighterHead;
	stage.bulletTail = &stage.bulletHead;
	stage.explosionTail = &stage.explosionHead;
	stage.debrisTail = &stage.debrisHead;
	stage.pointsTail = &stage.pointsHead;
}

static void initPlayer() 
{
	player = new Entity;
	memset(player, 0, sizeof(Entity));
	stage.fighterTail->next = player;
	stage.fighterTail = player;

	player->health = 3;
	player->lives = 3; // Đặt số mạng ban đầu
	player->x = 100;
	player->y = 100;
	player->texture = playerTexture;
	SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);

	player->side = SIDE_PLAYER;
}

static void logic(void) 
{
	doBackground();

	doStarfield();

	doPlayer();

	doEnemies();

	doFighters();

	doBullets();

	doExplosions();

	doDebris();

	doPointsPods();

	spawnEnemies();

	clipPlayer();

	if (player == NULL && --stageResetTimer <= 0)
	{
		addHighscore(stage.score);

		initHighscores();
	}
}

static void doPlayer(void) 
{
	if (player != NULL) 
	{
		player->dx = player->dy = 0;

		if (player->reload > 0)
		{
			player->reload--;
		}

		if (app.keyboard[SDL_SCANCODE_UP])
		{
			player->dy = -PLAYER_SPEED;
		}

		if (app.keyboard[SDL_SCANCODE_DOWN])
		{
			player->dy = PLAYER_SPEED;
		}

		if (app.keyboard[SDL_SCANCODE_LEFT])
		{
			player->dx = -PLAYER_SPEED;
		}

		if (app.keyboard[SDL_SCANCODE_RIGHT])
		{
			player->dx = PLAYER_SPEED;
		}
		if (app.keyboard[SDL_SCANCODE_SPACE] && player->reload <= 0)
		{
			playSound(SND_PLAYER_FIRE, CH_PLAYER);

			fireBullet();
		}
		if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload <= 0)
		{
			playSound(SND_PLAYER_FIRE, CH_PLAYER);

			fireBullet();
		}
		if (app.keyboard[SDL_SCANCODE_ESCAPE])
		{
			exit(1);
		}
	}
}

static void fireBullet(void) 
{
	Entity* bullet;

	bullet = new Entity;
	memset(bullet, 0, sizeof(Entity));
	stage.bulletTail->next = bullet;
	stage.bulletTail = bullet;

	bullet->x = player->x;
	bullet->y = player->y;
	bullet->dx = PLAYER_BULLET_SPEED;
	bullet->health = 1;
	bullet->texture = bulletTexture;
	bullet->side = player->side;
	SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

	bullet->y += (player->h / 2) - (bullet->h / 2);

	bullet->side = SIDE_PLAYER;

	player->reload = 8;
}

static void doEnemies (void)
{
	Entity* e;

	for (e = stage.fighterHead.next; e != NULL; e = e->next) 
	{
		if (e != player)
		{
			e->y = MIN(MAX(e->y, 0), SCREEN_HEIGHT - e->h);

			if (player != NULL && --e->reload <= 0)
			{
				fireAlienBullet(e);

				playSound(SND_ALIEN_FIRE, CH_ALIEN_FIRE);
			}
		}
	}
}

static void fireAlienBullet (Entity *e) 
{
	Entity* bullet;

	bullet = new Entity;
	memset(bullet, 0, sizeof(Entity));
	stage.bulletTail->next = bullet;
	stage.bulletTail = bullet;

	bullet->x = e->x;
	bullet->y = e->y;
	bullet->health = 1;
	bullet->texture = alienBulletTexture;
	bullet->side = e->side;
	SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);
	
	bullet->x += (e->w / 2) - (bullet->w / 2);
	bullet->y += (e->h / 2) - (bullet->h / 2);

	calcSlope(player->x + (player->w / 2), player->y + (player->h / 2), e->x, e->y, &bullet->dx, &bullet->dy);
	
	bullet->dx *= ALIEN_BULLET_SPEED;
	bullet->dy *= ALIEN_BULLET_SPEED;

	bullet->side = SIDE_ALIEN;

	e->reload = (rand() % FPS * 2);
}

static void doFighters(void) 
{
	Entity* e, * prev;

	prev = &stage.fighterHead;

	for (e = stage.fighterHead.next; e != NULL; e = e->next)
	{
		e->x += e->dx;
		e->y += e->dy;

		if (e != player && e->x < -e->w) 
		{
			e->health = 0;
		}

		if (e->health == 0) 
		{
			if (e == player) 
			{
				player = NULL;
			}

			if (e == stage.fighterTail)
			{
				stage.fighterTail = prev;
			}

			prev->next = e->next;
			free(e);
			e = prev;
		}

		prev = e;
	}
}

static void doBullets(void) 
{
	Entity* b, * prev;

	prev = &stage.bulletHead;

	for (b = stage.bulletHead.next; b != NULL; b = b->next) 
	{
		b->x += b->dx;
		b->y += b->dy;

		if (bulletHitFighter(b) || b->x < -b->w || b->y < -b->h ||  b->x > SCREEN_WIDTH || b->y > SCREEN_HEIGHT)
		{
			if (b == stage.bulletTail) 
			{
				stage.bulletTail = prev;
			}

			prev->next = b->next;
			free(b);
			b = prev;
		}

		prev = b;
	}
}

static int bulletHitFighter(Entity* b)
{
	Entity* e;

	for (e = stage.fighterHead.next; e != NULL; e = e->next)
	{
		if (e->side != b->side && collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h))
		{
			b->health = 0;

			if (e == player)
			{
				if (player->lives > 0)
				{
					player->lives--; 
					if (player->lives == 0)
					{
						player->health = 0;
					}
				}

				playSound(SND_PLAYER_DIE, CH_PLAYER);
			}
			else
			{
				e->health = 0;
				addPointsPod(e->x + e->w / 2, e->y + e->h / 2);
				playSound(SND_ALIEN_DIE, CH_ANY);
			}

			addExplosions(e->x, e->y, 32);
			addDebris(e);

			return 1;
		}
	}

	return 0;
}


static void spawnEnemies(void)
{
	Entity* enemy;

	if (--enemySpawnTimer <= 0) 
	{
		enemy = new Entity;
		memset(enemy, 0, sizeof(Entity));
		stage.fighterTail->next = enemy;
		stage.fighterTail = enemy;

		enemy->x = SCREEN_WIDTH;
		enemy->y = rand() % SCREEN_HEIGHT;
		enemy->texture = enemyTexture;
		SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);

		enemy->dx = -(2 + (rand() % 4));
		enemy->dy = -100 + (rand() % 200);
		enemy->dy /= 100;
		
		enemy->side = SIDE_ALIEN;
		enemy->health = 1;

		enemy->reload = FPS * (1 + (rand() % 3));
		
		enemySpawnTimer = 30 + (rand() % FPS);
	}
}

static void clipPlayer (void)
{
	if (player != NULL) 
	{
		if (player->x < 0)
		{
			player->x = 0;
		}

		if (player->y < 0)
		{
			player->y = 0;
		}

		if (player->x > SCREEN_WIDTH - player->w)
		{
			player->x = SCREEN_WIDTH - player->w;
		}

		if (player->y > SCREEN_HEIGHT - player->h)
		{
			player->y = SCREEN_HEIGHT - player->h;
		}
	}
}

static void doExplosions(void)
{
	Explosion* e, * prev;

	prev = &stage.explosionHead;

	for (e = stage.explosionHead.next; e!= NULL; e = e->next) 
	{
		e->x += e->dx;
		e->y += e->dy;

		if (--e->a <= 0)
		{
			if (e == stage.explosionTail)
			{
				stage.explosionTail = prev;
			}

			prev->next = e->next;
			free(e);
			e = prev;
		}

		prev = e;
	}
}

static void doDebris(void) 
{
	Debris* d, * prev;

	prev = &stage.debrisHead;

	for (d = stage.debrisHead.next; d != NULL; d = d->next)
	{
		d->x += d->dx;
		d->y += d->dy;

		d->dy += 0.5;

		if (--d->life <= 0)
		{
			if (d == stage.debrisTail)
			{
				stage.debrisTail = prev;
			}

			prev->next = d->next;
			free(d);
			d = prev;
		}

		prev = d;
	}
}

static void doPointsPods(void) 
{
	Entity* e, * prev;

	prev = &stage.pointsHead;

	for (e = stage.pointsHead.next; e != NULL; e = e->next)
	{
		if (e->x < 0)
		{
			e->x = 0;
			e->dx = -e->dx;
		}

		if (e->x + e->w > SCREEN_WIDTH)
		{
			e->x = SCREEN_WIDTH - e->w;
			e->dx = -e->dx;
		}

		if (e->y < 0)
		{
			e->y = 0;
			e->dy = -e->dy;
		}

		if (e->y + e->h > SCREEN_HEIGHT)
		{
			e->y = SCREEN_HEIGHT - e->h;
			e->dy = -e->dy;
		}

		e->x += e->dx;
		e->y += e->dy;

		if (player != NULL && collision(e->x, e->y, e->w, e->h, player->x, player->y, player->w, player->h))
		{
			e->health = 0;

			stage.score++;
			
			playSound(SND_POINTS, CH_POINTS);
		}

		if (--e->health <= 0)
		{
			if (e == stage.pointsTail)
			{
				stage.pointsTail = prev;
			}

			prev->next = e->next;
			free(e);
			e = prev;
		}

		prev = e;
	}
}

static void addExplosions(int x, int y, int num) 
{
	Explosion* e;
	int i;

	for (i = 0; i < num; i++) 
	{
		e = new Explosion;
		memset(e, 0, sizeof(Explosion));
		stage.explosionTail->next = e;
		stage.explosionTail = e;

		e->x = x + (rand() % 32) - (rand() % 32);
		e->y = y + (rand() % 32) - (rand() % 32);
		e->dx = (rand() % 10) - (rand() % 10);
		e->dy = (rand() % 10) - (rand() % 10);

		e->dx /= 10;
		e->dy /= 10;

		switch (rand() % 4) 
		{
			case 0:
				e->r = 255;
				break;

			case 1:
				e->r = 255;
				e->g = 128;
				break;

			case 2:
				e->r = 255;
				e->g = 255;
				break;

			default:
				e->r = 255;
				e->g = 255;
				e->b = 255;
				break;
		}

		e->a = rand() % FPS * 3;
	}
}

static void addDebris(Entity *e)
{
	Debris* d;
	int x, y, w, h;

	w = e->w / 2;
	h = e->h / 2;

	for (y = 0; y <= h; y += h)
	{
		for (x = 0; x <= w; x += w)
		{
			d = new Debris;
			memset(d, 0, sizeof(Debris));
			stage.debrisTail->next = d;
			stage.debrisTail = d;

			d->x = e->x + e->w / 2;
			d->y = e->y + e->h / 2;
			d->dx = (rand() % 5) - (rand() % 5);
			d->dy = -(5 + (rand() % 12));
			d->life = FPS * 2;
			d->texture = e->texture;

			d->rect.x = x;
			d->rect.y = y;
			d->rect.w = w;
			d->rect.h = h;
		}
	}
}

static void addPointsPod(int x, int y)
{
	Entity* e;

	e = new Entity;
	memset(e, 0, sizeof(Entity));
	stage.pointsTail->next = e;
	stage.pointsTail = e;

	e->x = x;
	e->y = y;
	e->dx = -(rand() % 5);
	e->dy = (rand() % 5) - (rand() % 5);
	e->health = FPS * 10;
	e->texture = pointsTexture;

	SDL_QueryTexture(e->texture, NULL, NULL, &e->w, &e->h);

	e->x -= e->w / 2;
	e->y -= e->h / 2;
}

static void draw(void) 
{
	drawBackground();

	drawStarfield();

	drawPointsPods();

	drawFighters();

	drawDebris();

	drawExplosions();

	drawBullets();

	drawHud();
}

static void drawPointsPods(void)
{
	Entity* e;

	for (e = stage.pointsHead.next; e != NULL; e = e->next)
	{
		if (e->health > (FPS *2) || e->health % 12 < 6)
		{
			blit(e->texture, e->x, e->y);
		}
	}
}

static void drawFighters(void) 
{
	Entity* e;

	for (e = stage.fighterHead.next; e != NULL; e = e->next) 
	{
		blit(e->texture, e->x, e->y);
	}
}

static void drawBullets(void) 
{
	Entity* b;

	for (b = stage.bulletHead.next; b != NULL; b = b->next)
	{
		blit(b->texture, b->x, b->y);
	}
}

static void drawDebris(void)
{
	Debris* d;

	for (d = stage.debrisHead.next; d != NULL; d = d->next)
	{
		blitRect(d->texture, &d->rect, d->x, d->y);
	}
}

static void drawExplosions(void)
{
	Explosion* e;

	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

	for (e = stage.explosionHead.next; e != NULL; e = e->next)
	{
		SDL_SetTextureColorMod(explosionTexture, e->r, e->g, e->b);
		SDL_SetTextureAlphaMod(explosionTexture, e->a);

		blit(explosionTexture, e->x, e->y);
	}

	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}

static void drawHud(void)
{
	drawText(10, 10, 255, 255, 255, TEXT_LEFT, "SCORE: %03d", stage.score);

	if (stage.score < highscores.highscore[0].score)
	{
		drawText(SCREEN_WIDTH - 10, 10, 255, 255, 255, TEXT_RIGHT, "HIGHSCORE: %03d", highscores.highscore[0].score);
	}
	else 
	{
		drawText(SCREEN_WIDTH - 10, 10, 0, 255, 0, TEXT_RIGHT, "HIGHSCORE: %03d", stage.score);
	}
}