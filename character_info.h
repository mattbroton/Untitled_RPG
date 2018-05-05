//
//  character_info.h
//  Untitled_RPG
//
//  Created by Matthew Broton on 4/7/16.
//  Copyright © 2016 Matthew Broton. All rights reserved.
//

#ifndef character_info_h
#define character_info_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "game_save.h"

typedef struct{
    char * name; // must be <= MAX_CHAR_NAME
    int level;
    int exp;
    char * weapon; // must be <= MAX_WEP_NAME
    char * armor; // must be <= MAX_ARMOR_NAME
    
    int base_ATK;
    int base_DEF;
    int base_MAG;
    
    int currentHP;
    int maxHP;
    
    int currentMP;
    int maxMP;
    
    
} character;

#define MAX_CHAR_NAME 6
#define MAX_WEP_NAME 20
#define MAX_ARMOR_NAME 20

#define MAX_EXP 1000000


/*
 Array used for calculating what level you should be.
 
 causes some sort of error in xcode. Check what it is later.
 */
// long exp_array[] = {0, 10, 30};

/*
 Given a character, it will gain that much experience on top of what it already has.
 Checks if it is higher than MAX_EXP and will then set it to MAX_EXP
 */
void gain_experience(character * main_character, int exp);

void init_character(character * main_character);

void set_name(char * inputName, character * main_character);

void change_weapon(character * main_character ,char * newWeapon);

void change_armor(character * main_character, char * newArmor);

void exit_character(character * main_character);




#endif /* character_info_h */
