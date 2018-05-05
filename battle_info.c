//
//  battle_info.c
//  Untitled_RPG
//
//  Created by Matthew Broton on 4/6/16.
//  Copyright © 2016 Matthew Broton. All rights reserved.
//

#include "battle_info.h"


#include <stdio.h>

/*
 Definitions for the different atk and def types.
 
 Normal: no particular element association
 */


/*
 Returns float multiplier to see if atk is strong/weak againt def.
 */
float check_multiplier(int atk_type, int def_type){
    
    if( atk_type == NORMAL ){
        if( def_type == NORMAL ){
            return 1.0;
        }
    }
    else if( atk_type == FIRE ){
        if( def_type == ICE ){
            return 1.2;
        }
    }
    else if( atk_type == WATER ){
        if( def_type == FIRE ){
            return 2.0;
        }
    }
    else if( atk_type == LIGHT ){
        if( def_type == DARK ){
            return 3.0;
        }
    }
    else if( atk_type == POISON ){
        if( def_type == NORMAL ){
            return 2.0;
        }
    }
    else{
        // do nothing
    }
    return 1.0;
}