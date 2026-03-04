#pragma once
#ifndef LOGIC_H
#define LOGIC_H

#include "board.h"

void PlayGame(Pawns Board[], int player, int* WP, int* RP);
void GameShow(Pawns Board[]);
void SaveStats(int* WP, int* RP);

#endif