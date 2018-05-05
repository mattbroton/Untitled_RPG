//
//  objects.h
//  Untitled_RPG
//
//  Created by Matthew Broton on 4/13/16.
//  Copyright © 2016 Matthew Broton. All rights reserved.
//

#ifndef objects_h
#define objects_h

#include <stdio.h>

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"

typedef struct{
    char * fnpath; // name of object. also the path to it.
    
    SDL_Texture * objectT;
    SDL_Rect objectR;
    
    int solid; // 0 for no. 1 for yes. detect if you can go through it or not
    
    int interact; // 0 for no or 1 for yes.
    /*
        Between 10 000 and 20 000
            save points:
                    10 000. outside cabin save point
    */
    
    // top left corner of door. must be within 50 or so pixels of it to get in. click x to enter.
    int interactx;
    int interacty;
}object;


object * createhouse();
object * createPond();
object * createSaveLocation();
object * createTreeDoor();
object * createOldGuy();
object * createHouseExit();
object * createMountainDoor();
object * createTreeDoor2();
object * createStage2Trees();




void setobjectlocation(int x,int y, object * obj);
void destroyObject(object * d);


#endif /* objects_h */
