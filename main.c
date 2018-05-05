//
//  main.c
//  Untitled_RPG
//
//  Created by Matthew Broton on 4/6/16.
//  Copyright © 2016 Matthew Broton. All rights reserved.
//


/*
 Credit must go to this lazyfoo guy and his great tutorial on SDL. I used this a lot to get started.
 
 http://lazyfoo.net/tutorials/SDL/index.php
 
 
 also used, Example 3-12. Proper Game Movement
 
 https://www.libsdl.org/release/SDL-1.2.15/docs/html/guideinputkeyboard.html
 
*/

/*
 commit to svn by doing these commands in terminal
1. cd Documents
2. svn import -m "New Import" Untitled_RPG https://subversion.ews.illinois.edu/svn/sp16-cs242/mbroton2/
 
 svn import -m "New Import" finalproject_week3 https://subversion.ews.illinois.edu/svn/sp16-cs242/mbroton2/

 svn import -m "New Import" . https://subversion.ews.illinois.edu/svn/sp16-cs242/mbroton2/

  */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_ttf/SDL_ttf.h"

#include "battle_info.h"
#include "game_save.h"
#include "character_info.h"
#include "general_test_functions.h"
#include "objects.h"
// #include "enemy.h"


#define KEY_UP 0
#define KEY_RIGHT 1
#define KEY_DOWN 2
#define KEY_LEFT 3


#define battleMain 1
#define battleMagic 2
#define battleItem 3

#define interactRangeX 75
#define interactRangeY 25

const int screen_width = 750;
const int screen_height = 500;

const char * gameTitle ="Untitled_RPG";

SDL_Window * gameWindow = NULL;
SDL_Surface * gameScreen = NULL;

SDL_Texture * backgroundIMG = NULL;



// window renderer
SDL_Renderer * gameRender = NULL;

// current display texture
SDL_Texture * gameTexture = NULL;


// ewotald
// dragon boss

/*
 1. go into cabin and man tells you that you have to fight the dragon in the forest
 2. go into forest
 3. fight dragon
 4. you win
 */

int char_x = 0;
int char_y = 0;

int char_xvel = 0;
int char_yvel = 0;

int char_height = 100;
int char_width = 100;

// for which
int battleMenuState = 0;
int battleSelect = 0;

int cLocation = 1;

SDL_Event keys;

SDL_Texture * mGuy = NULL;

SDL_Rect ** objects = NULL;
object ** objectArray = NULL;

enemy ** enemies = NULL;
int enemyCount = 0;

int numobjects = 0;

SDL_Rect mGuyRect;

SDL_Rect bgRect;

character * mainChar = NULL;


SDL_Rect ** battlezones = NULL;
int battlezonesnum = 0;

/*
will create an array of pointers to SDL Rects. this will the the battle locations
*/
void createBattleZones(int num){
    battlezones = malloc( sizeof(SDL_Rect*)*num);
    battlezonesnum = num;
    for( int i = 0; i < num; i++){
        battlezones[i] = malloc(sizeof(SDL_Rect));
    }
}

// destroys all of them and sets to null
void destroyBattleZones(){
    if( battlezones != NULL ){
        for( int i = 0; i < battlezonesnum; i++){
            if( battlezones[i] != NULL ){
                free(battlezones[i]);
            }
        }
        free(battlezones);
        battlezones = NULL;
    }
}

/*
    displays the text that the old guy talks to you when you go into the house
*/
void loadOldGuyAdvice(){
    SDL_Texture * adviceT = IMG_LoadTexture(gameRender, "image_files/oldGuyAdvice.png");
    SDL_Rect adviceR;
    adviceR.x = 200;
    adviceR.y = 200;
    adviceR.w = 300;
    adviceR.h = 150;

    SDL_RenderCopy(gameRender, adviceT , NULL, &adviceR );

    SDL_RenderPresent(gameRender);
    
    SDL_Delay(2000);
}


// save the data of the game. this will allow you to load.
void savedata(){
    
    SDL_Texture * saveGameT = IMG_LoadTexture(gameRender, "image_files/savingGame.png");
    SDL_Rect saveGameRect;
    saveGameRect.x = 200;
    saveGameRect.y = 200;
    saveGameRect.w = 300;
    saveGameRect.h = 150;
    
    SDL_RenderCopy(gameRender, saveGameT , NULL, &saveGameRect );
    
    SDL_RenderPresent(gameRender);
    
    save_game("save_data/save_data_1.txt" , mainChar->name, mainChar->level, mainChar->exp, 1 , mainChar->maxHP, mainChar->maxMP, mainChar->base_ATK, mainChar->base_MAG);
    
    SDL_DestroyTexture(saveGameT);
    SDL_Delay(2000);
}


int getEnemyCenterX(enemy * c){
    return c->enemyR.x + c->enemyR.w/2;
}

int getEnemyCenterY(enemy * c){
    return c->enemyR.y + c->enemyR.h/2;
}


int getNextEnemy(int c){
    int checker = 1;
    while( checker == 1 ){
        c ++;
        if( c >= enemyCount ){
            c = 0;
        }
        if( enemies[c] != NULL ){
            checker = 0;
        }
    }
    return c;
}


/*
    fills enemies gv with enemies for battle
    TODO: make this get random enemies
*/
void getEnemies(int numEnemy){
    if( numEnemy <= 0 ){
        return;
    }
    enemies = malloc((numEnemy)*sizeof(enemy * ));
    enemyCount = numEnemy;
    for( int i = 0; i < numEnemy ; i++ ){
        // int d = rand() % 2;
        if( i == 0 ){
            enemies[i] = createClubGoblin();
            enemies[i]->enemyT = IMG_LoadTexture( gameRender, enemies[i]->name );
            enemies[i]->enemyR.x = 50;
            enemies[i]->enemyR.y = 50;
        }
        else if( i == 1 ){
            enemies[i] = createAxeSkeleton();
            enemies[i]->enemyT = IMG_LoadTexture( gameRender, enemies[i]->name );
            enemies[i]->enemyR.x = 350;
            enemies[i]->enemyR.y = 50;
        }
        else{
            enemies[i] = createAxeSkeleton();
            enemies[i]->enemyT = IMG_LoadTexture( gameRender, enemies[i]->name );
            enemies[i]->enemyR.x = 550;
            enemies[i]->enemyR.y = 50;
        }
    }
}

/*
    go through enemy array and display the still alive ones
*/
void displayEnemies(){
    for( int i = 0 ;  i < enemyCount ; i++ ){
        if( enemies[i] != NULL ){
            SDL_RenderCopy(gameRender, enemies[i]->enemyT, NULL , &enemies[i]->enemyR );
        }
    }
}

