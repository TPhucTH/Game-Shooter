
#include "input.h"

void doKeyDown(SDL_KeyboardEvent* event)
{
	
	if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS)
	{
		app.keyboard[event->keysym.scancode] = 1;
	}
}

void doKeyUp(SDL_KeyboardEvent* event)
{
	if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS)
	{
		app.keyboard[event->keysym.scancode] = 0;
	}
}

void doInput(void)
{
	SDL_Event event;

	memset(app.inputText, '\0', MAX_LINE_LENGTH);

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			exit(0);
			break;

		case SDL_KEYDOWN:
			doKeyDown(&event.key);
			break;
		
		case SDL_KEYUP:
			doKeyUp(&event.key);
			break;

		case SDL_TEXTINPUT:
			strncpy_s(app.inputText, 32, event.text.text, MAX_LINE_LENGTH);
			break;

		default:
			break;
		}
	}
}