#pragma once
#include "CommonFunc.hpp"

class Entity
{
private:
    float x, y;
    float x_sp, y_sp;
    int index = 0;
    SDL_Rect currentFrame;
    SDL_Texture* tex;
    vector <SDL_Texture*> vtex;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

public:
    Entity(float p_x, float p_y, float p_w, float p_h, SDL_Texture* p_tex);
    Entity(float p_x, float p_y, float p_w, float p_h, vector <SDL_Texture*> p_tex);
    Entity(float p_x, float p_y, float p_w, float p_h, float p_xsp, float p_ysp, SDL_Texture* p_tex);

    float getX();
    float getY();

    float getX_sp();
    float getY_sp();

    int getVLength();

    int getIndex();

    void changeX_sp(float p_xsp);
    void changeY_sp(float p_ysp);

    void changeX(float p_x);
    void changeY(float p_y);

    void changeXY(float p_x, float p_y);
    void changeXY_sp(float p_xsp, float p_ysp);

    void changeTex(SDL_Texture* p_tex);
    void changeVTex(vector<SDL_Texture*> p_tex);

    void changeFlip(SDL_RendererFlip p_flip);

    void changeIndex(int p_index);

    void moving(float p_xsp, float p_ysp);

    SDL_RendererFlip getFlip();

    SDL_Texture* getVTex(int p_index);
    SDL_Texture* getTex();
    SDL_Rect getCurrentFrame();
};