// will destroy the enemy array
void clearEnemies(){
    for( int i = 0 ;  i < enemyCount ; i++ ){
        if( enemies[i] != NULL ){
            // TODO: move this to enemy.h
            SDL_DestroyTexture(enemies[i]->enemyT);
            destroyEnemy(enemies[i]);
            enemies[i] = NULL;
        }
    }
    free(enemies);
    enemies = NULL;
}

// 0 for enough, 1 for not enough.
int checkMana(){
    if( mainChar->currentMP < 5 ){
        return 0;
    }
    return 1;
}

// what was this suppose to do. checks for dead enemies and removes them.
// returns number of enemies left
int checkDeadEnemies(){
    int count = enemyCount;
    for( int i = 0 ;  i < enemyCount ; i++ ){
        if( enemies[i] != NULL ){
            if( enemies[i]->currentHP <= 0 ){
                SDL_DestroyTexture(enemies[i]->enemyT);
                destroyEnemy(enemies[i]);
                enemies[i] = NULL;
            }
        }
    }
    
    for( int i = 0 ;  i < enemyCount ; i++ ){
        if( enemies[i] != NULL ){
        }
        else{
            count --;
        }
    }
    
    return count;
}

// checks if you are dead. that is main character.
// 0 if false, 1 if true
int checkIfDead(){
    if( mainChar->currentHP <= 0 ){
        return 1;
    }
    return 0;
}

TTF_Font* Sans = NULL;


/*

input: string you wish to display on screen
 
will render the text to the current screen
 
Used the code provided by this post on stack exchange
http://stackoverflow.com/questions/22886500/rendering-text-sdl2
*/
void renderText(char * input, int x, int y, int size){
    if( Sans == NULL){
        printf("Could not load font\n");
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }
    SDL_Color White = {0, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, input , White ); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gameRender, surfaceMessage); //now you can convert it into a texture

    SDL_Rect Message_rect; //create a rect
    Message_rect.x = x;  //controls the rect's x coordinate
    Message_rect.y = y; // controls the rect's y coordinte
    Message_rect.w = ((int) strlen(input))*10; // controls the width of the rect
    Message_rect.h = 50; // controls the height of the rect
    
    SDL_RenderCopy(gameRender, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your textur
}

// creates an object array of a certain size
void createObjectArray(int num){
    objectArray = malloc(sizeof(object*)*num);
    numobjects = num;
}

// used to destroy everythign and the texture
// TODO: it might be better to just destroy the texture in the destroyObject function
void destroyObjectArray(){
    if( objectArray != NULL ){
        for( int i = 0; i < numobjects ; i++ ){
            SDL_DestroyTexture(objectArray[i]->objectT);
            destroyObject(objectArray[i]);
        }
        free(objectArray);
        objectArray = NULL;
    }
    numobjects = 0;
}

/*
    loads the background to walk on
*/
void loadStage1(int entrance){
    
    destroyBattleZones();
    
    // set location of guy based on location
    if( cLocation == 0 ){
        char_x = 100;
        char_y = 200;
    }
    else if( cLocation == 2 ){
        char_x = 525;
        char_y = 100;
    }
    else{
        // i dunno
    }
    cLocation = 1;
    
    createObjectArray(4);
    objectArray[0] = createPond();
    objectArray[1] = createTreeDoor();
    objectArray[2] = createhouse();
    objectArray[3] = createSaveLocation();
    
    
    if( backgroundIMG != NULL){
        SDL_DestroyTexture(backgroundIMG);
    }
    backgroundIMG = IMG_LoadTexture( gameRender, "image_files/back1.png");
    
    
    SDL_Texture * cabin1 = IMG_LoadTexture(gameRender, "image_files/house1.png");
    SDL_Texture * pond = IMG_LoadTexture(gameRender, "image_files/pond.png");
    SDL_Texture * treeDoorT = IMG_LoadTexture(gameRender, "image_files/treeDoor.png" );
    SDL_Texture * sl = IMG_LoadTexture(gameRender, "image_files/savePoint.png" );

    
    objectArray[0]->objectT = pond;
    objectArray[1]->objectT = treeDoorT;
    objectArray[2]->objectT = cabin1;
    objectArray[3]->objectT = sl;
}

void loadStage2(int entrance){
    
    destroyBattleZones();

    
    if( backgroundIMG != NULL){
        SDL_DestroyTexture(backgroundIMG);
    }
    backgroundIMG = IMG_LoadTexture( gameRender, "image_files/back1.png");
    
    if( cLocation == 1 ){
        // main one that should be happeneing
        char_x = 100;
        char_y = 250;
    }
    else if ( cLocation == 3 ){
        // back from lair
    }
    else{
        // i dunno
    }
    
    cLocation = 2;

    destroyObjectArray();
    createObjectArray(3);
    
    objectArray[0] = createStage2Trees();
    SDL_Texture * trees = IMG_LoadTexture(gameRender, "image_files/stage2trees.png");
    objectArray[0]->objectT = trees;
    
    objectArray[1] = createMountainDoor();
    SDL_Texture * me = IMG_LoadTexture(gameRender, "image_files/mountainDoor.png");
    objectArray[1]->objectT = me;
    
    objectArray[2] = createTreeDoor2();
    SDL_Texture * td = IMG_LoadTexture(gameRender, "image_files/treedoor2.png");
    objectArray[2]->objectT = td;
    
    
    // load enemies
    createBattleZones(2);
    battlezones[0]->x = 300;
    battlezones[0]->y = 300;
    battlezones[0]->w = 100;
    battlezones[0]->h = 100;
    
    battlezones[1]->x = 500;
    battlezones[1]->y = 300;
    battlezones[1]->w = 100;
    battlezones[1]->h = 100;
}

void loadStage0(int entrance){
    
    destroyBattleZones();
    
    if( backgroundIMG != NULL){
        SDL_DestroyTexture(backgroundIMG);
    }
    
    cLocation = 0;
    
    backgroundIMG = IMG_LoadTexture( gameRender, "image_files/cabinback1.png");
    
    // load objects( guy, door )ds
    destroyObjectArray();
    createObjectArray(2);
    
    objectArray[0] = createOldGuy();
    
    SDL_Texture * oldGuy = IMG_LoadTexture(gameRender, "image_files/oldGuy.png");
    objectArray[0]->objectT = oldGuy;
    
    
    objectArray[1] = createHouseExit();
    SDL_Texture * he = IMG_LoadTexture(gameRender, "image_files/cabinexit.png");
    objectArray[1]->objectT = he;
    
    char_x = 750/2;
    char_y = 400;
}


