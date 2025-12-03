//=================================================================
// Implementation of ENEMY module.
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================

#include "enemy_private.h"
#include "doubly_linked_list.h"
#include "enemy_public.h"
#include <cstdlib>

int ENEMY_tick=0;

//Create a DLL for ENEMYs
DLinkedList* ENEMYDLL = NULL;
DLinkedList* projectileDLL = NULL;  //List for projectiles launched by enemies
//Reminder that Player fires missiles. Enemies fire projectiles



void enemy_init(void)
{
//Create a new doubly linked list of ENEMYs
    ENEMYDLL = create_dlinkedlist();
    projectileDLL = create_dlinkedlist();
}

void enemy_generator(void){
    ENEMY_tick++;
    // only fire the ENEMY at certain ticks
    if((ENEMY_tick % ENEMY_INTERVAL)==0 || ENEMY_tick==0){
        //printf("ENEMY_create()");
        enemy_create();
    }        
    // update the ENEMYs and draw them
    enemy_update_position();
    projectile_update_position();
}

void enemy_create(void){
    ENEMY* M = (ENEMY*)malloc(sizeof(ENEMY));
    // M->y = 0;
    //each ENEMY has its own tick
    M->type = rand() % 3;
    switch (M->type)
    {
    case 0:
        M->draw = draw_red;
        break;
    case 1:
        M->draw = draw_orange;
        break;
    case 2:
        M->draw = draw_green;
        break;
    default:
        break;
    }
    M->tick = rand();
    //set a random source for the ENEMY
    M->direction = rand() % 3;
    if (M->direction == 0){
        M->source = rand() % (SIZE_X - ENEMY_SIZE - PLAYER_SPACE);
    //set a random target for the ENEMY
        M->target = rand() % (SIZE_X - ENEMY_SIZE - PLAYER_SPACE);
    //the ENEMY starts at its source
        M->box.topLeft.x = M->source + PLAYER_SPACE;
        M->box.topLeft.y = 0; // = {M->source + PLAYER_SPACE, 0};
        M->box.bottomRight.x = M->source + ENEMY_SIZE + PLAYER_SPACE + 5;
        M->box.bottomRight.y = ENEMY_SIZE + 5;
        //M->box.bottomRight = {M->source + ENEMY_SIZE + PLAYER_SPACE, ENEMY_SIZE};
        double diagnal = sqrt((M->source - M->target)*(M->source - M->target) + SIZE_Y*SIZE_Y);
        M->delta_x = (M->target - M->source) / diagnal;
        M->delta_y = fabs(SIZE_Y / diagnal);
        M->hit = 3;
    }
    else if(M->direction == 1){
        M->source = rand() % (SIZE_Y - ENEMY_SIZE - 50); // 50 is bound size
    //set a random target for the ENEMY
        M->target = rand() % (SIZE_Y - ENEMY_SIZE - 50);
        M->box.topLeft.x = PLAYER_SPACE;
        M->box.topLeft.y = M->source;
        //M->box.topLeft = {PLAYER_SPACE, M->source};
        M->box.bottomRight.x = PLAYER_SPACE + ENEMY_SIZE + 3;
        M->box.bottomRight.y = M->source + ENEMY_SIZE + 3;
        //M->box.bottomRight = {PLAYER_SPACE + ENEMY_SIZE, M->source + ENEMY_SIZE};
        double diagnal = sqrt((M->source - M->target)*(M->source - M->target) + (SIZE_X - PLAYER_SPACE)*(SIZE_X - PLAYER_SPACE));
        M->delta_x = (SIZE_X - PLAYER_SPACE) / diagnal; 
        M->delta_y = fabs((M->target - M->source) / diagnal);
        M->hit = 2; 
    }else{
        M->source = rand() % (SIZE_Y - ENEMY_SIZE - 50);
    //set a random target for the ENEMY
        M->target = rand() % (SIZE_Y - ENEMY_SIZE - 50);
        M->box.topLeft.x = PLAYER_SPACE + SIZE_X - ENEMY_SIZE;
        M->box.topLeft.y = M->source;
        //M->box.topLeft = {PLAYER_SPACE + SIZE_X - ENEMY_SIZE, M->source};
        M->box.bottomRight.x = PLAYER_SPACE + SIZE_X;
        M->box.bottomRight.y = M->source + ENEMY_SIZE;
        //M->box.bottomRight = {PLAYER_SPACE + SIZE_X, M->source + ENEMY_SIZE};
        double diagnal = sqrt((M->source - M->target)*(M->source - M->target) + (SIZE_X - PLAYER_SPACE)*(SIZE_X - PLAYER_SPACE));
        M->delta_x = (PLAYER_SPACE - SIZE_X) / diagnal; 
        M->delta_y = fabs((M->target - M->source) / diagnal);
        M->hit = 1; 
    }
        
    
    M->status = ENEMY_ACTIVE;
    
    insertHead(ENEMYDLL, M);
}

