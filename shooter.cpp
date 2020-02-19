#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include "libs\w_mlib.h"    // Mouse-library
#include "libs\gfxlib.h"   // Gfx-header
#include "libs\imageio.h"

const short TRUE = 1;
const short FALSE = 0;
const short SHOT_DELAY = 10;
const short SHIP_WIDTH = 16;
const short SHIP_HEIGHT = 32;
const short SHIP_ANIMATION_TRESHOLD = 2;

short smallestNumber(short a, short b);
short largestNumber(short a, short b);
void loadAssets();
void initBullets();
void initVgaGraphicsMode();
void initMouse();
short checkShotFired(short shotDelay, short curMouseX, short curMouseY);
short updateBullets(short shotDelay, short shotVelocity);
void clearShip(short curMouseX, short curMouseY, signed short deltaX, signed short deltaY);
// To add:
void spawnEnemy();
void clippingPutBlock();

struct Bullet {
    short x;
    short y;
};

byte ship[5][512];
byte bullet[12];
signed char palette[768];
Bullet bullets[20];

int main(int argc, char *argv[])
{
    initBullets();
    loadAssets();
    initVgaGraphicsMode();
    initMouse();

    short curMouseX = MOUSE_X, curMouseY = MOUSE_Y;
    signed short deltaX = 0, deltaY = 0;
    short targetState = 2;
    short curState = 2;
    short freezeFrames = 0;

    short shotDelay = 0;
    short shotVelocity = 4;

    short xOffset = 0;
    short yOffset = 0;
    short quit = FALSE;

    while (quit == FALSE) {
        short key;

        shotDelay = checkShotFired(shotDelay, curMouseX, curMouseY);
        shotDelay = updateBullets(shotDelay, shotVelocity);

        put_block(curMouseX, curMouseY, SHIP_WIDTH, SHIP_HEIGHT, ship[curState]);

        wait_vbl();
        get_mouse_status();

        deltaX = MOUSE_X - curMouseX;
        deltaY = MOUSE_Y - curMouseY;

        clearShip(curMouseX, curMouseY, deltaX, deltaY);

        curMouseX = MOUSE_X;
        curMouseY = MOUSE_Y;

        if (freezeFrames == 0) {
            if (abs(deltaX) >= SHIP_ANIMATION_TRESHOLD) {
                targetState = deltaX > 0 ? 0 : 4;
            } else {
                targetState = 2;
            }

            if (targetState != curState) {
                if (targetState < curState) curState--;
                if (targetState > curState) curState++;
                freezeFrames = 10;
            }
        }

        if (freezeFrames > 0) freezeFrames--;

        if (kbhit()) {
            key = getch();

            if (key == 27)      // ESC  - Quit
                quit = TRUE;
        }
    }

    text_mode();
    reset_mouse();
}

short smallestNumber(short a, short b) {
    return a < b ? a : b;
}

short largestNumber(short a, short b) {
    return a > b ? a : b;
}

void loadAssets()
{
    pcxRead("bullets.pcx", bullet, palette, 4, 3);
    pcxRead("ship_ll.pcx", ship[0], palette, 16, 32);
    pcxRead("ship_l.pcx", ship[1], palette, 16, 32);
    pcxRead("ship.pcx", ship[2], palette, 16, 32);
    pcxRead("ship_r.pcx", ship[3], palette, 16, 32);
    pcxRead("ship_rr.pcx", ship[4], palette, 16, 32);
}

void initBullets()
{
    for (short i = 0; i < 20; i++) {
        bullets[i].x = 0;
        bullets[i].y = 0;
    }
}

void initVgaGraphicsMode()
{
    mode_13h();
    set_pal(palette);
}

void initMouse()
{
    set_mouse_sens(20,16);
    xlimit_mouse(8,300);
    ylimit_mouse(167,167);
    get_mouse_status();
}

short checkShotFired(short shotDelay, short curMouseX, short curMouseY)
{
    if (shotDelay == 0 && BUTTON_STATE == 1)
    {
        short freeBulletSlot = -1;
        for (short i = 0; i < 20; i++) {
            if (bullets[i].y <= 0) {
                freeBulletSlot = i;
                break;
            }
        }
        if (freeBulletSlot >= 0) {
            bullets[freeBulletSlot].x = curMouseX + 6;
            bullets[freeBulletSlot].y = curMouseY - 4;
            shotDelay = SHOT_DELAY;
        }
    }
    return shotDelay;
}

short updateBullets(short shotDelay, short shotVelocity)
{
    if (shotDelay > 0) shotDelay--;
    for (short i = 0; i < 20; i++) {
        if (bullets[i].y > 0) {
            draw_box(bullets[i].x, bullets[i].y, 4, 3, 0);
            bullets[i].y -= shotVelocity;
        }
        if (bullets[i].y > 0) {
            put_block(bullets[i].x, bullets[i].y, 4, 3, bullet);
        }
    }
    return shotDelay;
}

void clearShip(short curMouseX, short curMouseY, signed short deltaX, signed short deltaY)
{
    short xOffset = 0,
        yOffset = 0;

    if (deltaX < 0) {
        xOffset = SHIP_WIDTH;
    }
    if (deltaY < 0) {
        yOffset = SHIP_HEIGHT;
    }

    draw_box(smallestNumber(curMouseX, MOUSE_X) + xOffset, curMouseY, abs(deltaX) + 1, SHIP_HEIGHT, 0);
    draw_box(curMouseX, smallestNumber(curMouseY, MOUSE_Y) + yOffset, SHIP_WIDTH, abs(deltaY) + 1, 0);
}

void spawnEnemy()
{

}

void clippingPutBlock()
{

}