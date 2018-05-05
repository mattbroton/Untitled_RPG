//
//  enemy.h
//  Untitled_RPG
//
//  Created by Matthew Broton on 4/13/16.
//  Copyright © 2016 Matthew Broton. All rights reserved.
//

#ifndef enemy_h
#define enemy_h

#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"

// const int totalweak = 8;

typedef struct{
    char * name;
    
    int maxHP;
    int currentHP;
    int weak;
    
    int exp;
    
    int atk;
    
    int boss; // 0 if not boss. > 0 if is a boss.
    
    char * texturefn;
    SDL_Texture * enemyT;
    SDL_Rect enemyR;
}enemy;

enemy * createClubGoblin();

void ewotaldFire(SDL_Renderer * gRender);

enemy * createEwotald();

void setZero(enemy * c);

enemy * createAxeSkeleton();

void destroyEnemy(enemy * c);

int bossAttack(int bossNum, SDL_Renderer * gRender);

#endif /* enemy_h */