void loadStage3(int entrance){
    destroyBattleZones();
    
    cLocation = 3;
    
    if( backgroundIMG != NULL){
        SDL_DestroyTexture(backgroundIMG);
    }
    backgroundIMG = IMG_LoadTexture( gameRender, "image_files/mountainback.png");
    
    // load objects( guy, door )ds
    destroyObjectArray();
    

    createBattleZones(1);
    battlezones[0]->x = 0;
    battlezones[0]->y = 0;
    battlezones[0]->w = 750;
    battlezones[0]->h = 250;

    
    char_x = 750/2;
    char_y = 400;
}

void displayCharBattleStats(){
    // display hp and mp in bottom right corner
    
    char str[180];
    
    sprintf(str, "HP: %d / %d", mainChar->currentHP, mainChar->maxHP);
    puts(str);
    renderText(str, 550 , 375 , 30 );
    
    
    sprintf(str, "MP: %d / %d", mainChar->currentMP, mainChar->maxMP);
    puts(str);
    renderText(str, 550  , 425 , 30 );
}

void doInteraction(int number){
    printf("interaction number i %d \n", number );
    if( number == 100 ){
        loadStage0(1);
    }
    else if( number == 101 ){
        loadStage1(1);
    }
    else if ( number == 102 ){
        loadStage2(1);
    }
    else if( number == 105 ){
        loadStage1(1);
    }
    else if( number == 200 ){
        loadStage3(1);
    }
    else if( number == 20000){
        loadOldGuyAdvice();
    }
    else if( number == 30000){
        savedata();
    }
    else{
        // nothing I guess?
    }
    // SDL_Delay(1500);
}

/*
 Given object, it will check if you can interact with it.
 */
int checkinteraction(object * o){
    if( o->interact > 0 ){
        SDL_Rect oRect = o->objectR;
        if( char_x < oRect.x + + oRect.w/2 + o->interactx && char_x > oRect.x + oRect.w/2 - o->interactx ){
            if( char_y < oRect.y + oRect.h + o->interacty && char_y > oRect.y + oRect.h/2 - o->interacty ){
                // interact with it somehow?, check the num and then do something
                // do different functions based on what the interact term is
                printf("Interacting\n");
                doInteraction(o->interact);
                return 1;
            }
        }
    }
    
    return 0;
}

/*
 Checks if the characer can move to the new location.
 */
int canMoveX(){
    if( mGuyRect.x == 0 && char_xvel  < 0 ){
        char_xvel = 0;
        return 0;
    }
    if( mGuyRect.x + mGuyRect.w == bgRect.w && char_xvel > 0 ){
        char_xvel = 0;
        return 0;
    }
    
    for( int i = 0; i < numobjects; i++ ){
        if( objectArray[i] != NULL ){
            SDL_Rect c = objectArray[i]->objectR;
            if( objectArray[i]->solid == 1 ){ // detect if you cant go through it.
                // from the left
                if( mGuyRect.x == c.x + c.w && char_xvel < 0 ){
                    if( (mGuyRect.y >= c.y && mGuyRect.y <= c.y + c.h) || (mGuyRect.y + mGuyRect.h >= c.y && mGuyRect.y + mGuyRect.h <= c.y + c.h) ){
                        // char_xvel = 0;
                        return 0;
                    }
                }
                
                // from the right
                if( mGuyRect.x + mGuyRect.w == c.x && char_xvel > 0 ){
                    if( (mGuyRect.y >= c.y && mGuyRect.y <= c.y + c.h) || (mGuyRect.y + mGuyRect.h >= c.y && mGuyRect.y + mGuyRect.h <= c.y + c.h) ){
                        // char_xvel = 0;
                        return 0;
                    }
                }
            }
        }
    }
    
    return 1;
}


int canMoveY(){
    
    
    if( mGuyRect.y == 0 && char_yvel  < 0 ){
        char_yvel = 0;
        return 0;
    }
    
    if( mGuyRect.y + mGuyRect.h == bgRect.h && char_yvel  > 0 ){
        char_yvel = 0;
        return 0;
    }
    
    for( int i = 0; i < numobjects; i++ ){
        if( objectArray[i] != NULL ){
            SDL_Rect c = (objectArray[i]->objectR);
            
            if( objectArray[i]->solid == 1 ){
                // from the top
                if( mGuyRect.y + mGuyRect.h == c.y  && char_yvel > 0 ){
                    if( (mGuyRect.x >= c.x && mGuyRect.x <= c.x + c.w) || (mGuyRect.x + mGuyRect.w >= c.x && mGuyRect.x + mGuyRect.w <= c.x + c.w) ){
                        // char_yvel = 0;
                        return 0;
                    }
                    
                    
                }
                // from the bottom
                if( mGuyRect.y == c.y + c.h && char_yvel < 0 ){
                    if( (mGuyRect.x >= c.x && mGuyRect.x <= c.x + c.w) || (mGuyRect.x + mGuyRect.w >= c.x && mGuyRect.x + mGuyRect.w <= c.x + c.w)){
                        // char_yvel = 0;
                        return 0;
                    }
                    
                }
            }
        }
    }
    
    return 1;
}

void loadObjects(int nObjects){
    numobjects = nObjects;
    if( objects != NULL ){
        free(objects);
    }
    objects = malloc(numobjects * sizeof(SDL_Rect *));
    for( int i = 0 ; i < numobjects ; i++ ){
        objects[i] = NULL;
    }
}

// WHAT IS THIS?
char * getDisplayString(int value, int type ){
    if( type == 10 ){
        // get level
    }
    return NULL;
}


// displays the pause menu stats of the main character
void displayCharStats(){
    
    char str[180];
    
    sprintf(str, " %s", mainChar->name);
    puts(str);
    renderText(str, 50, 50, 30 );
    
    sprintf(str, "Level: %d", mainChar->level);
    puts(str);
    renderText(str, 150 , 100 , 20 );
    
    sprintf(str, "Exp: %d", mainChar->exp );
    puts(str);
    renderText(str, 150 , 150 , 20 );
    
    sprintf(str, "HP: %d / %d", mainChar->currentHP, mainChar->maxHP);
    puts(str);
    renderText(str, 150 , 200 , 20 );
    
    sprintf(str, "MP: %d / %d", mainChar->currentMP, mainChar->maxMP);
    puts(str);
    renderText(str, 150 , 250 , 20 );
    
    
    sprintf(str, "Attack: %d", mainChar->base_ATK );
    puts(str);
    renderText(str, 150 , 300 , 20 );
    
    sprintf(str, "Magic: %d", mainChar->base_MAG );
    puts(str);
    renderText(str, 150 , 350 , 20 );

}

