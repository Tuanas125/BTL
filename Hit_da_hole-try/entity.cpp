#include "Entity.hpp"
#include "RenderWindow.hpp"

Entity::Entity(float p_x, float p_y, float p_w, float p_h, SDL_Texture* p_tex)
    :x(p_x), y(p_y), tex(p_tex)
{
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = p_w;
    currentFrame.h = p_h;
}

Entity::Entity(float p_x, float p_y, float p_w, float p_h, vector<SDL_Texture*> p_tex)
    :x(p_x), y(p_y), vtex(p_tex)
{
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = p_w;
    currentFrame.h = p_h;
}

Entity::Entity(float p_x, float p_y, float p_w, float p_h, float p_xsp, float p_ysp, SDL_Texture* p_tex)
    :x(p_x), y(p_y), tex(p_tex)
{
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = p_w;
    currentFrame.h = p_h;
    this->x_sp = p_xsp;
    this->y_sp = p_ysp;
}

float Entity::getX()
{
    return x;
}
float Entity::getY()
{
    return y;
}
float Entity::getX_sp()
{
    return x_sp;
}
float Entity::getY_sp()
{
    return y_sp;
}

int Entity::getVLength()
{
    return vtex.size();
}
int Entity::getIndex()
{
    return index;
}

void Entity::changeX_sp(float p_xsp)
{
    this -> x_sp = p_xsp;
}
void Entity::changeY_sp(float p_ysp)
{
    this -> y_sp = p_ysp;
}

void Entity::changeX(float p_x)
{
    this->x = p_x;
}
void Entity::changeY(float p_y)
{
    this->y = p_y;
}

void Entity::changeXY(float p_x, float p_y)
{
    this->x = p_x;
    this->y = p_y;
}
void Entity::changeXY_sp(float p_xsp, float p_ysp)
{
    this->x_sp = p_xsp;
    this->y_sp = p_ysp;
}

void Entity::changeTex(SDL_Texture* p_tex)
{
    this->tex = p_tex;
}

void Entity::changeVTex(vector<SDL_Texture*> p_tex)
{
    this->vtex = p_tex;
}

void Entity::changeFlip(SDL_RendererFlip p_flip)
{
    this->flip = p_flip;
}

void Entity::changeIndex(int p_index)
{
    this->index = p_index;
    if(p_index >= this->getVLength()) this->index = 0;
}

void Entity::moving(float p_xsp, float p_ysp)
{
    this->x += p_xsp;
    this->y += p_ysp;
}

SDL_RendererFlip Entity::getFlip()
{
    return flip;
}

SDL_Texture* Entity::getTex()
{
    return tex;
}

SDL_Texture* Entity::getVTex(int p_index)
{
    return vtex[p_index];
}


SDL_Rect Entity::getCurrentFrame()
{
    return currentFrame;
}
