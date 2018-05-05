//
//  objects.c
//  Untitled_RPG
//
//  Created by Matthew Broton on 4/13/16.
//  Copyright © 2016 Matthew Broton. All rights reserved.
//

#include "objects.h"

object * createPond(){
    object * c = malloc(sizeof(object));
    
    char * fn = "image_files/pond.png";
    c->fnpath = malloc(strlen(fn + 1)*sizeof(char));
    strcpy(c->fnpath,fn);
    
    c->objectR.x = 450;
    c->objectR.y = 300;
    
    c->objectR.w = 300;
    c->objectR.h = 200;
    
    c->interact = 0; // 100 is for house entry point
    c->interactx = 75;
    c->interacty = 25;
    
    c->solid = 1;
    
    return c;
}



object * createhouse(){
    
    object * c = malloc(sizeof(object));
    
    char * fn = "image_files/house1.png";
    c->fnpath = malloc(strlen(fn + 1)*sizeof(char));
    strcpy(c->fnpath,fn);
    
    c->objectR.x = 10;
    c->objectR.y = 10;
    
    c->objectR.w = 200;
    c->objectR.h = 150;
    
    c->interact = 100; // 100 is for house entry point
    c->interactx = 75;
    c->interacty = 25;
    
    c->solid = 1;
    
    return c;
}

object * createOldGuy(){
    object * c = malloc(sizeof(object));
    
    char * fn = "image_files/oldGuy.png";
    c->fnpath = malloc(strlen(fn + 1)*sizeof(char));
    strcpy(c->fnpath,fn);
    
    c->objectR.x = 750/2;
    c->objectR.y = 50;
    
    c->objectR.w = 100;
    c->objectR.h = 100;
    
    c->interact = 20000; // for the guy to talk about something
    
    c->interactx = 50;
    c->interacty = 50;
    
    c->solid = 1;
    
    return c;
}


object * createHouseExit(){
    object * c = malloc(sizeof(object));

    char * fn = "image_files/cabinexit.png";
    c->fnpath = malloc(strlen(fn + 1)*sizeof(char));
    strcpy(c->fnpath,fn);

    c->objectR.x = 750/2-100;
    c->objectR.y = 400;

    c->objectR.w = 200;
    c->objectR.h = 100;

    c->interact = 101; // cabin exit

    c->interactx = 100;
    c->interacty = 100;

    c->solid = 0;

return c;
}

object * createStage2Trees(){
    
    object * c = malloc(sizeof(object));
    
    char * fn = "image_files/stage2trees.png";
    c->fnpath = malloc(strlen(fn + 1)*sizeof(char));
    strcpy(c->fnpath,fn);
    
    c->objectR.x = 0;
    c->objectR.y = 0;
    
    c->objectR.w = 750;
    c->objectR.h = 150;
    
    c->interact = 0;
    
    c->solid = 1;
    
    return c;
}

object * createTreeDoor(){
    
    object * c = malloc(sizeof(object));
    
    char * fn = "image_files/treeDoor.png";
    c->fnpath = malloc(strlen(fn + 1)*sizeof(char));
    strcpy(c->fnpath,fn);
    
    c->objectR.x = 450;
    c->objectR.y = 0;
    
    c->objectR.w = 200;
    c->objectR.h = 100;
    
    c->solid = 1;
    
    c->interact = 102;
    c->interactx = 75;
    c->interacty = 25;
    
    return c;
}

object * createTreeDoor2(){
    object * c = malloc(sizeof(object));
    
    char * fn = "image_files/treedoor2.png";
    c->fnpath = malloc(strlen(fn + 1)*sizeof(char));
    strcpy(c->fnpath,fn);
    
    c->objectR.x = 0; // fix this other variables.
    c->objectR.y = 200;
    
    c->objectR.w = 100;
    c->objectR.h = 200;
    
    c->solid = 1;
    
    c->interact = 105;
    c->interactx = 75;
    c->interacty = 25;
    
    return c;
}


object * createMountainDoor(){
    
    object * c = malloc(sizeof(object));
    
    char * fn = "image_files/mountainDoor.png";
    c->fnpath = malloc(strlen(fn + 1)*sizeof(char));
    strcpy(c->fnpath,fn);
    
    c->objectR.x = 650; // fix this other variables.
    c->objectR.y = 200;
    
    c->objectR.w = 100;
    c->objectR.h = 200;
    
    c->solid = 1;
    
    c->interact = 200;
    c->interactx = 300;
    c->interacty = 100;
    
    return c;
}

object * createSaveLocation(){
    object * c = malloc(sizeof(object));
    
    char * fn = "image_files/savePoint.png";
    c->fnpath = malloc(strlen(fn + 1)*sizeof(char));
    strcpy(c->fnpath,fn);
    
    c->objectR.x = 250; // fix this other variables.
    c->objectR.y = 25;
    
    c->objectR.w = 100;
    c->objectR.h = 100;
    
    c->solid = 1;
    
    c->interact = 30000;
    c->interactx = 25;
    c->interacty = 25;
    
    return c;
}


/*
 sets the x and y of an object.
*/
void setobjectlocation(int x,int y, object * obj){
    obj->objectR.x = x;
    obj->objectR.y = y;
}


/*
 must destroy the SDL_Texture on their side.
*/
void destroyObject(object * d){
    free(d->fnpath);
    free(d);
}