#include "CommonFunc.hpp"

#include "renderwindow.hpp"
#include "entity.hpp"

bool haveAWinner = false;
bool p1Turn = true;
bool p1Ded = false;
bool p2Ded = false;
bool needSfx = true;

int timer=1;

Mix_Chunk* p_chunk = NULL;
// catch keyboard event
SDL_Event event;

RenderWindow window("Hit Da Hole", SCREEN_WIDTH, SCREEN_HEIGHT);

SDL_Texture* bgTexture = window.loadTexture("rec/bg.png");
SDL_Texture* ballTexture = window.loadTexture("rec/ball.png");
SDL_Texture* hole1Texture = window.loadTexture("rec/hole/1.png");
SDL_Texture* hole2Texture = window.loadTexture("rec/hole/2.png");
SDL_Texture* dedTexture = window.loadTexture("rec/ded.png");

vector <SDL_Texture*> player1ITexture =
{
    window.loadTexture("rec/player1/stand/1.png"),
    window.loadTexture("rec/player1/stand/2.png"),
    window.loadTexture("rec/player1/stand/3.png"),
    window.loadTexture("rec/player1/stand/4.png")
};
vector <SDL_Texture*> player1WTexture =
{
    window.loadTexture("rec/player1/walk/1.png"),
    window.loadTexture("rec/player1/walk/2.png"),
    window.loadTexture("rec/player1/walk/3.png"),
    window.loadTexture("rec/player1/walk/4.png")
};
vector <SDL_Texture*> player2ITexture =
{
    window.loadTexture("rec/player2/stand/1.png"),
    window.loadTexture("rec/player2/stand/2.png"),
    window.loadTexture("rec/player2/stand/3.png"),
    window.loadTexture("rec/player2/stand/4.png")
};
vector <SDL_Texture*> player2WTexture =
{
    window.loadTexture("rec/player2/walk/1.png"),
    window.loadTexture("rec/player2/walk/2.png"),
    window.loadTexture("rec/player2/walk/3.png"),
    window.loadTexture("rec/player2/walk/4.png"),
};
vector <SDL_Texture*> slashTexture =
{
    window.loadTexture("rec/slash/1.png"),
    window.loadTexture("rec/slash/2.png"),
    window.loadTexture("rec/slash/1.png")
};

Entity ball(SCREEN_WIDTH / 2 - 12, SCREEN_HEIGHT / 2, 24, 24, 0, 1, ballTexture);
Entity hole(SCREEN_WIDTH / 2 - 12, SCREEN_HEIGHT / 2 - 40, 24, 24, 0, -1, hole1Texture);
Entity player1(SCREEN_WIDTH / 3 - 30, SCREEN_HEIGHT / 2 - 24, 60, 60, player1ITexture);
Entity player2(SCREEN_WIDTH / 3 * 2 - 30, SCREEN_HEIGHT / 2 - 24, 60, 60, player2ITexture);
Entity slash1(0, 0, 120, 120, slashTexture);
Entity slash2(0, 0, 120, 120, slashTexture);

