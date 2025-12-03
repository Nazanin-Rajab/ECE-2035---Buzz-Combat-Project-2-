#include "graphics.h"
#include "globals.h"
#include "player_public.h"

#define YELLOW 0xFFFF00
#define GREEN 0x00FF00
#define WHITE 0xFFFFFF
#define BLACK 0x000000
#define BROWN  0xD2691E
#define DIRT   BROWN
#define RED    0xFF0000
#define ORANGE 0xFFA500


void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = YELLOW;
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'D') colors[i] = DIRT;
        // else if (img[i] == '5') colors[i] = LGREY;
        // else if (img[i] == '3') colors[i] = DGREY;
        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}

void draw_player(int u, int v, int key)
{
    uLCD.filled_rectangle(u, v, u+11, v+11, WHITE);
}

void draw_nothing(boundingBox b)
{
    // Erase a bounding box
    uLCD.filled_rectangle(b.topLeft.x, b.topLeft.y, b.bottomRight.x, b.bottomRight.y, BLACK);
}

void draw_red(boundingBox b)
{
    // Draw an apple using a filled_rectangle or get creative and use a sprite!
    // Erase a bounding box
    uLCD.filled_rectangle(b.topLeft.x, b.topLeft.y, b.bottomRight.x, b.bottomRight.y, RED);
}

void draw_green(boundingBox b)
{
    // Draw a banana using a filled_rectangle or get creative and use a sprite!
    uLCD.filled_rectangle(b.topLeft.x, b.topLeft.y, b.bottomRight.x, b.bottomRight.y, GREEN);
}

void draw_orange(boundingBox b)
{
// Draw an orange using a filled_rectangle or get creative and use a sprite!
    uLCD.filled_rectangle(b.topLeft.x, b.topLeft.y, b.bottomRight.x, b.bottomRight.y, ORANGE);
    
}

void draw_blue(boundingBox b)
{
// Draw an orange using a filled_rectangle or get creative and use a sprite!
    uLCD.filled_rectangle(b.topLeft.x, b.topLeft.y, b.bottomRight.x, b.bottomRight.y, BLUE);
}

void draw_sprit(int x, int y, int height) {
    static int new_piskel_data[121] =
    {
        BLACK, BLACK, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, BLACK, BLACK,
        BLACK, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, BLACK,
        YELLOW, YELLOW, YELLOW, BLACK, YELLOW, YELLOW, YELLOW, BLACK, YELLOW, YELLOW, YELLOW,
        YELLOW, YELLOW, YELLOW, BLACK, YELLOW, YELLOW, YELLOW, BLACK, YELLOW, YELLOW, YELLOW,
        YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW,
        YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW,
        YELLOW, BLACK, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, BLACK, YELLOW,
        YELLOW, YELLOW, BLACK, BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  YELLOW, YELLOW, 
        YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW,
        BLACK, BLACK, YELLOW,  YELLOW,  YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, BLACK, BLACK,
        BLACK, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, BLACK
    };
    uLCD.BLIT(x, y, 11, 11, new_piskel_data);
}

// Creates red missles
void draw_bomb(boundingBox b) {
    static int new_piskel_data[121] =
    {
        BLACK, RED, RED, BLACK, 
        RED, RED, RED, RED, 
        RED, RED, RED, RED,  
        RED, RED, RED, RED,  
        RED, RED, RED, RED, 
        BLACK, RED, RED, BLACK
    };
    uLCD.BLIT(b.topLeft.x, b.topLeft.y, b.bottomRight.x - b.topLeft.x, b.bottomRight.y - b.topLeft.y, new_piskel_data);
}

// Creates arrow missiles
void draw_arrow(boundingBox b) { 
    static int new_piskel_data[24] =
    {
        BLACK, GREEN, GREEN, BLACK, GREEN, GREEN,
        GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, 
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, 
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK

    };
    
    uLCD.BLIT(b.topLeft.x, b.topLeft.y, b.bottomRight.x - b.topLeft.x, b.bottomRight.y - b.topLeft.y, new_piskel_data);
}

void draw_blueberry(boundingBox b) {
    static int new_piskel_data[121] =
    {
        BLACK, GREEN , GREEN, BLACK, 
        BLUE, BLUE, BLUE, BLUE, 
        BLUE, BLUE, BLUE, BLUE,   
        BLUE, BLUE, BLUE, BLUE,   
        BLUE, BLUE, BLUE, BLUE, 
        BLACK, BLUE, BLUE, BLACK
    };
    uLCD.BLIT(b.topLeft.x, b.topLeft.y, b.bottomRight.x - b.topLeft.x, b.bottomRight.y - b.topLeft.y, new_piskel_data);
}

