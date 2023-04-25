#pragma once
#include "CommonFunc.hpp"
#include "Entity.hpp"
#include "Text.hpp"

class RenderWindow
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
public:
    RenderWindow(const char* p_title, int p_w, int p_h);
    SDL_Texture* loadTexture(const char* p_filePath);
    void clear();
    void render(Entity& p_entity);
    void render(Entity& p_entity, int index);
    void render(float p_x, float p_y, SDL_Texture* p_tex);
    void render(SDL_Texture* p_tex);
    void render(Text &p_text, float p_x, float p_y);
    void display();
    void cleanUp();
};
