#pragma once


#ifndef BOARD_H
#define BOARD_H

// Declarations of global constants
const int BASECOLOR = 8;
const int COLORTWO = 15;
const int COLORONE = 4;
const int BAR = 25;
const int COURT = 0;
const int MAXPAWNS = 15;
const int MAXFIELDS = 25;
const int FIRSTFIELD = 1;
const int LASTFIELD = 24;
// Structure defining the state of a single board field
struct Pawns {
    int WhitePawns;
    int RedPawns;
};

// Function prototypes available to other modules
void BoardInitialization(Pawns Board[]);
void drawboard(Pawns Board[]);

#endif