void ResetStatus()
{
    haveAWinner = false;
    p1Turn = true;
    p1Ded = false;
    p2Ded = false;
    needSfx = true;
}
void PlaySfx(Mix_Chunk* p_chunk, const char* p_filePath)
{
    p_chunk = Mix_LoadWAV(p_filePath);
    Mix_PlayChannel(-1, p_chunk, 0);
}
void MovingOfBH()
{
    if(ball.getX() <= 0 || ball.getX() >= SCREEN_WIDTH - 12)
    {
        ball.changeX_sp(ball.getX_sp() * -1);
        PlaySfx(p_chunk,"sfx/bounce.wav");
    }
    if(ball.getY() <= 0 || ball.getY() >= SCREEN_HEIGHT - 12)
    {
        ball.changeY_sp(ball.getY_sp() * -1);
        PlaySfx(p_chunk,"sfx/bounce.wav");
    }
    ball.moving(ball.getX_sp(), ball.getY_sp());

    if(hole.getX() <= 0 || hole.getX() >= SCREEN_WIDTH - 12)
    {
        hole.changeX_sp(hole.getX_sp() * -1);
        PlaySfx(p_chunk,"sfx/bounce.wav");
    }
    if(hole.getY() <= 0 || hole.getY() >= SCREEN_HEIGHT - 12)
    {
        hole.changeY_sp(hole.getY_sp() * -1);
        PlaySfx(p_chunk,"sfx/bounce.wav");
    }
    hole.moving(hole.getX_sp(), hole.getY_sp());
}
void Auto()
{
    player1.changeVTex(player1ITexture);
    player2.changeVTex(player2ITexture);
    if(timer%15 == 0)
    {
        player1.changeIndex(player1.getIndex() + 1);
        player2.changeIndex(player2.getIndex() + 1);
        slash1.changeIndex(slash1.getIndex() + 1);
        slash2.changeIndex(slash2.getIndex() + 1);
    }
    if(timer == 100) timer = 0;
    timer++;
}
void Animation(Entity &player, vector<SDL_Texture*> p_vtex, float p_x, float p_y, SDL_RendererFlip p_flip)
{
    player.changeFlip(p_flip);
    player.changeX(p_x);
    player.changeY(p_y);
    player.changeVTex(p_vtex);
}
void Hit(Entity &player, Entity &obj1, Entity &obj2, SDL_KeyCode key)
{
    float txh = obj1.getX() + 12, tyh = obj1.getY() + 12, txp = player.getX() + 30, typ = player.getY() + 30;
    float d = sqrt((txh-txp) * (txh-txp) + (tyh-typ) * (tyh-typ));
    if(d <= 70 && (p1Turn && key == SDLK_SPACE || !p1Turn && key == SDLK_SLASH))
    {
        PlaySfx(p_chunk,"sfx/hit2.wav");

        float currentSpeed = sqrt(obj1.getX_sp() * obj1.getX_sp() + obj1.getY_sp() * obj1.getY_sp());

        obj1.changeX_sp((txh - txp) / d * (currentSpeed + 1));
        if(tyh >= typ) obj1.changeY_sp(sqrt((currentSpeed + 1) * (currentSpeed + 1) - obj1.getX_sp() * obj1.getX_sp()));
        else obj1.changeY_sp(-sqrt((currentSpeed + 1) * (currentSpeed + 1) - obj1.getX_sp() * obj1.getX_sp()));

        if (key == SDLK_SPACE)
        {
            obj1.changeTex(ballTexture);
            obj2.changeTex(hole2Texture);
        }
        if(key == SDLK_SLASH)
        {
            obj1.changeTex(ballTexture);
            obj2.changeTex(hole1Texture);
        }
        p1Turn = !p1Turn;
    }
}

