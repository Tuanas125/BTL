
#include "RenderWindow.hpp"
#include "Entity.hpp"

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
{
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

    if (window == NULL)
        cout << "Init Error: " << SDL_GetError() << endl;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath)
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, p_filePath);

    if (texture == NULL)
        std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;

    return texture;
}

void RenderWindow::clear()
{
    SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& p_entity)
{
    SDL_Rect src;
    src.x = p_entity.getCurrentFrame().x;
    src.y = p_entity.getCurrentFrame().y;
    src.w = p_entity.getCurrentFrame().w;
    src.h = p_entity.getCurrentFrame().h;

    SDL_Rect dst;
    dst.x = p_entity.getX() + 0;
    dst.y = p_entity.getY() + 0;
    dst.w = p_entity.getCurrentFrame().w;
    dst.h = p_entity.getCurrentFrame().h;

    SDL_RenderCopyEx(renderer, p_entity.getTex(), &src, &dst, NULL, NULL, SDL_FLIP_NONE);
}

void RenderWindow::render(Entity& p_entity, int index)
{
    SDL_Rect src;
    src.x = p_entity.getCurrentFrame().x;
    src.y = p_entity.getCurrentFrame().y;
    src.w = p_entity.getCurrentFrame().w;
    src.h = p_entity.getCurrentFrame().h;

    SDL_Rect dst;
    dst.x = p_entity.getX() + 0;
    dst.y = p_entity.getY() + 0;
    dst.w = p_entity.getCurrentFrame().w;
    dst.h = p_entity.getCurrentFrame().h;

    if(SDL_RenderCopyEx(renderer, p_entity.getVTex(index), &src, &dst, 0, NULL, p_entity.getFlip()) != 0)
    {
        cout << SDL_GetError;
    };
}

void RenderWindow::render(float p_x, float p_y, SDL_Texture* p_tex)
{
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    SDL_QueryTexture(p_tex, NULL, NULL, &src.w, &src.h);

    SDL_Rect dst;
    dst.x = p_x;
    dst.y = p_y;
    dst.w = src.w;
    dst.h = src.h;

    SDL_RenderCopy(renderer, p_tex, &src, &dst);
}

void RenderWindow::render(SDL_Texture* p_tex)
{
    render(0, 0, p_tex);
}
void RenderWindow::render(Text &p_text, float p_x, float p_y)
{
    p_text.changeTextTex(SDL_CreateTextureFromSurface(renderer, p_text.getTextSurf()));

    SDL_Rect dst;
    dst.x = p_x;
    dst.y = p_y;
    dst.w = p_text.getTextRect().w;
    dst.h = p_text.getTextRect().h;

    SDL_RenderCopy(renderer, p_text.getTextTex(), NULL, &dst);
}
void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}

void RenderWindow::cleanUp()
{
    SDL_DestroyWindow(window);
}
