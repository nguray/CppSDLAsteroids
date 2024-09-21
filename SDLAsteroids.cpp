/*-------------------------------------------------------------------------*\
                        Simple Asteroids using SDL2

    Raymond NGUYEN THANH                                    20024-09- 18
\*-------------------------------------------------------------------------*/

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <istream>
#include <map>
#include <math.h>
#include <memory> // for std::unique_ptr
#include <ostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <utility> // for std::move
#include <vector>

namespace fs = std::filesystem;

#include "Bullet.h"
#include "Rock.h"
#include "Ship.h"

typedef struct HighScore
{
  std::string name;
  int score;
} HighScore;

typedef enum
{
  STAND_BY,
  PLAY,
  GAME_OVER,
  HIGH_SCORES
} GameMode;

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

bool fPause = true;
std::shared_ptr<Ship> ship{
    new Ship(RVector2D{WIN_WIDTH / 2.0, WIN_HEIGHT / 2.0}, -90.0) };

std::vector<std::shared_ptr<Bullet>> bullets;
std::vector<std::shared_ptr<Rock>> rocks;

void DoScreenFrameCollison(std::shared_ptr<GameObject> obj, SDL_Rect& scrRect)
{
  auto left = static_cast<float>(scrRect.x) + obj->radius;
  auto top = static_cast<float>(scrRect.y) + obj->radius;
  auto right = static_cast<float>(scrRect.x + scrRect.w) - obj->radius;
  auto bottom = static_cast<float>(scrRect.y + scrRect.h) - obj->radius;

  if ((obj->pos.x <= left) || (obj->pos.x > right))
  {
    obj->velocity.x = -obj->velocity.x;
  }

  if ((obj->pos.y <= top) || (obj->pos.y > bottom))
  {
    obj->velocity.y = -obj->velocity.y;
  }
}

void DoCollison(std::shared_ptr<GameObject> obj0,
  std::shared_ptr<GameObject> obj1)
{
  //--------------------------------
  auto p0 = obj0->pos;
  auto p1 = obj1->pos;
  auto m0 = obj0->mass;
  auto m1 = obj1->mass;
  auto r0 = obj0->radius;
  auto r1 = obj1->radius;
  auto veloVec0 = obj0->velocity;
  auto veloVec1 = obj1->velocity;

  auto v = p1 - p0;
  auto d = v.magnitude();

  if (d <= (r0 + r1))
  {

    auto nV12 = v;
    auto tV12 = v.normal();

    auto unV12 = RVector2D::normalize(nV12);
    auto utV12 = RVector2D::normalize(tV12);

    auto nV1 = veloVec0.dot(unV12);
    auto tV1 = veloVec0.dot(utV12);
    auto nV2 = veloVec1.dot(unV12);
    auto tV2 = veloVec1.dot(utV12);

    auto sumMass = m0 + m1;

    auto nV1c = (nV1 * (m0 - m1) + 2 * m1 * nV2) / sumMass;
    auto nV2c = (nV2 * (m1 - m0) + 2 * m0 * nV1) / sumMass;

    //--
    auto v0 = unV12;
    v0.mul(nV1c);
    auto newVeloVec0 = utV12;
    newVeloVec0.mul(tV1);
    newVeloVec0 += v0;
    obj0->velocity = newVeloVec0;

    //--
    auto v1 = unV12;
    v1.mul(nV2c);
    auto newVeloVec1 = utV12;
    newVeloVec1.mul(tV2);
    newVeloVec1 += v1;
    obj1->velocity = newVeloVec1;

  }
}

void FireBullet()
{
  //--------------------------------
  if (fPause)
  {
    fPause = false;
  }
  auto v = ship->DirectionVect();
  v.mul(5.0);
  bullets.push_back(std::make_shared<Bullet>(ship->pos, v));
}

int RandomInt(int a, int b)
{
  int diff = b - a + 1;
  int r = rand() % diff;
  return a + r;
}