// loads the pause screen and then waits until you press p to exit
void pauseScreen( SDL_Event * event ){
    printf("entered pause\n");
    
    // load pause screen here
    
    
    SDL_Texture * pauseScreenT = IMG_LoadTexture( gameRender, "image_files/pauseScreen.png");
    SDL_Rect pauseScreenRect;
    pauseScreenRect.x = 0;
    pauseScreenRect.y = 0;
    pauseScreenRect.w = 750;
    pauseScreenRect.h = 500;
    
    int savedX = mGuyRect.x;
    int savedY = mGuyRect.y;
    int savedW = mGuyRect.w;
    int savedH = mGuyRect.h;

    
    mGuyRect.x = 750 - 20 - 300;
    mGuyRect.y = 50;
    mGuyRect.w = 300;
    mGuyRect.h = 300;
    
    SDL_Texture * charBack = IMG_LoadTexture( gameRender, "image_files/pauseCharacterBackground.png");
    
    SDL_RenderClear(gameRender);

    SDL_RenderCopy(gameRender, pauseScreenT, NULL , &pauseScreenRect );
    
    displayCharStats();
    
    SDL_RenderCopy(gameRender, charBack , NULL , &mGuyRect );

    SDL_RenderCopy(gameRender, mGuy, NULL , &mGuyRect );

    
    SDL_RenderPresent(gameRender);
    
    int checker = 1;
    while( checker == 1 ){
        SDL_PollEvent(event);
        if( event->type == SDL_KEYDOWN ){
            if( event->key.keysym.sym == SDLK_p ){
                checker = 0;
            }
        }
    }
    
    mGuyRect.x = savedX;
    mGuyRect.y = savedY;
    mGuyRect.w = savedW;
    mGuyRect.h = savedH;
    
    SDL_DestroyTexture(pauseScreenT);
    SDL_DestroyTexture(charBack);

    
    printf("exit pause\n");
}

/*
 if switching levels, remeber to set velocity of char.
 */
void checkInteract(){
    if( objectArray != NULL ){
        for( int i = 0; i < numobjects; i++ ){
            if( objectArray[i] != NULL ) {
                checkinteraction(objectArray[i]);
            }
        }
    }
}

/*
Must check all 4 sides of character to make sure it nothing is run into.
*/


/*
 w : up
 a : left
 s : down
 d : right
*/
void char_movement(SDL_Event * event){

    int checker = 0;
        switch ( event->type ) {
                // key pressed down
            case SDL_KEYDOWN:
                switch ( event->key.keysym.sym ) {
                    case SDLK_a:
                        char_xvel = -1;
                        break;
                    case SDLK_s:
                        char_yvel = 1;
                        break;
                    case SDLK_d:
                        char_xvel = 1;
                        checker = 1;
                        break;
                    case SDLK_w:
                        char_yvel = -1;
                        break;
                    case SDLK_p:
                        pauseScreen(event);
                        break;
                    case SDLK_z:
                        // save the game
                        // savedata();
                        break;
                    case SDLK_l:
                        checkInteract();
                        break;
                    default:
                        break;
                }
                break;
                // key released
            case SDL_KEYUP:
                switch (event->key.keysym.sym) {
                    case SDLK_a:
                        if( char_xvel < 0 ){
                            char_xvel = 0;
                        }
                        break;
                    case SDLK_s:
                        if( char_yvel > 0 ){
                            char_yvel = 0;
                        }
                        break;
                    case SDLK_d:
                        if( char_xvel > 0 ){
                            char_xvel = 0;
                        }
                        checker = 1;
                        break;
                    case SDLK_w:
                        if( char_yvel < 0 ){
                            char_yvel = 0;
                        }
                        break;
                    default:
                        break;
                }
            default:
                break;
        }
    
    
    // update position
    if( char_xvel != 0 ){
        if(canMoveX() == 1 ){
            char_x += char_xvel;
        }
    }

    if( char_yvel != 0 ){
        if(canMoveY() == 1 ){
            char_y += char_yvel;
        }
    }
    
    mGuyRect.x = char_x;
    mGuyRect.y = char_y;
    
    /*
    if( checker == 1 ){
        printf("x = %d, y = %d\n",char_x,char_y);
        checker = 0;
    }
    */
}


/*
 Init everything here
 
 */
