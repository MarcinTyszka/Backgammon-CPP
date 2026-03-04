#include "board.h"
#include "conio2.h"
#include <stdio.h>


// Initializes the board with starting pawn positions
void BoardInitialization(Pawns Board[]) {
    for (int i = 0; i <= MAXFIELDS; ++i) {
        Board[i].WhitePawns = 0;
        Board[i].RedPawns = 0;
    }
    Board[1].WhitePawns = 2;
    Board[6].RedPawns = 5;
    Board[8].RedPawns = 3;
    Board[12].WhitePawns = 5;
    Board[13].RedPawns = 5;
    Board[17].WhitePawns = 3;
    Board[19].WhitePawns = 5;
    Board[24].RedPawns = 2;
}

// Internal helper functions (not declared in the header)
void DrawCourt(Pawns Board[]) {
    gotoxy(57, 12);
    textcolor(COLORONE);
    printf("Pawns in  Red  Court: %d", Board[COURT].RedPawns);
    gotoxy(57, 16);
    textcolor(COLORTWO);
    printf("Pawns in White Court: %d", Board[COURT].WhitePawns);
    textcolor(BASECOLOR);
}

void DrawBar(Pawns Board[]) {
    int Bar = 15;

    if (Board[BAR].WhitePawns > 0) {
        Bar -= Board[BAR].WhitePawns;
    }
    if (Board[BAR].RedPawns > 0) {
        Bar -= Board[BAR].RedPawns;
    }

    int x = 28;
    int y = 7;

    gotoxy(x, y);

    for (int i = 0; i < Board[BAR].WhitePawns; ++i) {
        textcolor(COLORTWO);
        printf("xx");
        gotoxy(x, ++y);
    }

    for (int i = 0; i < Bar; ++i) {
        textcolor(BASECOLOR);
        printf("||");
        gotoxy(x, ++y);
    }

    for (int i = 0; i < Board[BAR].RedPawns; ++i) {
        textcolor(COLORONE);
        printf("xx");
        gotoxy(x, ++y);
    }

    textcolor(BASECOLOR);
}

void WhiteHome(Pawns Board[]) {
    int x = 32;
    int y = 21;

    for (int i = 6; i >= 1; --i) {
        int ToPrint = 0;

        if (Board[i].WhitePawns > 0) {
            ToPrint = Board[i].WhitePawns;
            textcolor(COLORTWO);
        }
        else if (Board[i].RedPawns > 0) {
            ToPrint = Board[i].RedPawns;
            textcolor(COLORONE);
        }

        int limit = (ToPrint < 6) ? ToPrint : 6;

        for (int j = 0; j < limit; ++j) {
            gotoxy(x, y);
            printf("XX\n");
            y--;
        }

        for (int j = 0; j < 6 - ToPrint; ++j) {
            textcolor(BASECOLOR);
            gotoxy(x, y);
            printf("/\\\n");
            y--;
        }

        gotoxy(x, y);
        printf("N%d", ToPrint);
        textcolor(BASECOLOR);
        x += 4;
        y += 6;
    }
}

void Redhome(Pawns Board[]) {
    int x = 32;
    int y = 7;

    for (int i = 19; i <= 24; ++i) {
        int ToPrint = 0;

        if (Board[i].WhitePawns > 0) {
            ToPrint = Board[i].WhitePawns;
            textcolor(COLORTWO);
        }
        else if (Board[i].RedPawns > 0) {
            ToPrint = Board[i].RedPawns;
            textcolor(COLORONE);
        }

        int limit = (ToPrint < 6) ? ToPrint : 6;

        for (int j = 0; j < limit; ++j) {
            gotoxy(x, y);
            printf("XX\n");
            y++;
        }

        for (int j = 0; j < 6 - ToPrint; ++j) {
            textcolor(BASECOLOR);
            gotoxy(x, y);
            printf("\\/\n");
            y++;
        }

        gotoxy(x, y);
        printf("N%d", ToPrint);
        textcolor(BASECOLOR);
        x += 4;
        y -= 6;
    }
}

void LeftTop(Pawns Board[]) {
    int x = 4;
    int y = 7;

    for (int i = 13; i <= 18; ++i) {
        int ToPrint = 0;

        if (Board[i].WhitePawns > 0) {
            ToPrint = Board[i].WhitePawns;
            textcolor(COLORTWO);
        }
        else if (Board[i].RedPawns > 0) {
            ToPrint = Board[i].RedPawns;
            textcolor(COLORONE);
        }

        int limit = (ToPrint < 6) ? ToPrint : 6;

        for (int j = 0; j < limit; ++j) {
            gotoxy(x, y);
            printf("XX\n");
            y++;
        }

        for (int j = 0; j < 6 - ToPrint; ++j) {
            textcolor(BASECOLOR);
            gotoxy(x, y);
            printf("\\/\n");
            y++;
        }

        gotoxy(x, y);
        printf("N%d", ToPrint);
        textcolor(BASECOLOR);
        x += 4;
        y -= 6;
    }
}

void LeftDown(Pawns Board[]) {
    int x = 4;
    int y = 21;

    for (int i = 12; i >= 7; --i) {
        int ToPrint = 0;

        if (Board[i].WhitePawns > 0) {
            ToPrint = Board[i].WhitePawns;
            textcolor(COLORTWO);
        }
        else if (Board[i].RedPawns > 0) {
            ToPrint = Board[i].RedPawns;
            textcolor(COLORONE);
        }

        int limit = (ToPrint < 6) ? ToPrint : 6;

        for (int j = 0; j < limit; ++j) {
            gotoxy(x, y);
            printf("XX\n");
            y--;
        }

        for (int j = 0; j < 6 - ToPrint; ++j) {
            textcolor(BASECOLOR);
            gotoxy(x, y);
            printf("/\\\n");
            y--;
        }

        gotoxy(x, y);
        printf("N%d", ToPrint);
        textcolor(BASECOLOR);
        x += 4;
        y += 6;
    }
}

void DrawBorders() {
    gotoxy(1, 5);
    printf("   13  14  15  16  17  18  XX  19  20  21  22  23  24   ");

    gotoxy(3, 6);
    printf("====================================================");

    gotoxy(3, 22);
    printf("====================================================");

    gotoxy(1, 23);
    printf("   12  11  10  #9  #8  #7  XX  #6  #5  #4  #3  #2  #1   ");
}

// Renders the board combining all elements
void drawboard(Pawns Board[]) {
    DrawBar(Board);
    DrawCourt(Board);
    WhiteHome(Board);
    Redhome(Board);
    LeftTop(Board);
    LeftDown(Board);
    DrawBorders();

    int x = 1;
    int y = 6;
    for (int i = 0; i < 17; ++i) {
        gotoxy(x, y);
        printf("::");
        y++;
    }

    x = 55;
    y = 6;
    for (int i = 0; i < 17; ++i) {
        gotoxy(x, y);
        printf("::");
        y++;
    }
}