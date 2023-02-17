#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <windows.h>

#include "renderwindow.cpp"
#include "entity.cpp"

using namespace std;

const int Win_h = 480, Win_w = 640;

int main(int argc, char* args[])
{
    //ShowWindow(GetConsoleWindow(), SW_HIDE);

	if (SDL_Init(SDL_INIT_VIDEO) > 0)
		cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << endl;
	if (!(IMG_Init(IMG_INIT_PNG)))
		cout << "IMG_init has failed. Error: " << SDL_GetError() << endl;

	RenderWindow window("Hit Da Hole", Win_w, Win_h);

	SDL_Texture* bgTexture = window.loadTexture("rec/bg.png");
    SDL_Texture* ballTexture = window.loadTexture("rec/ball.png");
    SDL_Texture* holeTexture = window.loadTexture("rec/hole.png");
    SDL_Texture* player1Texture = window.loadTexture("rec/player1/stand/stand1.png");
    SDL_Texture* player2Texture = window.loadTexture("rec/player2/stand/stand1.png");

    Entity ball(308, 240, 20, 20, 0, 1, ballTexture);
    Entity hole(308, 200, 20, 20, 0, -1, holeTexture);
    Entity player1(150, 228, 60, 60, player1Texture);
    Entity player2(466, 228, 60, 60, player2Texture);

	bool gameRunning = true;
	bool p1Turn = true;

	SDL_Event event;

	while (gameRunning)
	{
		SDL_Delay(10);

		SDL_PollEvent(&event);

        if (event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_QUIT)
		{
		    //ShowWindow(GetConsoleWindow(), SW_SHOW);
		    gameRunning = false;
		}

		//ball and hole movement
        if(ball.getX() <= 0 || ball.getX() >= Win_w - 15) ball.changeX_sp(ball.getX_sp() * -1);
        if(ball.getY() <= 0 || ball.getY() >= Win_h - 15) ball.changeY_sp(ball.getY_sp() * -1);
        ball.moving(ball.getX_sp(), ball.getY_sp());

        if(hole.getX() <= 0 || hole.getX() >= Win_w - 15) hole.changeX_sp(hole.getX_sp() * -1);
        if(hole.getY() <= 0 || hole.getY() >= Win_h - 15) hole.changeY_sp(hole.getY_sp() * -1);
        hole.moving(hole.getX_sp(), hole.getY_sp());


        const Uint8 * keystates = SDL_GetKeyboardState(NULL);
        // p1 controller
        if(keystates[SDL_SCANCODE_D] && player1.getX() <= Win_w - 60)
        {
            player1.changeX(player1.getX() + 4);
        }
        if(keystates[SDL_SCANCODE_A] && player1.getX() >= 0)
        {
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
        if(event.key.keysym.sym == SDLK_e)
        {
            float txh = hole.getX(), tyh = hole.getY(), txp = player1.getX() + 30, typ = player1.getY() + 30;
            float d = sqrt((txh-txp) * (txh-txp) + (tyh-typ) * (tyh-typ));
            if(d <= 80 && p1Turn)
            {
                float moreSpeed = 0.5, currentSpeed = sqrt(hole.getX_sp() * hole.getX_sp() + hole.getY_sp() * hole.getY_sp());

                if(txh >= typ) hole.changeX_sp((txh - txp) / d + moreSpeed);
                else hole.changeX_sp((txh - txp) / d - moreSpeed);
                if(tyh >= typ) hole.changeY_sp(sqrt((currentSpeed + 0.7) * (currentSpeed + 0.7) - hole.getX_sp() * hole.getX_sp()));
                else hole.changeY_sp(-sqrt((currentSpeed + 0.7) * (currentSpeed + 0.7) - hole.getX_sp() * hole.getX_sp()));

                ball.changeTex(holeTexture);
                hole.changeTex(ballTexture);
                p1Turn = false;

            }
        }

        //p2 controller
        if(keystates[SDL_SCANCODE_L] && player2.getX() <= Win_w - 60)
        {
            player2.changeX(player2.getX() + 4);
        }
        if(keystates[SDL_SCANCODE_J] && player2.getX() >= 0)
        {
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
        if(event.key.keysym.sym == SDLK_o)
        {
            float txh = ball.getX(), tyh = ball.getY(), txp = player2.getX() + 30, typ = player2.getY() + 30;
            float d = sqrt((txh - txp) * (txh - txp) + (tyh - typ) * (tyh - typ));
            if(d <= 80 && !p1Turn)
            {
                float moreSpeed = 0.5, currentSpeed = sqrt(ball.getX_sp() * ball.getX_sp() + ball.getY_sp() * ball.getY_sp());

                if(txh >= typ) ball.changeX_sp((txh - txp) / d + moreSpeed);
                else ball.changeX_sp((txh - txp) / d - moreSpeed);
                if(tyh >= typ) ball.changeY_sp(sqrt((currentSpeed + 0.7) * (currentSpeed + 0.7) - ball.getX_sp() * ball.getX_sp()));
                else ball.changeY_sp(-sqrt((currentSpeed + 0.7) * (currentSpeed + 0.7) - ball.getX_sp() * ball.getX_sp()));

                ball.changeTex(ballTexture);
                hole.changeTex(holeTexture);
                p1Turn = true;

            }
        }

        //winner


        //render
        window.clear();
        window.render(bgTexture);
        window.render(hole);
        window.render(ball);
        window.render(player1);
        window.render(player2);
		window.display();
	}

	window.cleanUp();
	SDL_Quit();

	return 0;
}
