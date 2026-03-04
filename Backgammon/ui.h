#pragma once
#ifndef UI_H
#define UI_H

#include "board.h"


const int MENUX = 95;
const int MENUY = 9;

const int POS_X_HEADER = 1;
const int POS_Y_HEADER = 1;

const int POS_X_BOARD = 1;
const int POS_Y_BOARD_TOP = 4;
const int POS_Y_BOARD_BOT = 12;

const int POS_X_BAR = 27;
const int POS_Y_BAR_W = 7;
const int POS_Y_BAR_R = 14;

const int POS_X_SIDEBAR = 62;
const int POS_Y_COURT_R = 6;
const int POS_Y_MENU = 9;
const int POS_Y_COURT_W = 17;

const int POS_X_PROMPT = 1;
const int POS_Y_PROMPT = 21;



void DrawTitle();
void DrawMenu();
void DrawShowMenu();
void drawinterface(int* WP, int* PR);

#endif