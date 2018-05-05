//
//  game_save.c
//  Untitled_RPG
//
//  Created by Matthew Broton on 4/7/16.
//  Copyright © 2016 Matthew Broton. All rights reserved.
//

#include "game_save.h"


/*
 There will be 3 save data possible. If 
 
 RETURNS ON STACK PROBABLY SO DANGEROUS. REMEMBER TO CHANGE THIS.
*/
char * get_save_name(int save_num){
    if( save_num == 1 ){
        return "save_data/save_data_1.txt";
    }
    else if( save_num == 2){
        return "save_data/save_data_2.txt";
    }
    else if( save_num == 3){
        return "save_data/save_data_3.txt";
    }
    else{
        return NULL;
    }
}

/*
    given file name, checks if save data exists.
    0 for no. 1 for yes.
*/
int checksave(char * save_data_fn){
    if( access( save_data_fn , F_OK ) != -1 ) {
        // file exists
        return 1;
    }
    return 0;
}

/*
 Given character data, it will save the data into a text file.
 
 Returns > 0 if succesful.
 */
int save_game(char * save_data_fn , char * name, int level, int exp, int saveLocation, int hp, int mp, int atk, int mag){
    
    FILE * fn;
    
    fn = fopen(save_data_fn, "w" );
    
    if( fn == NULL ){
        printf("Cannot create file named: %s\n",save_data_fn);
        return -20;
    }
    
    fprintf(fn,"Name: %s\n",name);
    fprintf(fn,"Level: %d\n", level);
    fprintf(fn,"Exp: %d\n",exp);
    
    fprintf(fn,"maxHP: %d\n",hp);
    fprintf(fn,"maxMP: %d\n",mp);
    fprintf(fn,"base_ATK: %d\n",atk);
    fprintf(fn,"base_MAG: %d\n",mag);
    
    // fprintf(fn,"SaveLocation: %d\n", saveLocation);

    //fprintf(fn,"Weapon: %s\n");
    //fprintf(fn,"Armor: %s\n");
    
    fclose(fn);
    
    return 7;
}

/*
 Given save data file name, it will read the text file and load in the appropriate data.
 
 Returns > 0 if succesful.
 */
save_block * load_game(char * save_data_fn){
    
    FILE * fn;
    
    fn = fopen(save_data_fn, "r+" );
    
    if( fn == NULL ){
        printf("Cannot Open file named: %s\n",save_data_fn);
        return NULL;
    }
    
    save_block * c = malloc(sizeof(save_block));
    
    char line[256];
    
    int i = -1;
    while( fgets(line, sizeof(line), fn) ){
        if( i == -1 ){
            c->name = getLineValue(line);
        }
        if( i == 0 ){
            c->level = readLineData(line);
        }
        else if( i == 1 ){
            c->exp = readLineData(line);

        }
        else if( i == 2){
            c->HP = readLineData(line);

        }
        else if( i == 3){
            c->MP = readLineData(line);

        }
        else if( i == 4){
            c->base_ATK = readLineData(line);

        }
        else{
            c->base_MAG = readLineData(line);
        }
        i++;
    }
    
    
    fclose(fn);
    
    return c;
}

int readLineData(char * ln){
    if(strstr(ln, "Level:") != NULL) {
        return atoi(ln + 7);
    }
    else if(strstr(ln, "Exp:") != NULL) {
        return atoi(ln + 5);
    }
    else if(strstr(ln, "maxHP:") != NULL) {
        return atoi(ln + 7);
    }
    else if(strstr(ln, "maxMP:") != NULL) {
        return atoi(ln + 7);
    }
    else if(strstr(ln, "base_ATK:") != NULL) {
        return atoi(ln + 10);
    }
    else if(strstr(ln, "base_MAG:") != NULL) {
        return atoi(ln + 10);
    }
    else{
        return 0;
    }
    return 0;
}

char * getLineValue(char * ln){
    char * rv = NULL;
    if(strstr(ln, "Name:") != NULL){
        rv = malloc( (strlen(ln) - 6 + 1 )*sizeof(char));
        strcpy(rv, ln + 6 );
    }
    else if (strstr(ln, "Weapon:") != NULL){
        rv = malloc( (strlen(ln) - 8 + 1 )*sizeof(char));
        strcpy(rv, ln + 8 );
    }
    else if( strstr(ln, "Armor:") != NULL){
        rv = malloc( (strlen(ln) - 7 + 1 )*sizeof(char));
        strcpy(rv, ln + 7 );
    }
    else{
        return NULL;
    }
    return rv;
}

int delete_save(char * save_data_fn){
    int rv = remove(save_data_fn);
    
    if( rv != 0 ){
        printf("Could not delete save file : %s\n", save_data_fn);
        return 0;
    }
    
    return 1;
}