void init_game(){
    
    if( SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL FAIL: SDL Error = %s\n", SDL_GetError());
    }
    else{
        gameWindow = SDL_CreateWindow(gameTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
        
        if( gameWindow == NULL ){
            printf("Window Failed to Create : SDL_Error = %s\n", SDL_GetError());
        }
        else{
            
            // init png loading
            int imgFlags = IMG_INIT_PNG;
            if( !(IMG_Init(imgFlags) & imgFlags ) ){
            }
            else{
                gameScreen = SDL_GetWindowSurface( gameWindow );
            }
        }
    }
}


// delay in ms
// SDL_Delay(  );
void updateScreen(){
    // updates the gameWindow
    SDL_UpdateWindowSurface( gameWindow );
}


/*
 Loads the image to the screen.
*/
SDL_Surface * loadImage( char * imgPath ){
    // "image_files/background_image_1.png"
    // backgroundIMG = SDL_LoadBMP("background_image_1.png");
    
    SDL_Surface * newSurface = NULL;
    
    // load image at specified path
    SDL_Surface * loadedSurface = IMG_Load(imgPath);
    if( loadedSurface == NULL ){
        printf("Could not load image: %s\n Error: %s",imgPath, IMG_GetError() );
    }
    else{
        // convert surface to screen
        newSurface = SDL_ConvertSurface( loadedSurface, gameScreen->format, NULL );
        if( newSurface == NULL ){
            printf("Could not opt image: %s\n Error: %s",imgPath, SDL_GetError() );
        }
        SDL_FreeSurface( loadedSurface );
    }
    return newSurface;
}



int loadFile( char * imgPath ){
    // gameTexture = loadTexture();
    return 1;
}

/*
 exit everything here
 */
void exit_game(){
    
    SDL_DestroyWindow( gameWindow );
    
    SDL_Quit();
}


/*
Code used to test functions from all classes. Used along with the test header for individual functions.
*/
void test_everything(){
    printf("Starting Tests\n");
    
    int numPass = test_all_calls();
    int numberTests = get_number_tests();
    
    printf("Testing Complete\n");
    printf("Testing Complete: Total Tests = %d , Passed Tests = %d\n",numberTests,numPass);
}

/*
 checks if the character has entered a battle zone
 */
int checkifbattle(){
    if( battlezones != NULL ){
        for( int i = 0 ; i < battlezonesnum; i ++ ){
            if( battlezones[i] != NULL ){
                SDL_Rect r = *(battlezones[i]);
                if( (char_x >= r.x && char_x <= r.x+r.w && char_y >= r.y && char_y <= r.y+r.h) ){
                    battlezones[i] = NULL; // maybe delete this. just so if you run it will not interact again
                    return 1;
                }
            }
        }
    }
    return 0;
}

/*
    given position and state, it will tell you what to do
*/
int battlemove(int cpos, SDL_Event * event ){
    switch ( event->type ) {
        // key pressed down
        case SDL_KEYDOWN:
            switch ( event->key.keysym.sym ) {
                case SDLK_w:
                    if( cpos == 0 || cpos == 1){
                        return cpos;
                    }
                    else{
                        if( cpos == 2 ){
                            return 0;
                        }
                        return 1;
                    }
                    break;
                case SDLK_s:
                    if( cpos == 2 || cpos == 3){
                        return cpos;
                    }
                    else{
                        if( cpos == 0 ){
                            return 2;
                        }
                        return 3;
                    }
                    break;
                case SDLK_a:
                    if( cpos == 0 || cpos == 2){
                        return cpos;
                    }
                    else{
                        if( cpos == 1){
                            return 0;
                        }
                        return 2;
                    }
                    break;
                case SDLK_d:
                    if( cpos == 1 || cpos == 3){
                        return cpos;
                    }
                    else{
                        if( cpos == 0 ){
                            return 1;
                        }
                        return 3;
                    }
                    break;
                case SDLK_l:
                        return -10;
                    break;
                case SDLK_k:
                    return -20;
                    break;
            }
            /*
             case SDL_KEYUP:
             switch (event->key.keysym.sym){
             case SDLK_w:
             break;
             case SDLK_s:
             break;
             case SDLK_l:
             break;
             }
             break;
             */
    }
    return cpos;
}

/*
 get next non null enemy based on input
 direction, 0 is left. 1 is right
*/
int nextEnemy(int current, int direction ){
    int checker = 1;
    while( checker == 1 ){
        if( direction == 0 ){
            // left
            current --;
            if( current < 0 ){
                current = enemyCount - 1;
            }
            if( enemies[current] != NULL ){
                checker = 0;
            }
        }
        else{
            // right
            current ++;
            if( current >= enemyCount ){
                current = 0;
            }
            if( enemies[ current ] != NULL ){
                checker = 0;
            }
        }
    }
    return current;
}

int getTotalEnemyExp(){
    int exp = 0;
    for( int i = 0; i < enemyCount ; i++ ){
        if( enemies[i] != NULL ){
            exp += enemies[i]->exp;
        }
    }
    return exp;
}

int getLeftMostEnemy(){
    for( int i = 0; i < enemyCount ; i++ ){
        if( enemies[i] != NULL ){
            return i;
        }
    }
    return 0;
}


/*
    displays on screen the stats of the currently selected enemy
*/
void displayEnemyStats(int position){
    // display hp and mp in bottom left corner
    
    char str[180];
    
    sprintf(str, "HP: %d / %d", enemies[position]->currentHP, enemies[position]->maxHP);
    puts(str);
    renderText(str, 550 , 375 , 30 );
}

/*
 code for when atk happens. select target. negative/ <0 if canceled.
 returns the position of which enemy was attacked.
*/
int makeTargetSelection(SDL_Event * event, SDL_Texture * battleback, SDL_Rect bbr ){
    printf("entering Target Selection\n");
    
    int currentTarget = getLeftMostEnemy();
    
    SDL_Texture * enemySelectorT = IMG_LoadTexture( gameRender, "image_files/enemySelector.png");
    
    SDL_Texture * battleSelectMenu = IMG_LoadTexture( gameRender, "image_files/EnemySelectMenu.png");

    SDL_Texture * enemyStatsBack = IMG_LoadTexture( gameRender, "image_files/EnemyStatsBack.png");

    
    // must change the x and y value when changing targets
    SDL_Rect enemySelectorR;
    enemySelectorR.x = 0;
    enemySelectorR.y = 300;
    enemySelectorR.w = 150;
    enemySelectorR.h = 150;
    
    SDL_Rect battleMainR;
    battleMainR.x = 0;
    battleMainR.y = 300;
    battleMainR.w = 400;
    battleMainR.h = 200;
    
    
    SDL_Rect enemyStatsBackRect;
    enemyStatsBackRect.x = 400;
    enemyStatsBackRect.y = 300;
    enemyStatsBackRect.w = 350;
    enemyStatsBackRect.h = 200;
    
    // Select Target should appear on screen in bottom left
    SDL_RenderCopy(gameRender, battleSelectMenu , NULL, &battleMainR );
    
    SDL_Delay(200);
    
    // SDL_FlushEvent(event->key);
    SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);
    SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);

    SDL_EventState(SDL_KEYUP, SDL_IGNORE);
    SDL_EventState(SDL_KEYUP, SDL_ENABLE);
    
    SDL_FlushEvent(SDL_KEYUP);
    SDL_FlushEvent(SDL_KEYDOWN);
    
    int running = 1;
    while( running == 1 ){
        SDL_PollEvent(event);
        // printf("entering select loop \n");
        switch ( event->type ) {
            // key presses
            case SDL_KEYDOWN:
                switch ( event->key.keysym.sym ) {
                    case SDLK_a:
                        printf("hitting left \n");
                        currentTarget = nextEnemy(currentTarget, 0 );
                        break;
                    case SDLK_d:
                        printf("hitting right\n");
                        currentTarget = nextEnemy(currentTarget, 1 );
                        break;
                    case SDLK_l:
                        // hit target
                        printf("exiting Target Selection\n");
                        return currentTarget;
                        break;
                    case SDLK_k:
                        printf("exiting Target Selection\n");
                        return -10;
                        break;
                    default:
                        break;
                }
            case SDL_KEYUP:
                switch ( event->key.keysym.sym ) {
                    default:
                        // do nothing
                        break;
                }
            default:
                break;
        }
        
        SDL_RenderClear(gameRender);
        
        SDL_RenderCopy(gameRender, battleback , NULL, &bbr );
        
        displayEnemies();


        SDL_RenderCopy(gameRender, enemyStatsBack , NULL, &enemyStatsBackRect );

        
        enemySelectorR.x = getEnemyCenterX(enemies[currentTarget])  - enemySelectorR.w/2;
        enemySelectorR.y = getEnemyCenterY(enemies[currentTarget]) - enemySelectorR.h/2;
        SDL_RenderCopy(gameRender, enemySelectorT , NULL, &enemySelectorR );
        
        SDL_RenderCopy(gameRender, battleSelectMenu , NULL, &battleMainR );

        
        displayEnemyStats(currentTarget);
        
        SDL_RenderPresent(gameRender);
    }
    
    
    
    
    SDL_DestroyTexture(enemySelectorT);
    SDL_DestroyTexture(battleSelectMenu);
    
    
    printf("exiting Target Selection\n");

    return 0;
}

/*
 loads the game into battle mode. this displays the battle screen.
 
 // 0 for dead, 1 for alive
 */
