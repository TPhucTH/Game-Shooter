#include "common.h"

static SDL_Texture* Load;

int selectedOption = 0;

void Event(SDL_Renderer* renderer, TTF_Font* font, const std::string& title, const std::vector<std::string>& options, int x, int y) {
    std::vector<SDL_Rect> optionRects(options.size()); 
    int menuHeight = 0;

    for (int i = 0; i < options.size(); i++) {
        SDL_Surface* tempSurface = TTF_RenderText_Solid(font, options[i].c_str(), { 255, 255, 255 });
        optionRects[i].x = x;
        optionRects[i].y = y + menuHeight;
        optionRects[i].w = tempSurface->w;
        optionRects[i].h = tempSurface->h;
        menuHeight += tempSurface->h + 10; 
        SDL_FreeSurface(tempSurface);
    }

    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit(1);
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                    selectedOption = (selectedOption - 1 + options.size()) % options.size();
                    break;
                case SDLK_DOWN:
                    selectedOption = (selectedOption + 1) % options.size();
                    break;
                case SDLK_RETURN:
                    done = true;
                    break;
                }
            }
            if (event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = event.motion.x;
                int mouseY = event.motion.y;
                for (int i = 0; i < options.size(); i++) {
                    if (mouseX >= optionRects[i].x && mouseX <= optionRects[i].x + optionRects[i].w &&
                        mouseY >= optionRects[i].y && mouseY <= optionRects[i].y + optionRects[i].h) {
                        selectedOption = i;
                        if (event.type == SDL_MOUSEBUTTONDOWN) {
                            done = true;
                        }
                        break;
                    }
                }
            }
        }

       
        for (int i = 0; i < options.size(); i++) {
            SDL_Color color = i == selectedOption ? SDL_Color{ 255, 0, 0, 255 } : SDL_Color{ 255, 255, 255, 255 };
            SDL_Surface* surface = TTF_RenderText_Solid(font, options[i].c_str(), color);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect renderQuad = { optionRects[i].x, optionRects[i].y, surface->w, surface->h };
            SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        SDL_RenderPresent(renderer);
    }
}

int showMenu(SDL_Renderer* renderer, TTF_Font* font, const std::string& title, const std::vector<std::string>& options)
{

    const char* s = "gfx/menu_background.jpg";
    choseBackground(s);

    int y = 240;
    int x = 150;
    Event(renderer, font, title, options, x, y);

    return selectedOption;
}
int back(SDL_Renderer* renderer, TTF_Font* font, const std::string& title, const std::vector<std::string>& options)
{
   
    int y = SCREEN_WIDTH / 10;
    int x = 70;
    Event(renderer, font, title, options, x, y);

    return selectedOption;
}
int Guide(SDL_Renderer* renderer, TTF_Font* font, const std::string& title) {
   
    const char* s = "gfx/Guide.jpg";
    choseBackground(s);

    SDL_Color color = { 0, 255, 0, 255};

    std::vector<std::string> option = {
        "Guide Game SHOOTER",
        "Press CTRL to play game",
        "Use CTRL to shoot",
        "Eat P to decrease Point"
    };
    drawText(SCREEN_WIDTH / 2, 80, 255, 255, 255, TEXT_CENTER, "GUIDE PLAY GAME");
    drawText(SCREEN_WIDTH / 2, 160, 255, 255, 255, TEXT_CENTER, "WELCOME TO SDL2 SHOOTER - VERY EASY GAME");
    int n = option.size();
    std::vector<std::string> option1 = { "back" };
    for (int i = 0; i < n; i++) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, option[i].c_str(), color);
        if (!surface) {
            
            continue;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            SDL_FreeSurface(surface);
            continue;
        }

        int textWidth = surface->w;
        int textHeight = surface->h;
        SDL_Rect renderQuad = { SCREEN_WIDTH / 2 - textWidth / 2, 240 + i * 80, textWidth, textHeight };
        SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
        
    int test = back(renderer, font, title, option1);
    return test;
}
int Setting_sound(SDL_Renderer* renderer, TTF_Font* font, const std::string& title) {
   
    const char* s = "gfx/Guide.jpg";
    choseBackground(s);

    SDL_Color color = { 0, 255, 0, 255 };

    std::vector<std::string> option = {
        "Chung ta cua tuong lai","The Fat Fat","Tat nhac","back"
    };
    drawText(SCREEN_WIDTH/2 , 80, 255, 255, 255, TEXT_CENTER, "CHOOSE MUSIC");
    int n = option.size();
    int y = 160;
    int x = 350;
    Event(renderer, font, title, option, x, y);
    return selectedOption;
}
