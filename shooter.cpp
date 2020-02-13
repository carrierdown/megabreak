#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include "libs\w_mlib.h"    // Mouse-library
#include "libs\gfxlib.h"   // Gfx-header
#include "libs\imageio.h"

const short TRUE = 1;
const short FALSE = 0;
short quit = FALSE;

short smallestNumber(short a, short b);
short largestNunber(short a, short b);

int main(int argc, char *argv[])
{
    static byte ship[5][512];
    static signed char palette[768];
    short key = 0;

    pcxRead("ship_ll.pcx", ship[0], palette, 16, 32);
    pcxRead("ship_l.pcx", ship[1], palette, 16, 32);
    pcxRead("ship.pcx", ship[2], palette, 16, 32);
    pcxRead("ship_r.pcx", ship[3], palette, 16, 32);
    pcxRead("ship_rr.pcx", ship[4], palette, 16, 32);

    mode_13h();
    set_pal(palette);
    xlimit_mouse(8,300);
    ylimit_mouse(167,167);
    get_mouse_status();

    short curMouseX = MOUSE_X, curMouseY = MOUSE_Y;
    signed short deltaX = 0, deltaY = 0;
    short moveWithAnimTreshold = 2;
    short targetState = 2;
    short curState = 2;
    short freezeFrames = 0;

    const short SHIP_WIDTH = 16;
    const short SHIP_HEIGHT = 32;

    short xOffset = 0;
    short yOffset = 0;

    while (quit == FALSE) {
        put_block(curMouseX, curMouseY, SHIP_WIDTH, SHIP_HEIGHT, ship[curState]);
        wait_vbl();
        get_mouse_status();

        deltaX = MOUSE_X - curMouseX;
        deltaY = MOUSE_Y - curMouseY;

        if (deltaX < 0) {
            xOffset = SHIP_WIDTH;
        } else {
            xOffset = 0;
        }
        if (deltaY < 0) {
            yOffset = SHIP_HEIGHT;
        } else {
            yOffset = 0;
        }

        draw_box(smallestNumber(curMouseX, MOUSE_X) + xOffset, curMouseY, abs(deltaX) + 1, SHIP_HEIGHT, 0);
        draw_box(curMouseX, smallestNumber(curMouseY, MOUSE_Y) + yOffset, SHIP_WIDTH, abs(deltaY) + 1, 0);

        curMouseX = MOUSE_X;
        curMouseY = MOUSE_Y;

        if (freezeFrames == 0) {
            if (abs(deltaX) >= moveWithAnimTreshold) {
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
//    reset_mouse();
}

short smallestNumber(short a, short b) {
    return a < b ? a : b;
}

short largestNunber(short a, short b) {
    return a > b ? a : b;
}