void NewGame()
{
  //--------------------------------
  for (auto i = 0; i < 5; ++i)
  {
    auto rock = std::make_shared<Rock>();

    auto m = static_cast<float>(RandomInt(1, 2));
    auto ri = 10.0 * m;

    auto px = static_cast<float>(RandomInt(0, WIN_WIDTH));
    if (px < ri)
    {
      px = ri + 1;
    }
    else if (px > (WIN_WIDTH - ri))
    {
      px = WIN_WIDTH - ri - 1;
    }

    auto py = static_cast<float>(RandomInt(0, WIN_HEIGHT));
    if (py < ri)
    {
      py = ri + 1;
    }
    else if (py > (WIN_HEIGHT - ri))
    {
      px = WIN_HEIGHT - ri - 1;
    }

    auto ra = static_cast<double>(RandomInt(0, 360) * M_PI / 180.0);
    rock->pos = RVector2D{ px, py };
    rock->velocity = RVector2D{ static_cast<float>(1.35f * cos(ra)),
                               static_cast<float>(1.35f * sin(ra)) };
    rock->mass = m;
    rock->radius = ri;
    rocks.push_back(rock);
  }
}

void SubDivideRock(std::shared_ptr<Rock> r, float m)
{
  //--
  auto uv = RVector2D::normalize(r->velocity);
  auto un = uv.normal();
  auto normeV = r->velocity.magnitude() * 1.7f;

  // Direction 10h30
  auto v10 = uv + un;
  auto p10 = r->pos + (v10 * 10);
  auto v11 = RVector2D::normalize(v10) * normeV;
  rocks.push_back(std::make_shared<Rock>(p10, v11, m));
  // Direction 1h30
  auto v20 = uv - un;
  auto p20 = r->pos + (v20 * 10);
  auto v21 = RVector2D::normalize(v20) * normeV;
  rocks.push_back(std::make_shared<Rock>(p20, v21, m));
  // Direction 7h30
  auto v30 = uv - un;
  v30.mul(-1);
  auto p30 = r->pos + v30 * 10;
  auto v31 = RVector2D::normalize(v30) * normeV;
  rocks.push_back(std::make_shared<Rock>(p30, v31, m));
  // Direction 4h30
  auto v40 = uv + un;
  v40.mul(-1);
  auto p40 = r->pos + v40 * 10;
  auto v41 = RVector2D::normalize(v40) * normeV;
  rocks.push_back(std::make_shared<Rock>(p40, v41, m));
}

constexpr const char* resoucesDir{ "../resources" };