int battlemode(){
    // gameRender
    // SDL_RenderCopy(gameRender, bz , NULL, &bzr );
    // SDL_RenderPresent(gameRender);
    // SDL_RenderClear(gameRender);
    
    /*
     #define battleMain 1
     #define battleMagic 2
     #define battleItem 3
     */
    
    printf("clocation = %d \n", cLocation);
    
    if( cLocation == 3 ){
        printf("hit this\n");
        enemies = malloc((1)*sizeof(enemy * ));
        enemyCount = 1;
        enemies[0] = createEwotald();
        enemies[0]->enemyT = IMG_LoadTexture( gameRender, enemies[0]->name );
        enemies[0]->enemyR.x = 0;
        enemies[0]->enemyR.y = 0;
    }
    else{
        getEnemies(3);
    }
    
    int enemySelectorPosition = 0; // should always at first be a valid target
    
    int enemyAttackPosition = 0;
    
    int potentialexp = getTotalEnemyExp();
    
    
    
    SDL_Texture * battleMainMenu = IMG_LoadTexture( gameRender, "image_files/battleMain.png");
    SDL_Texture * battleMagicMenu = IMG_LoadTexture( gameRender, "image_files/battleMagic.png");
    SDL_Texture * battleItemMenu = IMG_LoadTexture( gameRender, "image_files/battleItem.png");
    
    
    SDL_Texture * selecter = IMG_LoadTexture( gameRender, "image_files/BattleSelectIcon.png");
    
    
    SDL_Texture * enemyAttack = IMG_LoadTexture( gameRender, "image_files/slash.png");
    
    
    SDL_Rect enemyAttackRect;
    enemyAttackRect.x = 50;
    enemyAttackRect.y = 50;
    enemyAttackRect.w = 600;
    enemyAttackRect.h = 400;
    
    
    
    SDL_Rect selecterR;
    selecterR.x = 0;
    selecterR.y = 300;
    selecterR.w = 200;
    selecterR.h = 100;
    
    
    SDL_Texture * battleback = IMG_LoadTexture( gameRender, "image_files/battleback1.png");
    
    SDL_Rect bbr; // battlebackR
    bbr.x = 0;
    bbr.y = 0;
    bbr.w = 750;
    bbr.h = 300;
    
    
    SDL_Texture * battlestatus = IMG_LoadTexture( gameRender, "image_files/battlestatus.png");
    
    SDL_Rect bbs;
    bbs.x = 400;
    bbs.y = 300;
    bbs.w = 350;
    bbs.h = 200;
    
    
    
    SDL_Rect battleMainR;
    battleMainR.x = 0;
    battleMainR.y = 300;
    battleMainR.w = 400;
    battleMainR.h = 200;
    
    
    int hits = 0;
    
    SDL_Texture * effectdmg = NULL;
    
    SDL_Rect effectdmgR;
    effectdmgR.x = 320;
    effectdmgR.y = 70;
    effectdmgR.w = 50;
    effectdmgR.h = 50;
    
    int usedTurn = 0;
    
    int ranAway = 0;
    
    /*
        top left: 0 , 300
        top right: 200 , 300
        bottom left: 0 , 400
        bottom right: 200 , 400
    */
    
    int fireAtk = 0; // 0 if yes, 1 if no. used for fire paper.
    
    int bmm = 1;
    
    int bmpos = 0;
    
    int bmode = battleMain;
    
    int currentTarget = 0;
    
    while( bmm == 1 ){
        SDL_PollEvent(&keys);
        if( keys.type == SDL_QUIT ){
            bmm = 0;
            // return 300;
        }
        else{
            
            int testmenu = battlemove( bmpos, &keys );
            
            // if enter was pressed
            if( testmenu < 0 ){
                // k was pressed. so go back
                if( testmenu == -20 ){
                    bmode = battleMain;
                    bmpos = 0;
                }
                else{
                    if( bmode == battleMain ){
                        if( bmpos == 0 ){
                            // first need to select target. somehow.
                            printf("hit1\n");
                            currentTarget = makeTargetSelection(&keys,battleback, bbr);
                            printf("hit2\n");
                            if( currentTarget >= 0 ){
                                // atk regular
                                // printf("fire attack value = %d \n", fireAtk );
                                if( fireAtk == 0 ){
                                    effectdmg = IMG_LoadTexture( gameRender, "image_files/slash.png");
                                    enemies[currentTarget]->currentHP -= mainChar->base_ATK;
                                }
                                else{
                                    effectdmg = IMG_LoadTexture( gameRender, "image_files/fireSlash.png");
                                    enemies[currentTarget]->currentHP -= mainChar->base_ATK;
                                }
                                usedTurn = 1;

                            }
                        }
                        else if( bmpos == 1 ){
                            bmpos = 0;
                            bmode = battleMagic;
                        }
                        else if( bmpos == 2 ){
                            bmpos = 0;
                            bmode = battleItem;
                        }
                        else if( bmpos == 3 ){
                            // run option. should be a check here though.
                            bmm = 0;
                            ranAway = 1;
                        }
                        else{}
                    }
                    else if( bmode == battleMagic ){
                        // SDL_Texture * magiceffect;
                        if( bmpos == 0 ){
                            // fire atk
                            if( checkMana() > 0 ){
                                currentTarget = makeTargetSelection(&keys,battleback, bbr);
                                if( currentTarget >= 0 ){
                                    // atk
                                    usedTurn = 1;
                                    bmpos = 0;
                                    effectdmg = IMG_LoadTexture( gameRender, "image_files/fireDmg.png");
                                    enemies[currentTarget]->currentHP -= mainChar->base_MAG;
                                }
                            }
                        }
                        else if( bmpos == 1 ){
                            // ice atk
                            if( checkMana() > 0 ){
                                currentTarget = makeTargetSelection(&keys,battleback, bbr);
                                if( currentTarget >= 0 ){
                                    // atk
                                    usedTurn = 1;
                                    bmpos = 0;
                                    effectdmg = IMG_LoadTexture( gameRender, "image_files/IceDmg.png");
                                    enemies[currentTarget]->currentHP -= mainChar->base_MAG;
                                }
                            }
                        }
                        else if( bmpos == 2 ){
                            // thunder atk
                            if( checkMana() > 0 ){
                                currentTarget = makeTargetSelection(&keys,battleback, bbr);
                                if( currentTarget >= 0 ){
                                    // atk
                                    usedTurn = 1;
                                    bmpos = 0;
                                    effectdmg = IMG_LoadTexture( gameRender, "image_files/ThunderDmg.png");
                                    enemies[currentTarget]->currentHP -= mainChar->base_MAG;
                                }
                            }
                        }
                        else if( bmpos == 3 ){
                            // water atk
                            if( checkMana() > 0 ){
                                currentTarget = makeTargetSelection(&keys,battleback, bbr);
                                if( currentTarget >= 0 ){
                                    // atk
                                    usedTurn = 1;
                                    bmpos = 0;
                                    effectdmg = IMG_LoadTexture( gameRender, "image_files/WaterDmg.png");
                                    enemies[currentTarget]->currentHP -= mainChar->base_MAG;
                                }
                            }
                        }
                        else{
                        }
                        bmode = battleMain;
                        usedTurn = 1;
                        
                    }
                    else if( bmode == battleItem ){
                        if( bmpos == 0 ){
                            // hp potion
                            mainChar->currentHP += 50;
                            if( mainChar-> currentHP > mainChar->maxHP ){
                                mainChar->currentHP = mainChar->maxHP;
                            }
                        }
                        else if( bmpos == 1 ){
                            // fire paper
                            fireAtk = 1;
                        }
                        else if( bmpos == 2 ){
                            // mp potion
                            mainChar->currentMP += 10;
                            if( mainChar-> currentMP > mainChar->maxMP ){
                                mainChar->currentMP = mainChar->maxMP;
                            }
                        }
                        else if( bmpos == 3 ){
                            // poison dart
                            // does nothing right now.
                        }
                        else{
                            
                        }
                        bmode = battleMain;
                        usedTurn = 1;
                        
                    }
                    else{
                        // does something go here?
                    }
                    // BATTLEMODE : quick reference
                }
            }
            else{
                bmpos = testmenu;
            }
            
            if( bmpos == 0 ){
                selecterR.x = 0;
                selecterR.y = 300;
            }
            else if( bmpos == 1 ){
                selecterR.x = 200;
                selecterR.y = 300;
            }
            else if( bmpos == 2 ){
                selecterR.x = 0;
                selecterR.y = 400;
            }
            else if ( bmpos == 3 ){
                selecterR.x = 200;
                selecterR.y = 400;
            }
            else{
                // might need to temp change it to something else to make sure it doesn't display anything
            }
            
            
            SDL_RenderClear(gameRender);
            
            SDL_RenderCopy(gameRender, battleback , NULL, &bbr );
            SDL_RenderCopy(gameRender, battlestatus , NULL, &bbs );
            
            // SDL_RenderCopy(gameRender, enemy1 , NULL, &enemy1R );
            displayEnemies();
            
            
            displayCharBattleStats();
            
            if( bmode == battleMain ){
                SDL_RenderCopy(gameRender, battleMainMenu , NULL, &battleMainR );
            }
            else if( bmode == battleMagic ){
                SDL_RenderCopy(gameRender, battleMagicMenu , NULL, &battleMainR );
            }
            else if( bmode == battleItem ){
                SDL_RenderCopy(gameRender, battleItemMenu , NULL, &battleMainR );
            }
            else{
                // something may go here.
            }
            
            
            SDL_RenderCopy(gameRender, selecter , NULL, &selecterR );
            
            if( effectdmg != NULL ){
                if( enemies[currentTarget]->boss <= 0 ){
                
                    effectdmgR.x = getEnemyCenterX(enemies[currentTarget]) - effectdmgR.w/2;
                    effectdmgR.y = getEnemyCenterY(enemies[currentTarget]) - effectdmgR.h/2;
                    SDL_RenderCopy(gameRender, effectdmg , NULL, &effectdmgR );
                }
                //SDL_Delay(1000);
            }
            
            SDL_RenderPresent(gameRender);
            
            if( effectdmg != NULL ){
                SDL_DestroyTexture(effectdmg);
                effectdmg = NULL;
                SDL_RenderPresent(gameRender);
                SDL_Delay(1000); // delay so you can see dmg effect
            }
            
            // test if all enemies are dead.
            if( checkDeadEnemies() <= 0 ){
                bmm = 0;
            }
            else{
                 // check if you used your turn and made a mode
                if( usedTurn == 1 ){
                    // TODO: fix this. unforseen consequences. enemy could be null
                    if( enemies[enemyAttackPosition] != NULL ){
                        if( enemies[enemyAttackPosition]->boss > 0 ){
                            bossAttack(enemies[enemyAttackPosition]->boss, gameRender );
                        }
                        else{
                            SDL_RenderCopy(gameRender, enemyAttack , NULL, &enemyAttackRect );
                            SDL_RenderPresent(gameRender);
                            
                            SDL_Delay(1500);
                        }
                    }
                    
                    // get hit by an enemy
                    enemyAttackPosition = getNextEnemy(enemyAttackPosition);
                    mainChar->currentHP -= enemies[ enemyAttackPosition ]->atk;
                    
                    if( checkIfDead() == 1 ){
                        return -1; // will SHUT DOWN EVERYTHING
                    }
                    
                    usedTurn = 0;
                }
            }
        }
    }
    
    clearEnemies();
    
    
    SDL_DestroyTexture(battleMainMenu);
    SDL_DestroyTexture(battleMagicMenu);
    SDL_DestroyTexture(battleItemMenu);
    // SDL_DestroyTexture(battleSelect);
    SDL_DestroyTexture(enemyAttack);

    
    SDL_DestroyTexture(battleback);
    SDL_DestroyTexture(battlestatus);
    
    // add exp to player, if did not run
    if( ranAway != 1 ){
        gain_experience(mainChar, potentialexp );
    }
    return 1;
}

