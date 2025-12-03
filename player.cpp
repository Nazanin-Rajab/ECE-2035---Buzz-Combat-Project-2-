//=================================================================
// Implementation for the player module
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================
#include "graphics.h"
#include "player_public.h"
#include "hardware.h"
#include <cstdio>
#include <cstdlib>

PLAYER player; // structure of player


// Complete the function below
PLAYER player_get_info(void){   
    return player; 
}

void player_init(void) {  
    player.score = 0;
    player.player_has_missile = false;
    player.x = 60; 
    player.y = 90; 
    player.height = 11; 
    player.missile_length = 6;
    player.strikes = 7; 
    player.multiplier = 1;
    player.missile_deltay = 5;
    player.missile_deltax = 0;
    player.type = 0; 

    draw_sprit(player.x, player.y, player.height);
}

// player_draw in main loop
void player_moveUp(void) { 
    if (player.y < 127 && player.y > 0) {
        uLCD.filled_rectangle(player.x, player.y, player.x + player.height, player.y + player.height, 0);
        player.y = player.y + 1;
    } 
}

void player_moveDown(void) { 
    if (player.y < 127 && player.y > 0) {
        uLCD.filled_rectangle(player.x, player.y, player.x + player.height, player.y + player.height, 0);
        player.y = player.y - 1;
    }
}

void player_moveLeft(void) { 
    if (player.x < 127 && player.x > 0) {
        uLCD.filled_rectangle(player.x, player.y, player.x + player.height, player.y + player.height, 0);
        player.x = player.x - 1; 
    }
}

void player_moveRight(void) { 
    if (player.x < 127 && player.x > 0) {
        uLCD.filled_rectangle(player.x, player.y, player.x + player.height, player.y + player.height, 0);
        player.x = player.x + 1;
    }
}

void player_throw(void) { 
    player.player_has_missile = 0;
    player.missile_boundingBox.topLeft.x = player.x; // set the coordinates of the missile  
    player.missile_boundingBox.topLeft.y = player.y;
    player.missile_boundingBox.bottomRight.x = player.missile_boundingBox.topLeft.x + 4;
    player.missile_boundingBox.bottomRight.y = player.missile_boundingBox.topLeft.y + 6;
    
    player.type = rand() % 3;
    if (player.type == 2) {
        player.type = 2;
    } else if (player.type == 1) {
        player.type = 1;
    } else {
        player.type = 0;
    }
}

void player_missile_draw(bool) {
    if (player.player_has_missile == 0) {
     if (player.missile_boundingBox.topLeft.y > 0 && 
        player.missile_boundingBox.topLeft.x > 0 && 
        player.missile_boundingBox.topLeft.x + player.height < 127) { 

        draw_nothing(player.missile_boundingBox);
        player.missile_boundingBox.topLeft.y = player.missile_boundingBox.topLeft.y - player.missile_deltay;
        player.missile_boundingBox.bottomRight.y = player.missile_boundingBox.bottomRight.y - player.missile_deltay;
        
        if (player.type == 2) {
            missile_arrow();
        } else if (player.type == 1) {
            missile_bomb();
        } else {
            draw_blueberry(player.missile_boundingBox);
        }
        

    } else { 
        player_missile_return();
        bool hit = false;
        player_multiplier(hit); 
        player.strikes--;
        
    }
   }
}

void player_missile_return(void) {
    draw_nothing(player.missile_boundingBox);
    player.missile_boundingBox.topLeft.x = player.x; // missle comes back to player position 
    player.missile_boundingBox.topLeft.y = player.y;
    player.missile_boundingBox.bottomRight.x = player.x + 4;
    player.missile_boundingBox.bottomRight.y = player.y + 6; 

    player.player_has_missile = 1;
}

void player_draw(int color) {
    draw_sprit(player.x, player.y, player.height); 

}

void player_score_inc(void) {
    player.score = player.score + player.multiplier;
}

void player_multiplier(bool hit) {
    if (hit) {
        player.multiplier = 5; 
    } else {
        player.multiplier = 1; 
    }
}

void missile_arrow(void) {
    // player.missile_deltax = 7;
    player.missile_deltay = 7;
    draw_arrow(player.missile_boundingBox);
}

void missile_bomb(void) { 
    player.missile_deltay = 5; 
    draw_bomb(player.missile_boundingBox);

}
