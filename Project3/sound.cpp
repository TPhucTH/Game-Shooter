#include "sound.h"

static void loadSounds(void);

std::vector<const char*> soundPaths = {
	"sound/334227__jradcoolness__laser.ogg",
	"sound/196914__dpoggioli__laser-gun.ogg",
	"sound/245372__quaker540__hq-explosion.ogg",
	"sound/10_Guage_Shotgun-SoundBible.com-74120584.ogg",
	"sound/mixkit-space-coin-win-notification-271.wav"
};
#define SND_MAX 5
#define SND_PLAYER_FIRE 0
#define SND_ALIEN_FIRE 1
#define SND_PLAYER_DIE 2
#define SND_ALIEN_DIE 3
#define SND_POINTS 4

static Mix_Chunk* sounds[SND_MAX];
static Mix_Music* music;

void initSounds(void) 
{
	memset(sounds, 0, sizeof(Mix_Chunk*) * SND_MAX);

	music = NULL;

	loadSounds();
}

void loadMusic(const char *filename)
{
	if (music != NULL) 
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}

	music = Mix_LoadMUS(filename);
}

void playMusic(int loop)
{
	Mix_PlayMusic(music, (loop) ? -1 : 0);
}

void playSound(int id, int channel) 
{
	Mix_PlayChannel(channel, sounds[id], 0);
}

static void loadSounds(void)
{
	for (int i = 0; i < SND_MAX; ++i)
	{
		sounds[i] = Mix_LoadWAV(soundPaths[i]);
	}
}