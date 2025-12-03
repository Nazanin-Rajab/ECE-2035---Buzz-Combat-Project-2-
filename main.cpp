//=================================================================
// The main program file.
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================

// External libs
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>

// Use these macros if you wanna read timer values
#define timer_read_f(x)     chrono::duration<float>((x).elapsed_time()).count()
#define timer_read_s(x)     chrono::duration_cast<chrono::seconds>((x).elapsed_time()).count();
#define timer_read_ms(x)    chrono::duration_cast<chrono::milliseconds>((x).elapsed_time()).count()
#define timer_read_us(x)    (x).elapsed_time().count()

// Project includes
//#include "enemy_private.h"
#include "globals.h"
#include "hardware.h"
#include "enemy_public.h"
#include "player_public.h"

//#include "SDBlockDevice.h"
//#include "FATFileSystem.h"


// ===User implementations start===
int enemy_contact(void); // iterate through enemys and see if any collided
int projectile_contact(void);   // see if any missiles have hit
void set_random_seed(Timer);

void save_game(const char *filename, const char *game_state);

int main()
{
    GameInputs inputs; 

    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    printf("Program Starting\n");

    // ===User implementations start===
    // Game state variables
    // Timer to measure game update speed (secondarily used to generate random seed)
    Timer t;
    int dt; // delta time
    // set seed for random enemy generation
    set_random_seed(t);
 
    
    //initialize functions
    enemy_init();
    // player_init();        
    printf("Initialization complete\n");
    // uLCD.baudrate(3000000);

    
    /*

    You will need to make helper methods to help out with the game objective
    You should have methods that deal with drawing the missile, checking for missile contact,
    checking for projectile contact, and any others as you see fit.

    The shell code is just a guideline. You can edit the code in any way you see fir   
       
    
    */
    
     
    while(1) {
        uLCD.locate(0, 0);
        uLCD.printf("BUZZ COMBAT: Hive Deffense\n");
        uLCD.printf("Press the start\nbutton to start the game!\n");
        inputs = read_inputs();
            if (inputs.start) {
                uLCD.cls();
                break;
        }
    }
    
    player_init();  

    while(1)
    {

        // <-----------Uncomment this
        t.start();

        // Draw enemys first
        enemy_generator();

        //Put score on screen
        // NOTE: Text is 8 pixels tall
        PLAYER player = player_get_info();

        player_draw(0x0000FF); 
        // print status
        uLCD.locate(0, 0);
        uLCD.printf("Player Score: %d", player.score);
        // uLCD.filled_rectangle(0, 0, 120, 6, BLACK);
        
        
        // read inputs
        inputs = read_inputs();
        // printf("player motions\n");
        if (inputs.up) player_moveUp(); 
        // fill the rest of it out           
        if (inputs.down) player_moveDown();
        if (inputs.left) player_moveLeft();
        if (inputs.right) player_moveRight();

        if (inputs.pause) { 
            while (1) {
                uLCD.locate(0, 0);
                uLCD.printf("The game has been paused!\n");
                uLCD.printf("Press the same button to restart the game!\n");
                inputs = read_inputs();
                if (inputs.start) {
                    uLCD.cls();
                    break; 
                }
            }
        }
        // TODO: Write helper methods as needed  
        DLinkedList* enemyList = get_enemy_list(); // pointer to the list of enemy
        LLNode* current = getHead(enemyList); // pointer to the head of the enemy list

        DLinkedList* projectileList = get_projectile_list(); // pointer to the list of projectile
        LLNode* currentPro = getHead(projectileList); // pointer to the head of the projectile list

        bool playerHitbyEnemy = false; 
        bool projectileHitPlayer = false; 
        bool missileHitEnemy = false; 
    
        DrawFunc draw = NULL; 

        // If button pressed the missile is thrown 
        if (inputs.fire) { // usign button 1
            player_throw();
        }
        player_missile_draw(true);

        // TODO: Check if missile hit enemy
        current = getHead(enemyList);
        while (current) {
            ENEMY* enemy = (ENEMY*) current->data;
            if (enemy->box.topLeft.x < player.missile_boundingBox.topLeft.x && 
                enemy->box.bottomRight.x > player.missile_boundingBox.topLeft.x && 
                enemy->box.topLeft.y < player.missile_boundingBox.topLeft.y && 
                enemy->box.bottomRight.y > player.missile_boundingBox.topLeft.y) {
                    if (enemy->hit == 0) {
                        missileHitEnemy = true;
                        printf("enemy die \n");
                        enemy->status = ENEMY_DESTROYED;
                        draw_nothing(enemy->box);
                        draw = NULL;
                        deleteNode(enemyList, current);
                        break;
                    } else {
                        printf("enemy hit\n");
                        enemy->hit = enemy->hit - 1; 
                        player_missile_return();
                        break;
                    }
                    
            } else if (enemy->box.topLeft.x < player.missile_boundingBox.bottomRight.x && 
                    enemy->box.bottomRight.x > player.missile_boundingBox.bottomRight.x && 
                    enemy->box.topLeft.y < player.missile_boundingBox.bottomRight.y && 
                    enemy->box.bottomRight.y > player.missile_boundingBox.bottomRight.y) {
                        if (enemy->hit == 0) {
                            missileHitEnemy = true;
                            printf("enemy die \n");
                            enemy->status = ENEMY_DESTROYED;
                            draw_nothing(enemy->box);
                            draw = NULL;
                            deleteNode(enemyList, current);
                            break;
                        } else {
                            printf("enemy hit\n");
                            enemy->hit = enemy->hit - 1; 
                            player_missile_return();
                            break;
                        }
            } else if (enemy->box.topLeft.x < player.missile_boundingBox.topLeft.x &&
                    enemy->box.bottomRight.x > player.missile_boundingBox.topLeft.x && 
                    enemy->box.topLeft.y < player.missile_boundingBox.bottomRight.y &&
                    enemy->box.bottomRight.y > player.missile_boundingBox.bottomRight.y) {
                        if (enemy->hit == 0) {
                            missileHitEnemy = true;
                            printf("enemy die \n");
                            enemy->status = ENEMY_DESTROYED;
                            draw_nothing(enemy->box);
                            draw = NULL;
                            deleteNode(enemyList, current);
                            break;
                        } else {
                            printf("enemy hit\n");
                            enemy->hit = enemy->hit - 1;
                            player_missile_return(); 
                            break;
                        }
            } else if (enemy->box.topLeft.x < player.missile_boundingBox.bottomRight.x &&
                    enemy->box.bottomRight.x > player.missile_boundingBox.bottomRight.x && 
                    enemy->box.topLeft.y < player.missile_boundingBox.topLeft.y &&
                    enemy->box.bottomRight.y > player.missile_boundingBox.topLeft.y) {
                        if (enemy->hit == 0) {
                            missileHitEnemy = true;
                            printf("enemy die \n");
                            enemy->status = ENEMY_DESTROYED;
                            draw_nothing(enemy->box);
                            draw = NULL;
                            deleteNode(enemyList, current);
                            break;
                        } else {
                            printf("enemy hit\n");
                            enemy->hit = enemy->hit - 1; 
                            player_missile_return();
                            break;
                        }
            } 
            current = getNext(current);
        }
        if (missileHitEnemy == true) {
            player_score_inc(); 
            player_multiplier(missileHitEnemy); 
            player_missile_return();
        }

        // If the player has lost all its strikes the game ends
        if (player.strikes <= 0) { 
            uLCD.cls();
            while(1) {
                uLCD.locate(3, 3);
                uLCD.printf("you Lost All of Your Strikes!");
            }
        }
        
        // TODO: Check if game won
        //      If player has reached 50 points, player wins the game
        if (player.score >= 50) {
            uLCD.cls();
            while(1) {
                uLCD.locate(3, 3);
                uLCD.printf("you won");
            }
        }

        // TODO: Check if enemy has hit the player 
        //      or projectile has hit the player
        current = getHead(enemyList);
        while(current || currentPro) {
            if (current) {
                ENEMY* enemy = (ENEMY*) current->data;
                if (player.x > enemy->box.topLeft.x &&
                    player.x < enemy->box.bottomRight.x &&
                    player.y > enemy->box.topLeft.y &&
                    player.y < enemy->box.bottomRight.y) {
                        playerHitbyEnemy = true;
                        break;

                } else if (player.x + player.height > enemy->box.topLeft.x &&
                        player.x + player.height < enemy->box.bottomRight.x &&
                        player.y + player.height > enemy->box.topLeft.y &&
                        player.y + player.height < enemy->box.bottomRight.y) {
                            playerHitbyEnemy = true;
                            break;

                } else if (player.x > enemy->box.topLeft.x &&
                        player.x < enemy->box.bottomRight.x &&
                        player.y + player.height > enemy->box.topLeft.y &&
                        player.y + player.height < enemy->box.bottomRight.y) {
                            playerHitbyEnemy = true;
                            break;

                } else if (player.x + player.height > enemy->box.topLeft.x &&
                        player.x + player.height < enemy->box.bottomRight.x &&
                        player.y > enemy->box.topLeft.y &&
                        player.y < enemy->box.bottomRight.y) { 
                            playerHitbyEnemy = true;
                            break;

                } 
                current = getNext(current);
            }
            if (currentPro) {
                PROJECTILE* projectile = (PROJECTILE*) getData(currentPro);
                if (player.x < projectile->box.bottomRight.x &&
                    player.x + player.height > projectile->box.bottomRight.x &&
                    player.y < projectile->box.bottomRight.y &&
                    player.y + player.height > projectile->box.bottomRight.y) {
                        projectileHitPlayer = true;
                        break;

                } else if (player.x > projectile->box.topLeft.x &&
                        player.x + player.height < projectile->box.topLeft.x &&
                        player.y > projectile->box.topLeft.y &&
                        player.y + player.height < projectile->box.topLeft.y) {
                            projectileHitPlayer = true;
                            break;

                } else if (player.x < projectile->box.topLeft.x &&
                        player.x + player.height > projectile->box.topLeft.x &&
                        player.y < projectile->box.bottomRight.y &&
                        player.y + player.height > projectile->box.bottomRight.y) {
                            projectileHitPlayer = true;
                            break;

                } else if (player.x < projectile->box.bottomRight.x &&
                        player.x + player.height > projectile->box.bottomRight.x &&
                        player.y < projectile->box.topLeft.y &&
                        player.y + player.height > projectile->box.topLeft.y) {
                            projectileHitPlayer = true;
                            break;
                        }
                currentPro = getNext(currentPro);
            }
        }

        // TODO: Check end of the game  
        //      Game is over if enemy hits player or projectile hits player
        if (playerHitbyEnemy == true || projectileHitPlayer == true) {
            uLCD.cls();
            while(1) {
                uLCD.locate(3, 3);
                uLCD.printf("you lost");
            }
        }

        // Compute update time
        t.reset();

        // draw player again to increase smoothness
        player_draw(0x0000FF);
        

       // Uncomment This --------------> 

        // uLCD.filled_rectangle(54, 54, 64, 64, 0xFF0000);
    }
    printf("out of main loop\n");

    destroyList(get_enemy_list());
    // destroy more lists as needed
    
    return 0;
    // ===User implementations end===
}

