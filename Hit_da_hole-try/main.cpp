#include "CommonFunc.hpp"

#include "renderwindow.hpp"
#include "entity.hpp"

bool needMenu = true;
bool pause = false;
bool haveAWinner = false;
bool p1Turn = true;
bool p1Ded = false;
bool p2Ded = false;
bool needSfx = true;

int timer = 1;
int xMouse, yMouse;

Mix_Chunk* p_chunk = NULL;
Mix_Music* p_music = NULL;
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

Entity ball(SCREEN_WIDTH / 2 - 12, SCREEN_HEIGHT / 2, 24, 24, -10, 10, ballTexture);
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
    pause = false;
    ball.changeXY(SCREEN_WIDTH / 2 - 12, SCREEN_HEIGHT / 2);
    ball.changeXY_sp(0, 1);
    ball.changeTex(ballTexture);
    hole.changeXY(SCREEN_WIDTH / 2 - 12, SCREEN_HEIGHT / 2 - 40);
    hole.changeXY_sp(0, -1);
    hole.changeTex(hole1Texture);
    player1.changeXY(SCREEN_WIDTH / 3 - 30, SCREEN_HEIGHT / 2 - 24);
    player2.changeXY(SCREEN_WIDTH / 3 * 2 - 30, SCREEN_HEIGHT / 2 - 24);
}
void PlaySfx(Mix_Chunk* p_chunk, const char* p_filePath)
{
    p_chunk = Mix_LoadWAV(p_filePath);
    Mix_PlayChannel(-1, p_chunk, 0);
}
void PlayMusic(Mix_Music* p_music, const char* p_filePath)
{
    p_music = Mix_LoadMUS(p_filePath);
    Mix_PlayMusic(p_music, -1);
}
void BallMovement()
{
    if(ball.getX() <= 0 || ball.getX() + 20 >= SCREEN_WIDTH)
    {
        ball.changeX_sp(ball.getX_sp() * -1);
        PlaySfx(p_chunk,"sfx/bounce.wav");
    }
    if(ball.getY() <= 0 || ball.getY() + 20 >= SCREEN_HEIGHT)
    {
        ball.changeY_sp(ball.getY_sp() * -1);
        PlaySfx(p_chunk,"sfx/bounce.wav");
    }
    ball.moving(ball.getX_sp(), ball.getY_sp());
}
void HoleMovement()
{
    if(hole.getX() <= 0 || hole.getX() + 20 >= SCREEN_WIDTH)
    {
        hole.changeX_sp(hole.getX_sp() * -1);
        PlaySfx(p_chunk,"sfx/bounce.wav");
    }
    if(hole.getY() <= 0 || hole.getY() + 20 >= SCREEN_HEIGHT)
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
    if(timer == 1000) timer = 0;
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
bool isFlickingButton(Text &text)
{
    if(event.motion.x || event.motion.y) xMouse = event.motion.x, yMouse = event.motion.y;
    return (xMouse >= text.getX() && xMouse <= text.getX() + text.getWidth() &&
        yMouse >= text.getY() && yMouse <= text.getY() + text.getHeight());
}
int main(int argc, char* args[])
{
    TTF_Init();
    TTF_Font *crackmanB = TTF_OpenFont("font/Crackman back.ttf", 100);
    TTF_Font *crackmanF = TTF_OpenFont("font/Crackman front.ttf", 100);
    TTF_Font *nasa = TTF_OpenFont("font/nasalization-rg.ttf", 70);
    TTF_Font *monster = TTF_OpenFont("font/MonsterBitesItalic-1Gy52.ttf", 150);
    TTF_Font *rimou = TTF_OpenFont("font/rimouski sb.ttf", 100);

    ShowWindow(GetConsoleWindow(), SW_HIDE);
    bool gameRunning = true;
    // sfx
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) printf("%s", Mix_GetError());

    while (gameRunning)
    {
        SDL_Delay(10);
        Auto();
        SDL_PollEvent(&event);
        const Uint8* keystates = SDL_GetKeyboardState(NULL);

        if (event.type == SDL_QUIT)
        {
            //ShowWindow(GetConsoleWindow(), SW_SHOW);
            gameRunning = false;
        }
        if(needMenu)
        {
            if(!Mix_PlayingMusic()) PlayMusic(p_music, "sfx/bgmusic/showurmove.mp3");
            SDL_Color black = {0, 0, 0}, white = {255, 255, 255};
            Text gameName1(crackmanB, "Hit da hole", black, 400, 130);
            Text gameName2(crackmanF, "Hit da hole", white, 400, 130);
            SDL_Color guiColor;
            if(timer % 40 == 0) guiColor = black;
            if(timer % 80 == 0) guiColor = white;
            Text guide(nasa, "Press [space] to start the game", guiColor, 480, 40);

            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                gameRunning = false;
            }
            if (event.key.keysym.sym == SDLK_SPACE)
            {
                needMenu = false;
                Mix_HaltMusic();
                ResetStatus();
            }
            BallMovement();

            window.clear();
            window.render(bgTexture);
            window.render(ball);
            window.render(gameName1, SCREEN_WIDTH / 2 - gameName1.getWidth() / 2, SCREEN_HEIGHT / 2 - gameName1.getHeight() + 30);
            window.render(gameName2, SCREEN_WIDTH / 2 - gameName2.getWidth() / 2, SCREEN_HEIGHT / 2 - gameName2.getHeight() + 30);
            window.render(guide, SCREEN_WIDTH / 2 - guide.getWidth() / 2, SCREEN_HEIGHT - guide.getHeight() - 50);
            window.display();
        }
        //endgame and restart
        else if(haveAWinner)
        {
            SDL_Color black = {0, 0, 0}, white = {255, 255, 255};
            Text p1w(monster, "Player 1 wins", white, 300, 50);
            Text p2w(monster, "Player 2 wins", white, 300, 50);
            Text playagain(rimou, "Play again", white, 150, 40);
            Text targetedPG(rimou, "> Play again <", white, 190, 40);
            Text main_menu(rimou, "Main menu", white, 150, 40);
            Text targetedMM(rimou, "> Main menu <", white, 190, 40);
            Text tip(nasa, "Press [R] key to restart quickly", white, 450, 28);

            if(needSfx)
            {
                needSfx = false;
                PlaySfx(p_chunk,"sfx/win.wav");
            }
            if(event.key.keysym.sym == SDLK_r || isFlickingButton(playagain) && event.button.clicks)
            {
                ResetStatus();
            }
            if(isFlickingButton(main_menu) && event.button.clicks)
            {
                ResetStatus();
                Mix_HaltMusic();
                needMenu = true;
                ball.changeXY_sp(-10, 10);
            }

            window.clear();
            window.render(bgTexture);
            window.render(hole);
            window.render(ball);
            if(!p1Ded) window.render(player1, player1.getIndex());
                else window.render(player1);
            if(!p2Ded) window.render(player2, player1.getIndex());
                else window.render(player2);

            if(!p1Turn) window.render(p1w, SCREEN_WIDTH / 2 - p1w.getWidth() / 2, p1w.getHeight() + 40);
                else window.render(p2w, SCREEN_WIDTH / 2 - p2w.getWidth() / 2, p2w.getHeight() + 40);
            if(isFlickingButton(playagain))window.render(targetedPG, SCREEN_WIDTH / 2 - targetedPG.getWidth() / 2, SCREEN_HEIGHT / 2);
                else window.render(playagain, SCREEN_WIDTH / 2 - playagain.getWidth() / 2, SCREEN_HEIGHT / 2);
            if(isFlickingButton(main_menu)) window.render(targetedMM, SCREEN_WIDTH / 2 - targetedMM.getWidth() / 2, SCREEN_HEIGHT / 2 + 60);
                else window.render(main_menu, SCREEN_WIDTH / 2 - main_menu.getWidth() / 2, SCREEN_HEIGHT / 2 + 60);
            window.render(tip, SCREEN_WIDTH / 2 - tip.getWidth() / 2, SCREEN_HEIGHT - tip.getHeight() - 10);
            window.display();
        }
        else if(pause)
        {
            SDL_Color black = {0, 0, 0}, white = {255, 255, 255};
            Text paused(monster, "PAUSED", white, 200, 50);
            Text resume(rimou, "Resume", white, 120, 40);
            Text targetedResume(rimou, "> Resume <", white, 160, 40);
            Text main_menu(rimou, "Main Menu", white, 150, 40);
            Text targetedMM(rimou, "> Main Menu <", white, 190, 40);

            if(event.button.clicks)
            {
                if (isFlickingButton(resume))
                {
                    pause = false;
                }
                if (isFlickingButton(main_menu))
                {
                    ResetStatus();
                    needMenu = true;
                    Mix_HaltMusic();
                    ball.changeXY_sp(-10, 10);
                }
            }

            window.clear();
            window.render(bgTexture);
            window.render(hole);
            window.render(ball);
            if(!p1Ded) window.render(player1, player1.getIndex());
                else window.render(player1);
            if(!p2Ded) window.render(player2, player1.getIndex());
                else window.render(player2);

            window.render(paused, SCREEN_WIDTH / 2 - paused.getWidth() / 2, paused.getHeight() + 40);
            if(isFlickingButton(resume))window.render(targetedResume, SCREEN_WIDTH / 2 - targetedResume.getWidth() / 2, SCREEN_HEIGHT / 2);
                else window.render(resume, SCREEN_WIDTH / 2 - resume.getWidth() / 2, SCREEN_HEIGHT / 2);
            if(isFlickingButton(main_menu)) window.render(targetedMM, SCREEN_WIDTH / 2 - targetedMM.getWidth() / 2, SCREEN_HEIGHT / 2 + 60);
                else window.render(main_menu, SCREEN_WIDTH / 2 - main_menu.getWidth() / 2, SCREEN_HEIGHT / 2 + 60);
            window.display();
        }
        else
        {
            if(!Mix_PlayingMusic())
            {
                PlayMusic(p_music, "sfx/bgmusic/Battle BGM.mp3");
                Mix_VolumeMusic(50);
            }
            BallMovement();
            HoleMovement();

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
                Animation(player1, player1WTexture, player1.getX(), player1.getY() + 4, player1.getFlip());
            }
            if(keystates[SDL_SCANCODE_W] && player1.getY() >= 0)
            {
                Animation(player1, player1WTexture, player1.getX(), player1.getY() - 4, player1.getFlip());
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
                Animation(player2, player2WTexture, player2.getX(), player2.getY() + 4, player2.getFlip());
            }
            if(keystates[SDL_SCANCODE_I] && player2.getY() >= 0)
            {
                Animation(player2, player2WTexture, player2.getX(), player2.getY() - 4, player2.getFlip());
            }
            if(event.key.keysym.sym == SDLK_SLASH)
            {
                Hit(player2, ball, hole, SDLK_SLASH);
            }

            //winner
            if(sqrt((ball.getX() - hole.getX()) * (ball.getX() - hole.getX()) + (ball.getY() - hole.getY()) * (ball.getY() - hole.getY())) <= 24)
            {
                haveAWinner = true;
                PlaySfx(p_chunk,"sfx/winbh.wav");
            }

            if(sqrt((ball.getX()+12 - player1.getX()-30) * (ball.getX()+12 - player1.getX()-30) + (ball.getY()+12 - player1.getY()-30) * (ball.getY()+12 - player1.getY()-30)) <= 24
                    && p1Turn)
            {
                p1Ded = true;
                haveAWinner = true;
                player1.changeTex(dedTexture);
                PlaySfx(p_chunk,"sfx/lose.wav");
            }
            if(sqrt((hole.getX() + 12 - player2.getX() - 30) * (hole.getX()+12 - player2.getX()-30) + (hole.getY()+12 - player2.getY()-30) * (hole.getY()+12 - player2.getY()-30)) <= 24
                    && !p1Turn)
            {
                haveAWinner = true;
                p2Ded = true;
                player2.changeTex(dedTexture);
                PlaySfx(p_chunk,"sfx/lose.wav");
            }

            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                pause = true;
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
    TTF_Quit();
    SDL_Quit();
    return 0;
}
