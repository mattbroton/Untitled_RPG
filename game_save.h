//
//  game_save.h
//  Untitled_RPG
//
//  Created by Matthew Broton on 4/7/16.
//  Copyright © 2016 Matthew Broton. All rights reserved.
//

#ifndef game_save_h
#define game_save_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #include "character_info.c"

typedef struct{
    char * name; // must be <= MAX_CHAR_NAME
    int level;
    int exp;
    char * weapon; // must be <= MAX_WEP_NAME
    char * armor; // must be <= MAX_ARMOR_NAME
    
    int base_ATK;
    int base_DEF;
    int base_MAG;
    
    int HP;
    int MP;
    
} save_block;


#endif /* game_save_h */

int checksave(char * save_data_fn);

int save_game(char * save_data_fn , char * name, int level, int exp, int saveLocation, int hp, int mp, int atk, int mag);

save_block * load_game(char * save_data_fn);

char * get_save_name(int save_num);

int delete_save(char * save_data_fn);

int readLineData(char * ln);

char * getLineValue(char * ln);