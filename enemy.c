//
//  enemy.c
//  Untitled_RPG
//
//  Created by Matthew Broton on 4/13/16.
//  Copyright © 2016 Matthew Broton. All rights reserved.
//

#include "enemy.h"


void setZero(enemy * c){
    for( int i = 0; i < 1 ; i++ ){
        c->weak = 0;
    }
}


/*
    creates and returns the enemy *
    must set the x and y though for where you want the enemy.
    must load the texture on their side because of the renderer
*/

enemy * createAxeSkeleton(){
    
    enemy * c = malloc( sizeof(enemy) );
    
    char * n = "image_files/axeSkeleton.png";
    c->name = malloc(sizeof(char) * ( strlen(n) + 1 ) );
    strcpy(c->name,n);
    
    c->currentHP = 30;
    c->maxHP = 30;
    
    c->exp = 10;
    
    // setZero(c);
    c->weak = 1;
    
    c->boss = 0;
    
    c->atk = 10;
    
    c->enemyR.w = 300;
    c->enemyR.h = 300;
    
    return c;
}


enemy * createClubGoblin(){
    enemy * c = malloc( sizeof(enemy) );
    char * n = "image_files/clubGoblin.png";
    c->name = malloc(sizeof(char) * ( strlen(n) + 1 ) );
    strcpy(c->name,n);
    
    c->currentHP = 50;
    c->maxHP = 50;
    
    c->exp = 30;
    
    // setZero(c);
    c->weak = 1;
    
    c->boss = 0;
    
    c->atk = 20;
    
    c->enemyR.w = 200;
    c->enemyR.h = 200;
    
    return c;
}


enemy * createEwotald(){
    
    enemy * c = malloc( sizeof(enemy) );
    
    char * n = "image_files/ewotald.png";
    c->name = malloc(sizeof(char) * ( strlen(n) + 1 ) );
    strcpy(c->name,n);
    
    c->currentHP = 6000;
    c->maxHP = 6000;
    
    c->exp = 500;
    
    // setZero(c);
    c->weak = 1;
    
    c->boss = 1; // 1 is for ewotald
    
    c->atk = 50;
    
    c->enemyR.w = 750;
    c->enemyR.h = 200;
    
    return c;
}


void ewotaldFire(SDL_Renderer * gRender){
    SDL_Texture * fire1 = IMG_LoadTexture(gRender, "image_files/ewotald_fire1.png");
    SDL_Texture * fire2 = IMG_LoadTexture(gRender, "image_files/ewotald_fire2.png");
    SDL_Texture * fire3 = IMG_LoadTexture(gRender, "image_files/ewotald_fire3.png");
    SDL_Texture * fire4 = IMG_LoadTexture(gRender, "image_files/ewotald_fire4.png");
    
    SDL_Rect fire;
    fire.x = 0;
    fire.y = 0;
    fire.w = 750;
    fire.h = 200;

    SDL_RenderCopy(gRender, fire1 , NULL, &fire );
    SDL_RenderPresent(gRender);
    SDL_Delay(1000);
    
    SDL_RenderCopy(gRender, fire2 , NULL, &fire );
    SDL_RenderPresent(gRender);
    SDL_Delay(1000);

    SDL_RenderCopy(gRender, fire3 , NULL, &fire );
    SDL_RenderPresent(gRender);
    SDL_Delay(1000);

    SDL_RenderCopy(gRender, fire4 , NULL, &fire );
    SDL_RenderPresent(gRender);
    SDL_Delay(1000);

    
    SDL_DestroyTexture(fire1);
    SDL_DestroyTexture(fire2);
    SDL_DestroyTexture(fire3);
    SDL_DestroyTexture(fire4);
}


int bossAttack(int bossNum, SDL_Renderer * gRender){
    if( bossNum == 1 ){
        ewotaldFire(gRender);
    }
    else if(bossNum == 2){
        // nothing here yet
    }
    else{
    }
    return 0;
}


void destroyEnemy(enemy * c){
    free( c->name );
    SDL_DestroyTexture(c->enemyT);
    free(c);
    c = NULL;
}