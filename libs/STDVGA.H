/*
    Standard VGA functions
    Part of Knut's GFX-Lib for Watcom

    1/4-97
*/

void mode_13h();
void text_mode();
void wait_vbl();
void plot(long x, long y, long colour);
void do_plot();
void draw_box(long x, long y, long xlen, long ylen, long colour);
void do_draw_box(struct BoxParams *boxParams);
void bput_block(long x, long y, long xlen, long ylen, byte *imgpnt, byte *scr);
void do_bput_block(struct PutBlockParams *putBlockParams, byte *imgpnt, byte *scr);
void put_block(long x, long y, long xlen, long ylen, byte *imgpnt);
void do_put_block(byte *imgpnt);
void cput_block(long x, long y, long xlen, long ylen, byte *imgpnt);
void do_cput_block(struct PutBlockParams *putBlockParams, byte *imgpnt);
void bdraw_box(long x, long y, long xlen, long ylen, long colour, byte *scr);
void do_bdraw_box(struct BoxParams *boxParams, byte *scr);
void put_sprite(long x, long y, long xlen, long ylen, byte *imgpnt);
void do_put_sprite(struct PutBlockParams *putBlockParams, byte *imgpnt);
void bput_sprite(long x, long y, long xlen, long ylen, byte *imgpnt, byte *buffer);
void do_bput_sprite(struct PutBlockParams *putBlockParams, byte *imgpnt, byte *buffer);
void block_copy(int srcx, int srcy, int srcxlen, int srcylen, byte *buffer, int destx, int desty);
void do_block_copy(struct BlockCopyParams *params, byte *buffer);
void block_copy_b(int srcx, int srcy, int srcxlen, int srcylen, byte *buffer, int destx, int desty, byte *destimg);
void do_block_copy_b(struct BlockCopyParams *params, byte *buffer);
short detectvga();

long _clipart = 0;
long _x = 0;
long _y = 0;
long _xlen = 0;
long _ylen = 0;
long _colour = 0;
long _srcx = 0;
long _srcy = 0;
long _srcxlen = 0;
long _srcylen = 0;
long _dstx = 0;
long _dsty = 0;

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

struct PlotParams {
    long x;
    long y;
    long colour;
};

void plot(long x, long y, long colour)
{
    _x = x;
    _y = y;
    _colour = colour;
    do_plot();
}

#pragma aux do_plot = \
    "mov     eax,_y" \
    "mov ebx,_x" \
    "mov edx,_colour" \
    "mov     edi, 0a0000h" \
    "shl     eax, 6" \
    "add     edi, eax" \
    "shl     eax, 1" \
    "shl     eax, 1" \
    "add     edi, eax" \
    "add     edi, ebx" \
    "mov     BYTE PTR [edi], dl" \
modify [eax ebx edx edi];

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

struct PutBlockParams {
    long x;
    long y;
    long xlen;
    long ylen;
};

