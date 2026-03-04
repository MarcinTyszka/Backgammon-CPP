#include "conio2.h"
#include "Board.h"
#include "ui.h"
#include "file_io.h"
#include "logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

using namespace std;


void EnableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return;
    }
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        return;
    }
    dwMode |= 0x0004;
    SetConsoleMode(hOut, dwMode);
}


int main() {
    EnableANSI();
    textcolor(BASECOLOR);
    Pawns Board[MAXFIELDS + 1];
    srand(time(NULL));
    DrawTitle();
    char K;
    int X = 0;
    int Ws = 0;
    int Rs = 0;
    int* WP = &Ws;
    int* RP = &Rs;
    while (!X) {
        K = getch();
        int player = 0;
        switch (K) {
        case 'p':
            ClearGameInfoFile();
            BoardInitialization(Board);
            clrscr();
            PlayGame(Board, player, WP, RP);
            DrawTitle();
            break;
        case 'l':
            ClearGameInfoFile();
            LoadGame(Board, &player);
            PlayGame(Board, player, WP, RP);
            break;
        case 's':
            clrscr();
            GameShow(Board);
            clrscr();
            DrawTitle();
            break;
        case 'q':
            clrscr();
            SaveStats(WP, RP);
            X = 1;
            break;
        default:
            break;
        }
    }
    return 0;
}