// is this the old one? yes it is
int menumove(int cpos, SDL_Event * event){
    switch ( event->type ) {
        // key pressed down
        case SDL_KEYDOWN:
            switch ( event->key.keysym.sym ) {
                case SDLK_w:
                    if( cpos == 0 ){
                        return 0;
                    }
                    if( cpos == 1 ){
                        return 0;
                    }
                    return 1;
                    break;
                case SDLK_s:
                    if( cpos == 0 ){
                        return 1;
                    }
                    if( cpos == 1 ){
                        return 1;
                    }
                    return 0;
                    break;
                case SDLK_l:
                    if( cpos == 0 ){
                        return -10;
                    }
                    else{
                        return -20;
                    }
                    break;
            }
    }
    return cpos;
}


/*
cpos: current position on menu
maxpos: maximum position number in the menu
event: event in which to find which keys are being pressed
 
returns values between (0, maxpos), which will be the new position in menu
*/
int menuSwitch(int cpos, int maxpos, SDL_Event * event){
    switch ( event->type ) {
            // key pressed down
        case SDL_KEYDOWN:
            switch ( event->key.keysym.sym ) {
                case SDLK_w:
                    if( cpos == 0 ){
                        return 0;
                    }
                    return cpos - 1;
                    break;
                case SDLK_s:
                    if( cpos == maxpos ){
                        return maxpos;
                    }
                    return cpos + 1;
                    break;
                case SDLK_l:
                    // return -50 to tell a selection was made.
                    return -50;
                    break;
            }
    }
    return cpos;
}

