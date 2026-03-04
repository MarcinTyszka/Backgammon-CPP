#include "board.h"
#include "ui.h"
#include "conio2.h"
#include <stdio.h>



#define ANSI_RESET "\x1b[0m"
#define ANSI_BG_BROWN "\x1b[48;5;130m"
#define ANSI_RED_PAWN "\x1b[31;1m"
#define ANSI_WHT_PAWN "\x1b[37;1m"
#define ANSI_FRAME "\x1b[33m"

// Initialize board with starting pawns
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

// Print a single pawn or empty space
void DrawPawnSlot(int red, int white, int currentLevel) {
    if (red >= currentLevel) {
        printf(ANSI_RED_PAWN "(X)" ANSI_RESET);
    }
    else if (white >= currentLevel) {
        printf(ANSI_WHT_PAWN "(X)" ANSI_RESET);
    }
    else {
        printf(ANSI_BG_BROWN " | " ANSI_RESET);
    }
}

// Draw the top half of the board
void DrawTopHalf(Pawns Board[]) {
    int startY = POS_Y_BOARD_TOP;

    gotoxy(POS_X_BOARD, startY++);
    printf(ANSI_FRAME " 13  14  15  16  17  18   XX    19  20  21  22  23  24 " ANSI_RESET);
    gotoxy(POS_X_BOARD, startY++);
    printf(ANSI_FRAME "==========================================================" ANSI_RESET);

    for (int level = 1; level <= 6; ++level) {
        gotoxy(POS_X_BOARD, startY++);
        for (int i = 13; i <= 18; ++i) {
            DrawPawnSlot(Board[i].RedPawns, Board[i].WhitePawns, level);
            printf(" ");
        }

        printf(ANSI_FRAME "  ||   " ANSI_RESET);

        for (int i = 19; i <= 24; ++i) {
            DrawPawnSlot(Board[i].RedPawns, Board[i].WhitePawns, level);
            printf(" ");
        }
    }
}

// Draw the bottom half of the board
void DrawBottomHalf(Pawns Board[]) {
    int startY = POS_Y_BOARD_BOT;

    gotoxy(POS_X_BOARD, startY++);
    printf(ANSI_FRAME "----------------------------------------------------------" ANSI_RESET);

    for (int level = 6; level >= 1; --level) {
        gotoxy(POS_X_BOARD, startY++);
        for (int i = 12; i >= 7; --i) {
            DrawPawnSlot(Board[i].RedPawns, Board[i].WhitePawns, level);
            printf(" ");
        }

        printf(ANSI_FRAME "  ||   " ANSI_RESET);

        for (int i = 6; i >= 1; --i) {
            DrawPawnSlot(Board[i].RedPawns, Board[i].WhitePawns, level);
            printf(" ");
        }
    }

    gotoxy(POS_X_BOARD, startY++);
    printf(ANSI_FRAME "==========================================================" ANSI_RESET);
    gotoxy(POS_X_BOARD, startY++);
    printf(ANSI_FRAME " 12  11  10   9   8   7   XX    6   5   4   3   2   1 " ANSI_RESET);
}

// Draw captured pawns on the bar
void DrawBarState(Pawns Board[]) {
    gotoxy(POS_X_BAR, POS_Y_BAR_W);
    if (Board[BAR].WhitePawns > 0) {
        printf(ANSI_WHT_PAWN "W:%d" ANSI_RESET, Board[BAR].WhitePawns);
    }
    gotoxy(POS_X_BAR, POS_Y_BAR_R);
    if (Board[BAR].RedPawns > 0) {
        printf(ANSI_RED_PAWN "R:%d" ANSI_RESET, Board[BAR].RedPawns);
    }
}

// Draw pawns that have been borne off
void DrawCourtState(Pawns Board[]) {
    gotoxy(POS_X_SIDEBAR, POS_Y_COURT_R);
    printf(ANSI_RED_PAWN "Red Court: %d" ANSI_RESET, Board[COURT].RedPawns);
    gotoxy(POS_X_SIDEBAR, POS_Y_COURT_W);
    printf(ANSI_WHT_PAWN "Wht Court: %d" ANSI_RESET, Board[COURT].WhitePawns);
}

// Render complete board
void drawboard(Pawns Board[]) {
    DrawTopHalf(Board);
    DrawBottomHalf(Board);
    DrawBarState(Board);
    DrawCourtState(Board);

    gotoxy(POS_X_PROMPT, POS_Y_PROMPT);
}