// ===User implementations start===

// Write your helper methods here to deal with game loop objectives
// Feel free to do whatever you need to implement the game






// /* Wanna hear some annoying sounds?*/
// void playNotes(void)
// {
//     int i;
// // generate a 500Hz tone using PWM hardware output
//     speaker.period(1.0/500.0); // 500hz period
//     speaker =0.5; //50% duty cycle - max volume
//     wait(3);
//     speaker=0.0; // turn off audio
//     wait(2);
// // generate a short 150Hz tone using PWM hardware output
// // something like this can be used for a button click effect for feedback
//     for (i=0; i<10; i++) {
//         speaker.period(1.0/150.0); // 500hz period
//         speaker =0.25; //25% duty cycle - mid range volume
//         wait(.02);
//         speaker=0.0; // turn off audio
//         wait(0.5);
//     }

// // sweep up in frequency by changing the PWM period
//     for (i=0; i<8000; i=i+100) {
//         speaker.period(1.0/float(i));
//         speaker=0.25;
//         wait(.1);
//     }
//     wait(2);

// // two tone police siren effect -  two periods or two frequencies
// // increase volume - by changing the PWM duty cycle
//     for (i=0; i<26; i=i+2) {
//         speaker.period(1.0/969.0);
//         speaker = float(i)/50.0;
//         wait(.5);
//         speaker.period(1.0/800.0);
//         wait(.5);
//     }
//     // decrease volume
//     for (i=25; i>=0; i=i-2) {
//         speaker.period(1.0/969.0);
//         speaker = float(i)/50.0;
//         wait(.5);
//         speaker.period(1.0/800.0);
//         wait(.5);
//     }
//     speaker =0.0;
//     wait(2);
// }

void set_random_seed(Timer t) {
    GameInputs inputs; 
    t.start();
    uLCD.printf("Push the nav switch to start.\n");
    while(1){
      inputs = read_inputs();
      if (inputs.up || inputs.down || inputs.fire || inputs.right || inputs.left) break;
      }
    uLCD.cls();
    t.stop();
    int seed = timer_read_ms(t);    
    srand(seed);
}


// ===User implementations end===
