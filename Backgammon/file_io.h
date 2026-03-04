#pragma once
#ifndef FILE_IO_H
#define FILE_IO_H

#include "board.h"

const int MAXBUFFER = 150;
const int MAXMOVES = 1000;
const int MAXP = 100;
const int MAXNL = 30;

int GetLastMoveNumber();
void Replay(int moveNumber, Pawns Board[]);
void AlwaysSave(Pawns Board[]);
void ClearGameInfoFile();
void SaveGame(Pawns Board[], int player);
void LoadGame(Pawns Board[], int* player);
void SaveScore(const char* player, int score);
void HallOfFame();

#endif