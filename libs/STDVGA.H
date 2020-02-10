/*
    Standard VGA functions
    Part of Knut's GFX-Lib for Watcom

    1/4-97
*/

extern "C" {

void mode_13h();
void text_mode();
void wait_vbl();
void put_block(long x, long y, long xlen, long ylen, byte *imgpnt);
void bput_block(long x, long y, long xlen, long ylen, byte *imgpnt, byte *scr);
void cput_block(long x, long y, long xlen, long ylen, byte *imgpnt);
void plot(long X, long Y, long colour);
void h_line(int x, int y, int x2, byte colour);
void functest(long,long,long,long,long);
void clear_screen();
void clear_screen_c(long colour);
void draw_box(long X, long Y, long xlen, long ylen, long colour);
void bdraw_box(long X, long Y, long xlen, long ylen, long colour, byte *scr);
void draw_light(long X, long Y, long xlen, long ylen, byte *lightptr,
                byte *imgptr);
void put_sprite(long,long,long,long,byte *);
void bput_sprite(long,long,long,long,byte *, byte *);
void draw_colbar(long,long);
void screen_copy(byte *dest, byte *src);
void block_copy(int srcx, int srcy, int srcxlen, int srcylen, byte *buffer,
                int destx, int desty);
void block_copy_b(int srcx, int srcy, int srcxlen, int srcylen, byte *buffer,
                int destx, int desty, byte *destimg);
void anti_alias(int x, int y, int xlen, int ylen, byte *scrptr);
};

short detectvga();

#pragma aux detectvga =                \
    "mov    bx, 0xFFFF"                 \
    "mov    ax, 0x101A"                 \
    "int    0x10"                       \
    modify [AX]                         \
    value [BX];

short detect_vga()
{
    short result = detectvga();
    if (result == 0xFFFF)
        return 0;
    else
        return 1;
}