void enemy_update_position(void){

    
    //controls how fast the ENEMY will move
    int rate = ENEMY_SPEED;
    //delta_x and delta_y account for the slope of the ENEMY
    DrawFunc draw = NULL;
    LLNode* current = ENEMYDLL->head;
    ENEMY* newENEMY;
    //iterate over all ENEMYs
    while(current)
    {   newENEMY = (ENEMY*) current->data;
        if(newENEMY->status == ENEMY_DESTROYED ||
            newENEMY->box.topLeft.x > 127 ||
            newENEMY->box.topLeft.y > 127 ||
            newENEMY->box.bottomRight.x < 0   ||
            newENEMY->box.bottomRight.y < 0)
        {
            //cover the last ENEMY location
            draw_nothing(newENEMY->box);
            // clear the ENEMY on the screen
            draw = NULL;           
            // Remove it from the list
            //printf("deleting ENEMY node...\n");
            deleteNode(ENEMYDLL, current);
            //printf("ENEMY node deleted.\n");
        }
        else 
        {
            //cover the last ENEMY location
            draw_nothing(newENEMY->box);

            // update ENEMY position
            
            //printf("%f, %f\n", newENEMY->delta_x, newENEMY->delta_y);
            newENEMY->box.topLeft.x += rate*newENEMY->delta_x;
            newENEMY->box.topLeft.y += rate*newENEMY->delta_y;
            newENEMY->box.bottomRight.x += rate*newENEMY->delta_x;
            newENEMY->box.bottomRight.y += rate*newENEMY->delta_y;
            //printf(" %f, %f", newENEMY->delta_x, newENEMY->delta_y);
            // draw ENEMY
            draw = newENEMY->draw;
            //update ENEMY's internal tick
            newENEMY->tick++;


            if(newENEMY->tick % 50 == 0) {
                enemy_fire(newENEMY);
            } 
            

    
        }       
        // Advance the loop
        if(draw) draw(newENEMY->box);
        current = current->next;
    }
}




/*
In this file, you need to create functions that deal with enemies and their projectiles.
You need to make methods that make enemies shoot projectiles randomly,
    Update projectile positions
    Delete Projectiles and Enemies as needed
    and other things as needed
    
You can choose to implement the below methods or make your own new methods
*/

//TODO: make a function that make a new Projectile that fires from the enemy. Make sure that you add it to the list of PROJECTILES
// You should only call this to just launch the projectile. You can use enemy.tick or anything else to simulate randomness
// This method also should insert the projectile into the list after initializng a projectile
void enemy_fire(ENEMY* enemy) {
    // Create a projectile
    PROJECTILE* projectile = (PROJECTILE*)malloc(sizeof(PROJECTILE));     // new PROJECTILE;

    // Assign the projectile to the enemy, by placing it at the same location 
    projectile->box.topLeft.x = enemy->box.topLeft.x;
    projectile->box.topLeft.y = enemy->box.topLeft.y;
    projectile->box.bottomRight.x = projectile->box.topLeft.x + PROJ_WID;
    projectile->box.bottomRight.y = projectile->box.topLeft.y - PROJ_LEN;
    projectile->delta_y = 2;

    insertHead(projectileDLL, projectile);
    return;
}

// TODO: Update projectile position  for all projectiles. May require an iterative approach. Make sure you are checking for LCD bounds.
// This method also updates the Projectile List by updating the projectile parameters
// This is called every iteration of the game loop to update the projectile positions and delete projectiles as needed
void projectile_update_position() {  
    LLNode* current = projectileDLL->head;
    PROJECTILE* projectile;
    while(current) {
        LLNode* next = current->next;
        projectile = (PROJECTILE*) current->data;
        if (projectile->box.topLeft.y > 127 ||
            projectile->box.bottomRight.y > 127 ||

            projectile->box.topLeft.y < 0 ||
            projectile->box.bottomRight.y < 0) {
                draw_nothing(projectile->box);
                deleteNode(projectileDLL, current); 
        } else {
            draw_nothing(projectile->box);
            projectile->box.topLeft.y += projectile->delta_y;
            projectile->box.bottomRight.y += projectile->delta_y;
            uLCD.filled_rectangle(projectile->box.topLeft.x, projectile->box.topLeft.y, projectile->box.bottomRight.x, projectile->box.bottomRight.y, RED);
        }
        current = next;
    }
    return; 
}


//TODO:  Return Enemy List. This method also updates the enemy list
// Remember that you also need to delete DESTROYED enemies from the list. You can do this here or somewhere else.
DLinkedList* get_enemy_list() {
    LLNode* current = getHead(ENEMYDLL);
    for (int i = 0; i < getSize(ENEMYDLL); i++) {
        ENEMY* enemy = (ENEMY*) getData(current);
        if (enemy->status == ENEMY_DESTROYED) {
            deleteNode(ENEMYDLL, current);
        }
        current = getNext(current);
    }
    return ENEMYDLL;
}

//TODO: Return Projectile List.
DLinkedList* get_projectile_list() {
    return projectileDLL;
}

// Remember that you also need to delete enemies