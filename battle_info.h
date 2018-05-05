//
//  battle_info.h
//  Untitled_RPG
//
//  Created by Matthew Broton on 4/6/16.
//  Copyright © 2016 Matthew Broton. All rights reserved.
//

#ifndef battle_info_h
#define battle_info_h

#include <stdio.h>
#include "enemy.h"

#define NORMAL 0
#define FIRE 1
#define WATER 2
#define ICE 3
#define THUNDER 4
#define LIGHT 5
#define DARK 6
#define POISON 7

#endif /* battle_info_h */


/*
 Returns float multiplier to see if atk is strong/weak againt def.
 */
float check_multiplier(int atk_type, int def_type);