void initNewChar(){
    
    mainChar->maxHP = 100;
    mainChar->currentHP = 100;

    mainChar->maxMP = 20;
    mainChar->currentMP = 20;
    
    mainChar->base_ATK = 20;
    mainChar->exp = 0;
    char * sname = "Main Guy";
    char * tname = malloc(sizeof(char) * (strlen(sname) + 1));
    strcpy(tname,sname);
    mainChar->name = tname;
    
    mainChar->base_MAG = 50;
    
    mainChar->level = 1;
    
    printf("name: %s\n", mainChar->name );
}

void initLoadChar(){
    save_block * sb = load_game("save_data/save_data_1.txt");
    
    mainChar->maxHP = sb->HP;
    mainChar->currentHP = sb->HP;
    
    mainChar->maxMP = sb->MP;
    mainChar->currentMP = sb->MP;
    
    mainChar->base_ATK = sb->base_ATK;
    printf("loaded in base attack of %d \n", mainChar->base_ATK );
    mainChar->exp = sb->exp;
    mainChar->name = sb->name;
    
    mainChar->base_MAG = 10;
    
    mainChar->level = sb->level;
    
    free(sb);
}

void destroyChar(){
    free(mainChar->name);
    free(mainChar);
}

void renderObjects(){
    if( objectArray != NULL){
        for( int i = 0; i < numobjects ; i++){
            if( objectArray[i] != NULL ){
                SDL_RenderCopy(gameRender, objectArray[i]->objectT, NULL, &(objectArray[i]->objectR) );
            }
        }
    }
}

int main(int argc, const char * argv[]) {
   
    //test_everything();
    // return 21;
    printf("Game Init\n");
    
    // for main game loop
    int gameRunning = 1;
    // init_game();
    
    gameWindow = SDL_CreateWindow("temp", 200 , 200 , 750 , 500 , SDL_WINDOWPOS_CENTERED );
    
    gameRender = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED );
    
    TTF_Init();
    
    Sans = TTF_OpenFont("fonts/HighlandGothicLightFLF.ttf", 20); //this opens a font style and sets a size
    if( Sans == NULL ){
        printf("could not load fond: extreme error. SHUT DOWN EVERYTHING \n");
    }
    
    SDL_Texture * mainmenu = IMG_LoadTexture( gameRender, "image_files/mainMenu.png");
    
    SDL_Rect mmr;
    mmr.h = 500;
    mmr.w = 750;
    mmr.x = 0;
    mmr.y = 0;
    
    
    SDL_Texture * mms = IMG_LoadTexture( gameRender, "image_files/greenselector.png");
    
    SDL_Rect mmsr;
    mmsr.h = 25;
    mmsr.w = 25;
    mmsr.x = 280;
    mmsr.y = 196;
    
    
    // 196 for start
    // 253 ish for exit
    
    
    // 0 for start, 1 for exit
    int mmpos = 0;
    
    int mm = 1;
    int usedLoad = 0;
    
    // menuloop
    while( mm == 1 ){
        SDL_PollEvent(&keys);
        if( keys.type == SDL_QUIT ){
            mm = 0;
            return 300;
        }
        else{
            
            int testmenu = menuSwitch(mmpos, 2, &keys);
            if( testmenu < 0 ){
                if( mmpos == 0 ){
                    mm = 0;
                }
                else if( mmpos == 1 ){
                    if( checksave("save_data/save_data_1.txt") > 0 ){
                        usedLoad = 1;
                        mm = 0;
                    }
                }
                else{
                    printf("exiting game\n");
                    return 0;
                }
            }
            else{
                mmpos = testmenu;
                
                if( mmpos == 0 ){
                    mmsr.y = 196;
                }
                else if(mmpos == 1 ){
                    mmsr.y = 253;
                }
                else{
                    mmsr.y =305;
                }
                
                
                // printf("mmpos = %d \n " , mmpos );
                
                SDL_RenderClear(gameRender);
                SDL_RenderCopy(gameRender, mainmenu , NULL, &mmr );
                SDL_RenderCopy(gameRender, mms , NULL, &mmsr );
                SDL_RenderPresent(gameRender);
            }
        }
    }
    
    SDL_DestroyTexture(mainmenu);
    SDL_DestroyTexture(mms);
    
    
    mainChar = malloc(sizeof(character));
    
    // start a NEW game
    if( usedLoad == 0 ){
        initNewChar();
    }
    else{
        // load a save game
        initLoadChar();
        printf("level = %d, mag = %d \n", mainChar->level, mainChar->base_MAG);
    }
    
    
    // backgroundIMG = loadImage("image_files/background_image_1.png");
    mGuy = IMG_LoadTexture( gameRender, "image_files/paint_guy.png");
    // ("image_files/background_image_1.png");

    // SDL_Rect
    // ("image_files/background_image_1.png");
    bgRect.h = 500;
    bgRect.w = 750;
    bgRect.x = 0;
    bgRect.y = 0;
    
    loadStage1(1);
    
    // use this to update the frame of reference
    SDL_RenderCopy(gameRender, backgroundIMG , NULL, &bgRect);
    
    
    cLocation = 1;

    
    mGuyRect.h = 100;
    mGuyRect.w = 100;
    mGuyRect.x = 300;
    mGuyRect.y = 400;
    
    
    char_x = 300;
    char_y = 350;
    
    SDL_RenderCopy(gameRender, mGuy , NULL, &mGuyRect );
    
    SDL_RenderPresent(gameRender);
   
    SDL_PollEvent(&keys);
    
    while( gameRunning == 1 ){
        SDL_PollEvent(&keys);
        if( keys.type == SDL_QUIT ){
            gameRunning = 0;
        }
        else{
            // printf("hitting here");
            
            char_movement(&keys);
            SDL_RenderClear(gameRender);
            SDL_RenderCopy(gameRender, backgroundIMG , NULL, &bgRect );
            
            renderObjects();
            
            SDL_RenderCopy(gameRender, mGuy , NULL, &mGuyRect );

            SDL_RenderPresent(gameRender);
            
            if( checkifbattle() == 1 ){
                printf("entering battle mode\n");
                int testAlive = battlemode();
                if( testAlive < 0 ){
                    gameRunning = 0;
                }
                printf("exiting battle mode\n");
                
                char_x = 300;
                char_y = 350;
                
                char_xvel = 0;
                char_yvel = 0;
                
                mGuyRect.x = 300;
                mGuyRect.y = 350;
            }
        }
        // gameRunning = 0;
    }
    
    // returns ms since last check
    // SDL_GetTicks();
    
    SDL_DestroyTexture(backgroundIMG);
    SDL_DestroyTexture(mGuy);
    // free(keys);
    free(objects);
    exit_game();
    
    TTF_Quit();
    
    destroyChar();
    
    printf("Game Closing\n");
    return 0;
}
