#pragma once
#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

class Entity
{
private:
    float x, y;
    float x_sp, y_sp;
    SDL_Rect currentFrame;
    SDL_Texture* tex;
    vector <SDL_Texture*> vtex;
public:
    Entity(float p_x, float p_y, float p_w, float p_h, SDL_Texture* p_tex);
    Entity(float p_x, float p_y, float p_w, float p_h, vector <SDL_Texture*> p_tex);
    Entity(float p_x, float p_y, float p_w, float p_h, float p_xsp, float p_ysp, SDL_Texture* p_tex);

    float getX();
    float getY();
    float getX_sp();
    float getY_sp();

    int vLength();

    void changeX_sp(float p_xsp);
    void changeY_sp(float p_ysp);
    void changeX(float p_x);
    void changeY(float p_y);
    void changeXY(float p_x, float p_y);
    void changeTex(SDL_Texture* p_tex);
    void changeVTex(vector<SDL_Texture*> p_tex);

    void moving(float p_xsp, float p_ysp);
    //void control();

    SDL_Texture* getVTex(int index);
    SDL_Texture* getTex();
    SDL_Rect getCurrentFrame();
};
