/*
    Standard VGA functions
    Part of Knut's GFX-Lib for Watcom

    1/4-97
*/

void mode_13h();
void text_mode();
void wait_vbl();
void draw_box(long x, long y, long xlen, long ylen, long colour);
void do_draw_box(struct BoxParams *boxParams);
void bput_block(long x, long y, long xlen, long ylen, byte *imgpnt, byte *scr);
void do_bput_block(struct BPutBlockParams *putBlockParams, byte *imgpnt, byte *scr);
void put_block(long x, long y, long xlen, long ylen, byte *imgpnt);
void do_put_block(struct PutBlockParams *putBlockParams, byte *imgpnt);
void cput_block(long x, long y, long xlen, long ylen, byte *imgpnt);
void do_cput_block(struct CPutBlockParams *putBlockParams, byte *imgpnt);

extern "C" {
void bdraw_box(long X, long Y, long xlen, long ylen, long colour, byte *scr); // *
void put_sprite(long,long,long,long,byte *); // *
void bput_sprite(long,long,long,long,byte *, byte *); // *
void block_copy(int srcx, int srcy, int srcxlen, int srcylen, byte *buffer, int destx, int desty); // *
void block_copy_b(int srcx, int srcy, int srcxlen, int srcylen, byte *buffer, int destx, int desty, byte *destimg); // *

void plot(long X, long Y, long colour);
void h_line(int x, int y, int x2, byte colour);
void functest(long,long,long,long,long);
void clear_screen();
void clear_screen_c(long colour);
void draw_light(long X, long Y, long xlen, long ylen, byte *lightptr, byte *imgptr);
void draw_colbar(long,long);
void screen_copy(byte *dest, byte *src);
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

#pragma aux mode_13h = \
    "mov    eax,0013h"       \
    "int    10h"             \
    modify [EAX];

#pragma aux text_mode = \
    "mov    eax,0003h"   \
    "int    10h"         \
    modify [EAX];

#pragma aux wait_vbl = \
    "mov  dx,03DAh" \
"loop1:"\
    "in   al,dx"\
    "test al,8"\
    "jnz  loop1"\
"loop2:"\
    "in   al,dx"\
    "test al,8"\
    "jz   loop2"\
    modify [DX AL];


struct BoxParams {
    long x;
    long y;
    long xlen;
    long ylen;
    long colour;
};

void draw_box(long x, long y, long xlen, long ylen, long colour)
{
    BoxParams params;
    params.x = x;
    params.y = y;
    params.xlen = xlen;
    params.ylen = ylen;
    params.colour = colour;
    do_draw_box(&params);
}

#pragma aux do_draw_box = \
    "mov    eax, [edx]"         /* x */ \
    "mov    ebx, [edx + 4]"     /* y */ \
    "mov    edi, 0A0000h" \
    "mov    ecx, eax"           /* Calculate pixel-position.. */ \
    "shl    eax, 8" \
    "shl    ecx, 6" \
    "add    eax, ecx" \
    "add    eax, ebx" \
    "add    edi, eax" \
    "mov    ebx, 320"           /* Kalkuler mengde som må legges til DI etter hver linje */ \
    "sub    ebx, [edx + 8]"     /* 320 - xlen */ \
    "mov    ah, [edx + 15]"     /* ylen - needs only a byte since y will be max 200 */ \
    "mov    al, [edx + 19]"     /* colour */ \
"next_line:" \
    "mov    ecx, [edx + 8]"     /* xlen */ \
"next:" \
    "mov    [edi], al"          /* Legg en byte til EDI */ \
    "dec    ecx" \
    "inc    edi" \
    "or     ecx,ecx" \
    "jnz    next" \
    "dec    ah" \
    "add    edi,ebx"            /* Legg til slik at neste linje begynner */ \
    "or     ah,ah"              /* Er vertikal teller 0 ennå? */ \
    "jnz    next_line" \
parm [edx] \
modify [eax ebx ecx edx edi];

struct BPutBlockParams {
    long x;
    long y;
    long xlen;
    long ylen;
};

void bput_block(long x, long y, long xlen, long ylen, byte *imgpnt, byte *scr)
{
    BPutBlockParams params;
    params.x = x;
    params.y = y;
    params.xlen = xlen;
    params.ylen = ylen;
    do_bput_block(&params, imgpnt, scr);
}

#pragma aux do_bput_block = \
    "mov     eax, [ebx + 4]"     /* y */ \
    "mov     ecx, eax" \
    "shl     eax, 8" \
    "shl     ecx, 6" \
    "add     eax, ecx"           /* Legg sammen begge Y-ene */ \
    "add     eax, [ebx]"           /* Legg til X */ \
    "add     edi, eax"           /* Ferdig koordinat ligger nå i EDI */ \
    "mov     edx, 320"           /* Kalkuler mengde som må legges til EDI etter hver linje */ \
    "sub     edx, [ebx + 8]"     /* 320 - xlen */ \
    "mov     eax, [ebx + 12]"    /* Legg vertikal teller i EAX */ \
"next_line:" \
    "mov     ecx, [ebx + 8]"           /* xlen */ \
    "rep     movsb"             /* Tegn opp en linje.. */ \
    "dec     eax"               /* En linje mindre.. */ \
    "add     edi, edx"           /* Legg til slik at neste linje begynner under.. */ \
    "or      eax, eax"           /* Vertikal teller 0 ennå? */ \
    "jnz     next_line"        /* Nei - gå til neste linje */ \
parm [ebx] [esi] [edi] \
modify [eax ebx ecx edx edi];

struct PutBlockParams {
    long x;
    long y;
    long xlen;
    long ylen;
};

void put_block(long x, long y, long xlen, long ylen, byte *imgpnt)
{
    PutBlockParams params;
    params.x = x;
    params.y = y;
    params.xlen = xlen;
    params.ylen = ylen;
    do_put_block(&params, imgpnt);
}

#pragma aux do_put_block = \
    "mov     eax, [ebx + 4]" \
    "mov     edi, 0A0000h"        /* Video-mem... */ \
    "mov     ecx, eax" \
    "shl     eax, 8" \
    "shl     ecx, 6" \
    "add     eax, ecx"             /* Legg sammen begge Y-ene */ \
    "add     eax, [ebx]"             /* Legg til X */ \
    "add     edi, eax"             /* Ferdig koordinat ligger nå i EDI */ \
    "mov     edx, 320"             /* Kalkuler mengde som må legges til EDI på */ \
    "sub     edx, [ebx + 8]"             /* 320 - xlen */ \
    "mov     eax, [edx + 12]"          /* Legg vertikal teller i EAX */ \
"next_line:" \
    "mov     ecx, [ebx + 8]"             /* Legg linje-teller i ECX */ \
    "rep     movsb"               /* Tegn opp en linje.. */ \
    "dec     eax"                 /* En linje mindre.. */ \
    "add     edi, edx"             /* Legg til slik at neste linje begynner under.. */ \
    "or      eax, eax"             /* Vertikal teller 0 ennå? */ \
    "jnz     next_line"           /* Nei - gå til neste linje */ \
parm [ebx] [esi] \
modify [eax ebx ecx edx edi];

long _clipart = 0;
long _x = 0;
long _y = 0;
long _xlen = 0;
long _ylen = 0;

void cput_block(long x, long y, long xlen, long ylen, byte *imgpnt)
{
    PutBlockParams params;
    params.x = x;
    params.y = y;
    params.xlen = xlen;
    params.ylen = ylen;
    do_cput_block(&params, imgpnt);
}

#pragma aux do_cput_block = \
    "mov    _x, [ebx]" \
    "mov    _y, [ebx + 4]" \
    "mov    _xlen, [ebx + 8]" \
    "mov    _ylen, [ebx + 12]" \
    "mov    eax,_y" \
    "mov    ebx,_x" \
    "cmp    ebx, 319" /* Are coords outside screen? */ \
    "ja     cend" \
    "cmp    eax, 199" \
    "ja     cend" \
    "add    ebx, _xlen" /* No - should the whole block be drawn? */ \
    "add    eax, _ylen" \
    "cmp    ebx, 319" \
    "jle    normal_put" \
    "cmp    eax, 199" \
    "jle    normal_put" \
    "sub    ebx, 319" \
    "sub    eax, 199" \
    "mov    _clipart, 0" \
    "cmp    ebx, 0" \
    "ja     new_xlen" \
    "cmp    eax, 0" \
    "ja     new_ylen" \
    "jmp    normal_put" \
"new_xlen:" \
    "sub    _xlen, ebx" \
    "mov    clipart, ebx" \
    "jmp    normal_put" \
"new_ylen:" \
    "sub    _ylen, eax" \
"normal_put:" \
    "mov    eax,_y" \
    "mov    _y, ebx" \
    "mov    ebx,_x" \
    "mov    edi,0A0000h"          /* Video-mem... */ \
    "mov    ecx,eax" \
    "shl    eax,8" \
    "shl    ecx,6" \
    "add    eax,ecx"             /* Legg sammen begge Y-ene */ \
    "add    eax,ebx"             /* Legg til X */ \
    "add    edi,eax"             /* Ferdig koordinat ligger nå i EDI */ \
    "mov    edx,320"             /* Kalkuler mengde som må legges til EDI på */ \
    "mov    ebx,_xlen"          /* slutten av hver linje. */ \
    "sub    edx,ebx"          /* Mengde ligger i EDX */ \
    "mov    eax,_ylen"          /* Legg vertikal teller i EAX */ \
"cnext_line:" \
    "mov    ecx,ebx"          /* Legg linje-teller i ECX */ \
    "rep    movsb"               /* Tegn opp en linje.. */ \
    "dec    eax"                 /* En linje mindre.. */ \
    "add    edi,edx"             /* Legg til slik at neste linje begynner under.. */ \
    "add    esi,_clipart" \
    "or     eax,eax"             /* Vertikal teller 0 ennå? */ \
    "jnz    cnext_line"          /* Nei - gå til neste linje */ \
"cend:" \
parm [ebx] [esi] \
modify [eax ebx ecx edx esi edi];