int main(int argc, char* args[])
{
    //ShowWindow(GetConsoleWindow(), SW_HIDE);
    bool gameRunning = true;

    // sfx
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) printf("%s", Mix_GetError());

    while (gameRunning)
    {
        SDL_Delay(10);
        Auto();
        SDL_PollEvent(&event);
        const Uint8* keystates = SDL_GetKeyboardState(NULL);

        if (event.key.keysym.sym == SDLK_ESCAPE || event.type == SDL_QUIT)
        {
            //ShowWindow(GetConsoleWindow(), SW_SHOW);
            gameRunning = false;
        }

        //endgame and restart
        if(haveAWinner)
        {
            if(needSfx)
            {
                needSfx = false;
                PlaySfx(p_chunk,"sfx/win.wav");
            }
            if(event.key.keysym.sym == SDLK_r)
            {
                ResetStatus();
                ball.changeXY(SCREEN_WIDTH / 2 - 12, SCREEN_HEIGHT / 2);
                ball.changeXY_sp(0, 1);
                ball.changeTex(ballTexture);
                hole.changeXY(SCREEN_WIDTH / 2 - 12, SCREEN_HEIGHT / 2 - 40);
                hole.changeXY_sp(0, -1);
                hole.changeTex(hole1Texture);
                player1.changeXY(SCREEN_WIDTH / 3 - 30, SCREEN_HEIGHT / 2 - 24);
                player2.changeXY(SCREEN_WIDTH / 3 * 2 - 30, SCREEN_HEIGHT / 2 - 24);
            }
        }
        else
        {
            MovingOfBH();

            // p1 controller
            if(keystates[SDL_SCANCODE_D] && player1.getX() <= SCREEN_WIDTH - 60)
            {
                Animation(player1, player1WTexture, player1.getX() + 4, player1.getY(), SDL_FLIP_HORIZONTAL);
            }
            if(keystates[SDL_SCANCODE_A] && player1.getX() >= 0)
            {
                Animation(player1, player1WTexture, player1.getX() - 4, player1.getY(), SDL_FLIP_NONE);
            }
            if(keystates[SDL_SCANCODE_S] && player1.getY() <= SCREEN_HEIGHT - 60)
            {
                Animation(player1, player1WTexture, player1.getX(), player1.getY() + 4, SDL_FLIP_NONE);
            }
            if(keystates[SDL_SCANCODE_W] && player1.getY() >= 0)
            {
                Animation(player1, player1WTexture, player1.getX(), player1.getY() - 4, SDL_FLIP_NONE);
            }
            if(event.key.keysym.sym == SDLK_SPACE)
            {
                Hit(player1, hole, ball, SDLK_SPACE);
            }

            //p2 controller
            if(keystates[SDL_SCANCODE_L] && player2.getX() <= SCREEN_WIDTH - 60)
            {
                Animation(player2, player2WTexture, player2.getX() + 4, player2.getY(), SDL_FLIP_HORIZONTAL);
            }
            if(keystates[SDL_SCANCODE_J] && player2.getX() >= 0)
            {
                Animation(player2, player2WTexture, player2.getX() - 4, player2.getY(), SDL_FLIP_NONE);
            }
            if(keystates[SDL_SCANCODE_K] && player2.getY() <= SCREEN_HEIGHT - 60)
            {
                Animation(player2, player2WTexture, player2.getX(), player2.getY() + 4, SDL_FLIP_NONE);
            }
            if(keystates[SDL_SCANCODE_I] && player2.getY() >= 0)
            {
                Animation(player2, player2WTexture, player2.getX(), player2.getY() - 4, SDL_FLIP_NONE);
            }
            if(event.key.keysym.sym == SDLK_SLASH)
            {
                Hit(player2, ball, hole, SDLK_SLASH);
            }

            //winner
            if(sqrt((ball.getX() - hole.getX()) * (ball.getX() - hole.getX()) + (ball.getY() - hole.getY()) * (ball.getY() - hole.getY())) <= 24)
            {
                haveAWinner = true;
                if(p1Turn) cout << "p2\n";
                else cout << "p1\n";
                PlaySfx(p_chunk,"sfx/winbh.wav");
            }

            if(sqrt((ball.getX()+12 - player1.getX()-30) * (ball.getX()+12 - player1.getX()-30) + (ball.getY()+12 - player1.getY()-30) * (ball.getY()+12 - player1.getY()-30)) <= 24
                    && p1Turn)
            {
                p1Ded = true;
                haveAWinner = true;
                cout << "p2\n";
                player1.changeTex(dedTexture);
                PlaySfx(p_chunk,"sfx/lose.wav");
            }
            if(sqrt((hole.getX() + 12 - player2.getX() - 30) * (hole.getX()+12 - player2.getX()-30) + (hole.getY()+12 - player2.getY()-30) * (hole.getY()+12 - player2.getY()-30)) <= 24
                    && !p1Turn)
            {
                haveAWinner = true;
                p2Ded = true;
                cout << "p1\n";
                player2.changeTex(dedTexture);
                PlaySfx(p_chunk,"sfx/lose.wav");
            }

            //render
            window.clear();
            window.render(bgTexture);
            window.render(hole);
            window.render(ball);

            if(!p1Ded) window.render(player1, player1.getIndex());
            else window.render(player1);

            if(!p2Ded) window.render(player2, player1.getIndex());
            else window.render(player2);

            if(event.key.keysym.sym == SDLK_SPACE && !p1Ded)
            {
                Animation(slash1, slashTexture, player1.getX() - 30, player1.getY() - 30, player1.getFlip());
                window.render(slash1, slash1.getIndex());
            }
            if(event.key.keysym.sym == SDLK_SLASH && !p2Ded)
            {
                Animation(slash2, slashTexture, player2.getX() - 30, player2.getY() - 30, player2.getFlip());
                window.render(slash2, slash2.getIndex());
            }

            window.display();
        }
    }

    window.cleanUp();
    SDL_Quit();
    return 0;
}
