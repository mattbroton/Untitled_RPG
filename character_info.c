//
//  character_info.c
//  Untitled_RPG
//
//  Created by Matthew Broton on 4/7/16.
//  Copyright © 2016 Matthew Broton. All rights reserved.
//

#include "character_info.h"


// static means private




void init_character(character * main_character){
    main_character= malloc(sizeof(character));
    
    main_character = malloc( (MAX_CHAR_NAME + 1) * sizeof(char) );
    main_character->weapon = malloc( (MAX_WEP_NAME + 1)* sizeof(char) );
    main_character->armor = malloc( (MAX_ARMOR_NAME + 1)* sizeof(char) );
}

void set_name(char * inputName, character * main_character){
    strcpy(main_character->name,inputName);
}

void change_weapon(character * charName ,char * newWeapon){
    strcpy(charName->weapon,newWeapon);
}

void change_armor(character * charName,char * newArmor){
    strcpy(charName->armor,newArmor);
}

void gain_experience(character * charName, int exp){
    charName->exp += exp;
    
    while( exp >= 25 ){
        charName->level += 1;
        charName->base_ATK += 10;
        charName->base_MAG += 10;
        charName->maxHP += 20;
        charName->maxMP += 1;
        
        exp -= 25;
    }
}

void exit_character(character * charName){
    
    free( charName->name );
    free( charName->weapon);
    free( charName->armor );
    
    free(charName);
}