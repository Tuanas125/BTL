#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <math.h>

#include "renderwindow.hpp"
#include "entity.hpp"

using namespace std;

const int Win_h = 480, Win_w = 640;

int main(int argc, char* args[])
{
    //weapon+doi huong
    //ShowWindow(GetConsoleWindow(), SW_HIDE);

    int indexStand=0, time=1;
    bool gameRunning = true;
    bool p1Turn = true;
    bool p1Ded = false;
    bool p2Ded = false;

    if (SDL_Init(SDL_INIT_VIDEO) > 0)
        cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << endl;
    if (!(IMG_Init(IMG_INIT_PNG)))
        cout << "IMG_init has failed. Error: " << SDL_GetError() << endl;

    RenderWindow window("Hit Da Hole", Win_w, Win_h);

    SDL_Texture* bgTexture = window.loadTexture("rec/bg.png");
    SDL_Texture* ballTexture = window.loadTexture("rec/ball.png");
    SDL_Texture* hole1Texture = window.loadTexture("rec/hole1.png");
    SDL_Texture* hole2Texture = window.loadTexture("rec/hole2.png");
    SDL_Texture* dedTexture = window.loadTexture("rec/ded.png");

    vector <SDL_Texture*> player1RTexture =
    {
        window.loadTexture("rec/player1/standR/stand1.png"),
        window.loadTexture("rec/player1/standR/stand2.png"),
        window.loadTexture("rec/player1/standR/stand3.png"),
        window.loadTexture("rec/player1/standR/stand4.png"),
    };
    vector <SDL_Texture*> player1LTexture =
    {
        window.loadTexture("rec/player1/standL/stand1.png"),
        window.loadTexture("rec/player1/standL/stand2.png"),
        window.loadTexture("rec/player1/standL/stand3.png"),
        window.loadTexture("rec/player1/standL/stand4.png"),
    };

    vector <SDL_Texture*> player2RTexture =
    {
        window.loadTexture("rec/player2/standR/stand1.png"),
        window.loadTexture("rec/player2/standR/stand2.png"),
        window.loadTexture("rec/player2/standR/stand3.png"),
        window.loadTexture("rec/player2/standR/stand4.png"),
    };
    vector <SDL_Texture*> player2LTexture =
    {
        window.loadTexture("rec/player2/standL/stand1.png"),
        window.loadTexture("rec/player2/standL/stand2.png"),
        window.loadTexture("rec/player2/standL/stand3.png"),
        window.loadTexture("rec/player2/standL/stand4.png"),
    };

    vector <SDL_Texture*> slashTexture =
    {
        window.loadTexture("rec/slash/1.png"),
        window.loadTexture("rec/slash/2.png"),
        window.loadTexture("rec/slash/2.png"),
        window.loadTexture("rec/slash/3.png"),
    };

    Entity ball(308, 240, 24, 24, 0, 1, ballTexture);
    Entity hole(308, 200, 24, 24, 0, -1, hole1Texture);
    Entity player1(150, 228, 60, 60, player1RTexture);
    Entity player2(466, 228, 60, 60, player2LTexture);
    Entity slash1(466, 228, 120, 120, slashTexture);
    Entity slash2(466, 228, 120, 120, slashTexture);

    SDL_Event event;

    while (gameRunning)
    {
        time++;
        SDL_Delay(10);

        SDL_PollEvent(&event);

        if (event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_QUIT)
        {
            //ShowWindow(GetConsoleWindow(), SW_SHOW);
            gameRunning = false;
        }

        //ball and hole movement
        if(ball.getX() <= 0 || ball.getX() >= Win_w - 12) ball.changeX_sp(ball.getX_sp() * -1);
        if(ball.getY() <= 0 || ball.getY() >= Win_h - 12) ball.changeY_sp(ball.getY_sp() * -1);
        ball.moving(ball.getX_sp(), ball.getY_sp());

        if(hole.getX() <= 0 || hole.getX() >= Win_w - 12) hole.changeX_sp(hole.getX_sp() * -1);
        if(hole.getY() <= 0 || hole.getY() >= Win_h - 12) hole.changeY_sp(hole.getY_sp() * -1);
        hole.moving(hole.getX_sp(), hole.getY_sp());


        const Uint8 * keystates = SDL_GetKeyboardState(NULL);
        // p1 controller
        if(keystates[SDL_SCANCODE_D] && player1.getX() <= Win_w - 60)
        {
            player1.changeVTex(player1RTexture);
            player1.changeX(player1.getX() + 4);
        }
        if(keystates[SDL_SCANCODE_A] && player1.getX() >= 0)
        {
            player1.changeVTex(player1LTexture);
            player1.changeX(player1.getX() - 4);
        }
        if(keystates[SDL_SCANCODE_S] && player1.getY() <= Win_h - 60)
        {
            player1.changeY(player1.getY() + 4);
        }
        if(keystates[SDL_SCANCODE_W] && player1.getY() >= 0)
        {
            player1.changeY(player1.getY() - 4);
        }
        if(event.key.keysym.sym == SDLK_SPACE)
        {
            float txh = hole.getX() + 12, tyh = hole.getY() + 12, txp = player1.getX() + 30, typ = player1.getY() + 30;
            float d = sqrt((txh-txp) * (txh-txp) + (tyh-typ) * (tyh-typ));
            if(d <= 70 && p1Turn)
            {
                float currentSpeed = sqrt(hole.getX_sp() * hole.getX_sp() + hole.getY_sp() * hole.getY_sp());

                hole.changeX_sp((txh - txp) / d * (currentSpeed + 1));
                if(tyh >= typ) hole.changeY_sp(sqrt((currentSpeed + 1) * (currentSpeed + 1) - hole.getX_sp() * hole.getX_sp()));
                else hole.changeY_sp(-sqrt((currentSpeed + 1) * (currentSpeed + 1) - hole.getX_sp() * hole.getX_sp()));

                ball.changeTex(hole2Texture);
                hole.changeTex(ballTexture);
                p1Turn = false;

            }
        }

        //p2 controller
        if(keystates[SDL_SCANCODE_L] && player2.getX() <= Win_w - 60)
        {
            player2.changeVTex(player2RTexture);
            player2.changeX(player2.getX() + 4);
        }
        if(keystates[SDL_SCANCODE_J] && player2.getX() >= 0)
        {
            player2.changeVTex(player2LTexture);
            player2.changeX(player2.getX() - 4);
        }
        if(keystates[SDL_SCANCODE_K] && player2.getY() <= Win_h - 60)
        {
            player2.changeY(player2.getY() + 4);
        }
        if(keystates[SDL_SCANCODE_I] && player2.getY() >= 0)
        {
            player2.changeY(player2.getY() - 4);
        }
        if(event.key.keysym.sym == SDLK_SLASH)
        {
            float txh = ball.getX() + 12, tyh = ball.getY() + 12, txp = player2.getX() + 30, typ = player2.getY() + 30;
            float d = sqrt((txh - txp) * (txh - txp) + (tyh - typ) * (tyh - typ));
            if(d <= 70 && !p1Turn)
            {
                float currentSpeed = sqrt(ball.getX_sp() * ball.getX_sp() + ball.getY_sp() * ball.getY_sp());

                ball.changeX_sp((txh - txp) / d * (currentSpeed + 1));
                if(tyh >= typ) ball.changeY_sp(sqrt((currentSpeed + 1) * (currentSpeed + 1) - ball.getX_sp() * ball.getX_sp()));
                else ball.changeY_sp(-sqrt((currentSpeed + 1) * (currentSpeed + 1) - ball.getX_sp() * ball.getX_sp()));

                ball.changeTex(ballTexture);
                hole.changeTex(hole1Texture);
                p1Turn = true;

            }
        }

        //winner
        if(sqrt((ball.getX() - hole.getX()) * (ball.getX() - hole.getX()) + (ball.getY() - hole.getY()) * (ball.getY() - hole.getY())) <= 24)
        {
            if(p1Turn) cout << "p2\n";
            else cout << "p1\n";
        }

        if(sqrt((ball.getX()+12 - player1.getX()-30) * (ball.getX()+12 - player1.getX()-30) + (ball.getY()+12 - player1.getY()-30) * (ball.getY()+12 - player1.getY()-30)) <= 24
                && p1Turn)
        {
            cout << "p2\n";
            player1.changeTex(dedTexture);
            p1Ded = true;
        }
        if(sqrt((hole.getX() + 12 - player2.getX() - 30) * (hole.getX()+12 - player2.getX()-30) + (hole.getY()+12 - player2.getY()-30) * (hole.getY()+12 - player2.getY()-30)) <= 24
                && !p1Turn)
        {
            cout << "p1\n";
            player2.changeTex(dedTexture);
            p2Ded = true;
        }

        //render
        if(time%20==0)
        {
            indexStand++;
            time=1;
        }
        if(indexStand==4)
        {
            indexStand=0;
        }

        window.clear();
        window.render(bgTexture);
        window.render(hole);
        window.render(ball);
        if(!p1Ded) window.render(player1, indexStand);
        else window.render(player1);
        if(!p2Ded) window.render(player2, indexStand);
        else window.render(player2);

        if(event.key.keysym.sym == SDLK_SPACE && !p1Ded) {
            slash1.changeXY(player1.getX()-30, player1.getY()-30);
            window.render(slash1, indexStand);
        }
        if(event.key.keysym.sym == SDLK_SLASH && !p2Ded) {
            slash2.changeXY(player2.getX()-30, player2.getY()-30);
            window.render(slash2, indexStand);
        }

        window.display();
    }

    window.cleanUp();
    SDL_Quit();

    return 0;
}
