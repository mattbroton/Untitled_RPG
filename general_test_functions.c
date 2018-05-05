//
//  general_test_functions.c
//  Untitled_RPG
//
//  Created by Matthew Broton on 4/7/16.
//  Copyright © 2016 Matthew Broton. All rights reserved.
//

#include "general_test_functions.h"

int numTests = 0;

int get_number_tests(){
    return numTests;
}

int test_char_functions(){
    numTests ++;
    
    char * rv = NULL;
    
    rv = getLineValue("Name: Guy");
    if( strcmp(rv, "Guy" ) ){
        printf("Error: test_char_functions() -> Name: Guy\n");
        return 0;
    }
    free(rv);
    
    rv = getLineValue("Weapon: Scythe");
    if( strcmp(rv, "Scythe" ) ){
        printf("Error: test_char_functions() -> weapon: Scythe\n");
        return 0;
    }
    free(rv);
    
    rv = getLineValue("Armor: Death Robe");
    if( strcmp(rv, "Death Robe" ) ){
        printf("Error: test_char_functions() -> Death: Robe\n");
        return 0;
    }
    free(rv);
    
    return 1;
}

int test_parse_line(){
    numTests ++;
    
    int rv = readLineData("Level: 79");
    if( rv != 79 ){
        printf("Error: test_parse_line() -> Level: 79\n");
        return 0;
    }
    
    rv = readLineData("Level: 23");
    if( rv != 23 ){
        printf("Error: test_parse_line() -> Level: 23\n");
        return 0;
    }
    
    rv = readLineData("Exp: 987");
    if( rv != 987 ){
        printf("Error: test_parse_line() -> Exp: 987\n");
        return 0;
    }
    
    rv = readLineData("Exp: 209");
    if( rv != 209 ){
        printf("Error: test_parse_line() -> Exp: 209\n");
        return 0;
    }
    
    rv = readLineData("Base_ATK: 5");
    if( rv != 5 ){
        printf("Error: test_parse_line() -> Base:ATK 5\n");
        return 0;
    }
    
    rv = readLineData("Base_DEF: 10");
    if( rv != 10 ){
        printf("Error: test_parse_line() -> Base_DEF: 10\n");
        return 0;
    }
    
    rv = readLineData("asdasdasdasldmadasdaksodasdas: 5");
    if( rv != 0 ){
        printf("Error: test_parse_line() -> random junk \n");
        return 0;
    }
    
    return 1;
}


int test_save_data(){
    numTests ++;
    
    // save_game("save_data_1.txt","Eddard" ,17,12,131);
    // load_game("save_data_1.txt");
    
    return 1;
}

int test_multi(){
    numTests ++;
    
    float rv = check_multiplier(WATER, FIRE);
    if( rv != 2.0 ){
        printf("Error: test_multiplier() -> WATER,FIRE \n");
        return 0;
    }
    
    rv = check_multiplier(LIGHT, DARK);
    if( rv != 3.0 ){
        printf("Error: test_multiplier() -> LIGHT, DARK \n");
        return 0;
    }
    
    rv = check_multiplier(NORMAL, NORMAL);
    if( rv != 1.0 ){
        printf("Error: test_multiplier() -> LIGHT, DARK \n");
        return 0;
    }
    
    return 1;
}

int test_grab_save_fn(){
    numTests ++;
    
    char * rv = get_save_name(1);
    if( strcmp(rv, "save_data/save_data_1.txt") ){
        printf("Error: test_grab_save_fn() -> 1 \n");
        return 0;
    }
    
    rv = get_save_name(2);
    if( strcmp(rv, "save_data/save_data_2.txt") ){
        printf("Error: test_grab_save_fn() -> 2 \n");
        return 0;
    }
    
    rv = get_save_name(3);
    if( strcmp(rv, "save_data/save_data_3.txt") ){
        printf("Error: test_grab_save_fn() -> 3 \n");
        return 0;
    }
    
    rv = get_save_name(7);
    if( rv != NULL ){
        printf("Error: test_grab_save_fn() -> rando \n");
        return 0;
    }
    
    return 1;
}



int test_all_calls(){
    int numPassed = 0;
    
    numPassed += test_parse_line();
    numPassed += test_char_functions();
    numPassed += test_multi();
    numPassed += test_grab_save_fn();
    
    if( numPassed == numTests ){
        printf("All Tests have Succeded\n");
    }
    else{
        printf("Something has gone terribly wrong\n");
    }
    
    return numPassed;
}