void bput_block(long x, long y, long xlen, long ylen, byte *imgpnt, byte *scr)
{
    PutBlockParams params;
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

void put_block(long x, long y, long xlen, long ylen, byte *imgpnt)
{
    _x = x;
    _y = y;
    _xlen = xlen;
    _ylen = ylen;
    do_put_block(imgpnt);
}

#pragma aux do_put_block = \
    "mov     eax,_y" \
    "mov     ebx,_x" \
    "mov     edi,0A0000h"          /* Video-mem... */ \
    "mov     ecx,eax" \
    "shl     eax,8" \
    "shl     ecx,6" \
    "add     eax,ecx"             /* Legg sammen begge Y-ene */ \
    "add     eax,ebx"             /* Legg til X */ \
    "add     edi,eax"             /* Ferdig koordinat ligger nå i EDI */ \
    "mov     edx,320"             /* Kalkuler mengde som må legges til EDI på */ \
    "mov     ebx,_xlen"          /* slutten av hver linje. */ \
    "sub     edx,ebx"             /* Mengde ligger i EDX */ \
    "mov     eax,_ylen"          /* Legg vertikal teller i EAX */ \
    "next_line:" \
    "mov     ecx,ebx"             /* Legg linje-teller i ECX */ \
    "rep     movsb"               /* Tegn opp en linje.. */ \
    "dec     eax"                 /* En linje mindre.. */ \
    "add     edi,edx"             /* Legg til slik at neste linje begynner under.. */ \
    "or      eax,eax"             /* Vertikal teller 0 ennå? */ \
    "jnz     next_line"           /* Nei - gå til neste linje */ \
parm [esi] \
modify [eax ebx ecx edx edi esi];

void cput_block(long x, long y, long xlen, long ylen, byte *imgpnt)
{
    PutBlockParams params;
    params.x = x;
    params.y = y;
    params.xlen = xlen;
    params.ylen = ylen;
    _x = x;
    _y = y;
    _xlen = xlen;
    _ylen = ylen;
    do_cput_block(&params, imgpnt);
}

#pragma aux do_cput_block = \
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
    "mov    _clipart, ebx" \
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

void bdraw_box(long x, long y, long xlen, long ylen, long colour, byte *scr)
{
    BoxParams params;
    params.x = x;
    params.y = y;
    params.xlen = xlen;
    params.ylen = ylen;
    params.colour = colour;
    _x = x;
    _y = y;
    _xlen = xlen;
    _ylen = ylen;
    _colour = colour;
    do_bdraw_box(&params, scr);
}

#pragma aux do_bdraw_box = \
    "mov    eax,_y" \
    "mov    ebx,_x" \
    "mov    ecx,eax"       /* Calculate pixel-position.. */ \
    "shl    eax,8" \
    "shl    ecx,6" \
    "add    eax,ecx" \
    "add    eax,ebx" \
    "add    edi,eax" \
    "mov    ebx,320"            /* Kalkuler mengde som må legges til DI på */ \
    "mov    edx,_xlen" \
    "sub    ebx,edx" \
    "mov    eax,_ylen"         /* Legg vertikal teller i AX */ \
    "mov    edx,_colour" \
"next_line:" \
    "mov    ecx,_xlen"        /* Horisontal teller i CX */ \
"next:" \
    "mov    [edi],dl"    /* Legg en byte til EDI */ \
    "dec    ecx" \
    "inc    edi" \
    "or     ecx,ecx" \
    "jnz    next" \
    "dec    eax" \
    "add    edi,ebx"     /* Legg til slik at neste linje begynner */ \
    "or     eax,eax"     /* Er vertikal teller 0 ennå? */ \
    "jnz    next_line" \
parm [ebx] [edi] \
modify [eax ebx ecx edx esi edi];

void put_sprite(long x, long y, long xlen, long ylen, byte *imgpnt)
{
    PutBlockParams params;
    params.x = x;
    params.y = y;
    params.xlen = xlen;
    params.ylen = ylen;
    _x = x;
    _y = y;
    _xlen = xlen;
    _ylen = ylen;
    do_put_sprite(&params, imgpnt);
}

#pragma aux do_put_sprite = \
    "mov     eax,_y" \
    "mov     ebx,_x" \
    "mov     edi,0A0000h"             /* Video-mem... */ \
    "mov     ecx,eax" \
    "shl     eax,8" \
    "shl     ecx,6" \
    "add     eax,ecx"                 /* Legg sammen begge Y-ene */ \
    "add     eax,ebx"                 /* Legg til X */ \
    "add     edi,eax"                 /* Ferdig koordinat ligger nå i EDI */ \
    "mov     edx,320"                 /* Kalkuler mengde som må legges til EDI på */ \
    "mov     ebx,_xlen"              /* slutten av hver linje. */ \
    "sub     edx,ebx"                 /* Mengde ligger i EDX */ \
    "mov     eax,_ylen"              /* Legg vertikal teller i EAX */ \
"bloop1:" \
    "mov     ecx,_xlen"              /* Sett counter til horisontal lengde. */ \
"bloop2:" \
    "mov  bl,[esi]"                   /* Få en byte fra SI */ \
    "or   bl,bl"                    /* Er den 0? */ \
    "je   skip_pixel"                 /* OK - Skip den */ \
    "mov  [edi],bl"                   /* Nei - Tegn den */ \
    "inc  edi" \
    "inc  esi" \
    "dec  ecx"                        /* En pixel tegnet... */ \
    "or   ecx,ecx"                    /* Er denne linjen ferdig ennå? */ \
    "jne  bloop2"                      /* Nei - kjør gjennom igjen... */ \
    "add  edi,edx"                    /* Legg til slik at neste linje starter under... */ \
    "dec  eax"                        /* Ja - en linje ferdig... */ \
    "or   eax,eax"                    /* Er Vertikal teller null enda? */ \
    "jne  bloop1"                      /* Nei - kjør loop igjen */ \
    "jmp  bend"                        /* Ja - avslutt */ \
"skip_pixel:" \
    "inc  edi"                        /* Hopp over dette koordinatet */ \
    "inc  esi" \
    "dec  ecx"                        /* En pixel tegnet.. */ \
    "or   ecx,ecx"                    /* Er horisontal teller 0? */ \
    "jne  bloop2"                      /* Hopp tilbake hvis linje ikke er ferdig tegnet */ \
    "add  edi,edx"                    /* Legg til slik at neste linje starter under.. */ \
    "or   eax,eax"                    /* Er vertikal teller 0? */ \
    "dec  eax"                        /* En linje mindre.. */ \
    "jnz  bloop1"                      /* Hvis den ikke er 0, initialiser teller på nytt. */ \
"bend:" \
parm [ebx] [esi] \
modify [eax ebx ecx edx esi edi];

void bput_sprite(long x, long y, long xlen, long ylen, byte *imgpnt, byte *buffer)
{
    PutBlockParams params;
    params.x = x;
    params.y = y;
    params.xlen = xlen;
    params.ylen = ylen;
    _x = x;
    _y = y;
    _xlen = xlen;
    _ylen = ylen;
    do_bput_sprite(&params, imgpnt, buffer);
}

#pragma aux do_bput_sprite = \
    "mov     eax,_y" \
    "mov     ebx,_x" \
    "mov     ecx,eax" \
    "shl     eax,8" \
    "shl     ecx,6" \
    "add     eax,ecx"                 /* Legg sammen begge Y-ene */ \
    "add     eax,ebx"                 /* Legg til X */ \
    "add     edi,eax"                 /* Ferdig koordinat ligger nå i EDI */ \
    "mov     edx,320"                 /* Kalkuler mengde som må legges til EDI på */ \
    "mov     ebx,_xlen"              /* slutten av hver linje. */ \
    "sub     edx,ebx"                 /* Mengde ligger i EDX */ \
    "mov     eax,_ylen"              /* Legg vertikal teller i EAX */ \
"bbloop1:" \
    "mov     ecx,_xlen"              /* Sett counter til horisontal lengde.*/ \
"bbloop2:" \
    "mov  bl,[esi]"                   /* Få en byte fra SI*/ \
    "or   bl,bl"                    /* Er den 0?*/ \
    "je   bskip_pixel"                 /* OK - Skip den*/ \
    "mov  [edi],bl"                   /* Nei - Tegn den*/ \
    "inc  edi" \
    "inc  esi" \
    "dec  ecx"                        /* En pixel tegnet...*/ \
    "or   ecx,ecx"                    /* Er denne linjen ferdig ennå?*/ \
    "jne  bbloop2"                      /* Nei - kjør gjennom igjen...*/ \
    "add  edi,edx"                    /* Legg til slik at neste linje starter under...*/ \
    "dec  eax"                        /* Ja - en linje ferdig...*/ \
    "or   eax,eax"                    /* Er Vertikal teller null enda?*/ \
    "jne  bbloop1"                      /* Nei - kjør loop igjen*/ \
    "jmp  bbend"                        /* Ja - avslutt*/ \
"bskip_pixel:" \
    "inc  edi"                        /* Hopp over dette koordinatet*/ \
    "inc  esi" \
    "dec  ecx"                        /* En pixel "tegnet"..*/ \
    "or   ecx,ecx"                    /* Er horisontal teller 0?*/ \
    "jne  bbloop2"                      /* Hopp tilbake hvis linje ikke er ferdig tegnet*/ \
    "add  edi,edx"                    /* Legg til slik at neste linje starter under..*/ \
    "or   eax,eax"                    /* Er vertikal teller 0?*/ \
    "dec  eax"                        /* En linje mindre..*/ \
    "jnz  bbloop1"                      /* Hvis den ikke er 0, initialiser teller på nytt*/ \
"bbend:" \
parm [ebx] [esi] [edi] \
modify [eax ebx ecx edx esi edi];

struct BlockCopyParams {
    long srcx;
    long srcy;
    long srcxlen;
    long srcylen;
    long dstx;
    long dsty;
};

void block_copy(int srcx, int srcy, int srcxlen, int srcylen, byte *buffer, int destx, int desty)
{
    BlockCopyParams params;
    params.srcx = srcx;
    params.srcy = srcy;
    params.srcxlen = srcxlen;
    params.srcylen = srcylen;
    params.dstx = destx;
    params.dsty = desty;
    _srcx = srcx;
    _srcy = srcy;
    _srcxlen = srcxlen;
    _srcylen = srcylen;
    _dstx = destx;
    _dsty = desty;
    do_block_copy(&params, buffer);
}

#pragma aux do_block_copy = \
    "mov    ecx,_srcy" \
    "shl    ecx,6" \
    "mov    eax,_srcy" \
    "shl    eax,8" \
    "add    eax,ecx" \
    "add    eax,_srcx" \
    "add    esi,eax" \
    "mov    eax,_dsty" \
    "mov    edi,0A0000h"         /* Video-minne... */ \
    "mov    ecx,eax" \
    "shl    eax,8" \
    "shl    ecx,6" \
    "add    eax,ecx"             /* Legg sammen begge Y-ene */ \
    "add    eax,_dstx"         /* Legg til X */ \
    "add    edi,eax"             /* Ferdig koordinat ligger nå i EDI */ \
    "mov    edx,320"             /* Kalkuler mengde som må legges til hver av skjermene ved linjeskift */ \
    "mov    ebx,_srcxlen"       /* slutten av hver linje. */ \
    "sub    edx,ebx"             /* Mengde ligger i EDX */ \
    "mov    eax,_srcylen"       /* Legg vertikal teller i EAX */ \
"next_line:" \
    "mov    ecx,ebx"             /* Legg linje-teller i ECX */ \
    "rep    movsb"               /* Tegn opp en linje.. */ \
    "add    esi, edx"            /* Legg til 'linjeskift' på buffer */ \
    "dec    eax"                 /* En linje mindre.. */ \
    "add    edi,edx"             /* Legg til 'linjeskift' på skjerm */ \
    "or     eax,eax"             /* Vertikal teller 0 ennå? */ \
    "jnz    next_line"          /* Nei - gå til neste linje */ \
parm [ebx] [esi] \
modify [eax ebx ecx edx esi edi];

void block_copy_b(int srcx, int srcy, int srcxlen, int srcylen, byte *buffer, int destx, int desty, byte *destimg)
{
    BlockCopyParams params;
    params.srcx = srcx;
    params.srcy = srcy;
    params.srcxlen = srcxlen;
    params.srcylen = srcylen;
    params.dstx = destx;
    params.dsty = desty;
    _srcx = srcx;
    _srcy = srcy;
    _srcxlen = srcxlen;
    _srcylen = srcylen;
    _dstx = destx;
    _dsty = desty;
    do_block_copy_b(&params, buffer);
}

#pragma aux do_block_copy_b = \
    "mov     ecx,_srcy" \
    "shl     ecx,6" \
    "mov     eax,_srcy" \
    "shl     eax,8" \
    "add     eax,ecx" \
    "add     eax,_srcx" \
    "add     esi,eax" \
    "mov     eax,_dsty"           /* Kalkuler koordinater for skjerm-minne */ \
    "mov     ecx,eax" \
    "shl     eax,8" \
    "shl     ecx,6" \
    "add     eax,ecx"             /* Legg sammen begge Y-ene */ \
    "add     eax,_dstx"         /* Legg til X */ \
    "add     edi,eax"             /* Ferdig koordinat ligger nå i EDI */ \
    "mov     edx,320"             /* Kalkuler mengde som må legges til hver av skjermene ved linjeskift */ \
    "mov     ebx,_srcxlen"       /* slutten av hver linje. */ \
    "sub     edx,ebx"             /* Mengde ligger i EDX */ \
    "mov     eax,_srcylen"       /* Legg vertikal teller i EAX */ \
"next_line:" \
    "mov     ecx,ebx"             /* Legg linje-teller i ECX */ \
    "rep     movsb"               /* Tegn opp en linje.. */ \
    "add     esi, edx"            /* Legg til 'linjeskift' på buffer */ \
    "dec     eax"                 /* En linje mindre.. */ \
    "add     edi,edx"             /* Legg til 'linjeskift' på skjerm */ \
    "or      eax,eax"             /* Vertikal teller 0 ennå? */ \
    "jnz     next_line"          /* Nei - gå til neste linje */ \
parm [ebx] [esi] [edi] \
modify [eax ebx ecx edx esi edi];