int main(int argc, char* argv[])
{
  // std::cout << "Hello World !!" << SDL_GetError() << std::endl;
  TTF_Font* gFont = NULL;

  // The window we'll be rendering to
  SDL_Window* window = NULL;

  // The surface contained by the window

  SDL_Renderer* renderer = NULL;

  // std::cout << "Current working directory: " <<
  // std::filesystem::current_path() << std::endl;

  // Initialize PNG loading

  auto initError = IMG_Init(IMG_INIT_PNG);

  if (initError == IMG_INIT_PNG)
  {
    std::cout << "SDL_image initialized" << std::endl;
  }
  else
  {
    std::cerr << "Failed to initialize SDL_image" << std::endl;
    std::cerr << "Return value: " << initError << std::endl;
    std::cerr << "Error flags: " << IMG_GetError() << std::endl;
    return -1;
  }

  TTF_Init();

  // Open the font
  fs::path resDir;
  ;
  if (fs::exists("../resources"))
  {
    resDir = fs::path("../resources");
  }
  else
  {
    resDir = fs::path("./resources");
  }

  auto filePath = resDir / "sansation.ttf";
  gFont = TTF_OpenFont(filePath.c_str(), 18);
  // gFont = TTF_OpenFont( "../resources/sansation.ttf", 18 );
  if (gFont == NULL)
  {
    printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
    TTF_Quit();
    return 0;
  }
  TTF_SetFontStyle(gFont, TTF_STYLE_ITALIC | TTF_STYLE_BOLD);

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  }
  else
  {

    //--
    filePath = resDir / "Plane00.png";
    std::cout << filePath.c_str() << std::endl;
    // Load image at specified path
    SDL_Surface* shipdSurface0 = IMG_Load(filePath.c_str());

    filePath = resDir / "Plane01.png";
    std::cout << filePath.c_str() << std::endl;
    // Load image at specified path
    SDL_Surface* shipdSurface1 = IMG_Load(filePath.c_str());

    filePath = resDir / "Plane02.png";
    std::cout << filePath.c_str() << std::endl;
    // Load image at specified path
    SDL_Surface* shipdSurface2 = IMG_Load(filePath.c_str());

    //--
    srand(time(NULL));

    //--

    // Create window
    window = SDL_CreateWindow("C++ SDL Asteroids", SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT,
      SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {

      Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);

      // Mix_Music *tetrisMusic = Mix_LoadMUS("../resources/Tetris.wav");

      // Mix_Chunk *succesSound =
      // Mix_LoadWAV("../resources/109662__grunz__success.wav"); if
      // (succesSound!=NULL){
      //     Mix_VolumeChunk(succesSound,15);
      // }

      // Get window surface
      // screenSurface = SDL_GetWindowSurface( window );

      renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

      SDL_Texture* shipTex0 =
        SDL_CreateTextureFromSurface(renderer, shipdSurface0);
      SDL_Texture* shipTex1 =
        SDL_CreateTextureFromSurface(renderer, shipdSurface1);
      SDL_Texture* shipTex2 =
        SDL_CreateTextureFromSurface(renderer, shipdSurface2);

      ship->idleTex = shipTex0;
      ship->accelTex = shipTex1;
      ship->decelTex = shipTex2;
      ship->curTex = shipTex0;

      Uint32 startTimeV = SDL_GetTicks();
      Uint32 startExplodeUpdate = SDL_GetTicks();
      Uint32 startTimeR = startTimeV;

      Rock::InitCosSinValues();

      // Init new game
      NewGame();

      SDL_Rect screenFrame{ 0, 0, WIN_WIDTH, WIN_HEIGHT };

      // Event handler
      SDL_Event e;

      // Main loop flag
      bool fQuitGame = false;

      int iRotate = 0;
      int iAccel = 0;

      // While application is running
      while (!fQuitGame)
      {

        //--
        SDL_SetRenderDrawColor(renderer, 16, 16, 64, 64);
        SDL_RenderClear(renderer);

        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
          //
          if (e.type == SDL_QUIT)
          {
            fQuitGame = true;
          }
          else if (e.type == SDL_KEYDOWN)
          {
            switch (e.key.keysym.sym)
            {
            case SDLK_ESCAPE:
              fQuitGame = true;
              break;
            case SDLK_UP:
              iAccel = 1;
              break;
            case SDLK_DOWN:
              iAccel = -1;
              break;
            case SDLK_LEFT:
              iRotate = 1;
              break;
            case SDLK_RIGHT:
              iRotate = -1;
              break;
            case SDLK_SPACE:
              FireBullet();
              break;
            }
          }
          else if (e.type == SDL_KEYUP)
          {
            switch (e.key.keysym.sym)
            {
            case SDLK_UP:
            case SDLK_DOWN:
              iAccel = 0;
              break;
            case SDLK_LEFT:
            case SDLK_RIGHT:
              iRotate = 0;
              break;
            }
          }
        }

        if (iRotate < 0)
        {
          ship->OffsetAngle(2);
        }
        else if (iRotate > 0)
        {
          ship->OffsetAngle(-2);
        }

        if (!fPause)
        {

          if (iAccel > 0)
          {
            ship->Accelerate(0.1);
            ship->SetForwardThrush();
          }
          else if (iAccel < 0)
          {
            ship->Accelerate(-0.1);
            ship->SetBackwardThrush();
          }
          else
          {
            ship->SetIdleThrush();
          }

          ship->UpdatePosition();

          DoScreenFrameCollison(ship, screenFrame);

          // Check Hit
          auto it = bullets.begin();
          while (it != bullets.end())
          {
            //--
            auto b = (*it);
            b->UpdatePosition();
            if ((b->pos.x < 0) ||
              (b->pos.x > WIN_WIDTH) ||
              (b->pos.y < 0) ||
              (b->pos.y > WIN_HEIGHT))
            {
              it = bullets.erase(it);
              continue;
            }
            else {
              //--
              for (const auto rock : rocks)
              {
                if ((rock->iExplode == 0) && (b->Hit(rock)))
                {
                  b->fDelete = true;
                  if (rock->mass == 2)
                  {
                    rock->fDelete = true;
                    //-- SubDivide
                    SubDivideRock(rock, rock->mass / 3.0);

                    // fPause = true
                  }
                  else if (rock->mass == 1)
                  {
                    rock->fDelete = true;
                    //-- SubDivide
                    SubDivideRock(rock, rock->mass / 2.0);

                    // fPause = true
                  }
                  else
                  {
                    rock->iExplode = 1;
                    rock->InitExplosion();

                    //  fPause = true
                  }
                  break;
                }
              }

              //-- Update bullets liste
              if (b->fDelete) {
                it = bullets.erase(it);
              }
              else {
                it++;
              }

            }

          }

          for (const auto& r : rocks)
          {
            r->UpdatePosition();
            DoScreenFrameCollison(r, screenFrame);
          }

          // Do Collison Ship <-> Rocks
          for (const auto& r : rocks)
          {
            if ((!r->fDelete) && (r->iExplode == 0))
            {
              DoCollison(ship, r);
            }
          }

          // Do collison between rocks
          for (auto i = 0; i < rocks.size(); ++i)
          {
            auto r = rocks[i];
            if (!(r->fDelete) && (r->iExplode == 0))
            {
              for (auto j = i + 1; j < rocks.size(); ++j)
              {
                auto r1 = rocks[j];
                if (!(r1->fDelete) && (r1->iExplode == 0))
                {
                  DoCollison(r, r1);
                }
              }
            }
          }

          //--
          auto curTicks = SDL_GetTicks();
          if ((curTicks - startExplodeUpdate) > 100)
          {
            startExplodeUpdate = curTicks;
            for (auto& rock : rocks)
            {
              if (rock->iExplode > 0)
              {
                rock->iExplode++;
                rock->UpdateExplosion();
                if (rock->iExplode > 4)
                {
                  rock->fDelete = true;
                }
              }
            }
          }
        }
        else
        {
          if (iAccel != 0)
          {
            fPause = false;
          }
        }

        //--
        SDL_Rect rect = { 0, 0, WIN_WIDTH, WIN_HEIGHT };
        SDL_SetRenderDrawColor(renderer, 10, 10, 100, 255);
        SDL_RenderFillRect(renderer, &rect);

        //-- Draw Ship
        ship->Draw(renderer);

        //-- Draw Bullets
        for (const auto b : bullets) {
          b->Draw(renderer);
        }

        //-- Draw and remove delete rocks
        auto it1 = rocks.begin();
        while (it1 != rocks.end())
        {
          if ((*it1)->fDelete)
          {
            it1 = rocks.erase(it1);
          }
          else
          {
            (*it1)->Draw(renderer);
            it1++;
          }
        }

        //--
        SDL_RenderPresent(renderer);

        if (rocks.size() == 0)
        {
          NewGame();
          bullets.clear();
          fPause = true;
          while (SDL_PollEvent(&e) != 0)
          {
          }
          SDL_Delay(500);
        }

        SDL_Delay(20);

        //std::cout << "nbBullets : " << bullets.size() << std::endl;
      }

      Mix_CloseAudio();

      SDL_DestroyTexture(shipTex0);
      SDL_DestroyTexture(shipTex1);
      SDL_DestroyTexture(shipTex2);

      //-- Renderer
      if (renderer)
      {
        SDL_DestroyRenderer(renderer);
      }

      //-- Destroy window
      SDL_DestroyWindow(window);
    }

    //--
    SDL_FreeSurface(shipdSurface0);
    SDL_FreeSurface(shipdSurface1);
    SDL_FreeSurface(shipdSurface2);

    // Quit SDL subsystems
    SDL_Quit();
  }

  //--
  TTF_Quit();

  IMG_Quit();

  return 0;
}