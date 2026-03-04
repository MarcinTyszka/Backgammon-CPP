#include "ui.h"
#include "conio2.h"
#include <stdio.h>

#define ANSI_CYAN "\x1b[36;1m"
#define ANSI_RED "\x1b[31;1m"
#define ANSI_WHITE "\x1b[37;1m"
#define ANSI_RESET "\x1b[0m"

// Renders header with player scores
void drawinterface(int* WP, int* PR) {
    gotoxy(POS_X_HEADER, POS_Y_HEADER);
    printf(ANSI_CYAN "=== BACKGAMMON TERMINAL EDITION ===" ANSI_RESET "\n");
    printf("\x1b[31;1m Player One (Red) \x1b[0m | \x1b[37;1m Player Two (White) \x1b[0m\n\n");
}

// Renders main play menu
void DrawMenu() {
    int startY = POS_Y_MENU;
    gotoxy(POS_X_SIDEBAR, startY++);
    printf(ANSI_CYAN "ACTIONS:" ANSI_RESET);
    gotoxy(POS_X_SIDEBAR, startY++);
    printf("(n) Next move");
    gotoxy(POS_X_SIDEBAR, startY++);
    printf("(s) Save game");
    gotoxy(POS_X_SIDEBAR, startY++);
    printf("(l) Load game");
    gotoxy(POS_X_SIDEBAR, startY++);
    printf("(q) Quit to title");
}

// Renders replay menu
void DrawShowMenu() {
    int startY = POS_Y_MENU;
    gotoxy(POS_X_SIDEBAR, startY++);
    printf(ANSI_CYAN "REPLAY MENU:" ANSI_RESET);
    gotoxy(POS_X_SIDEBAR, startY++);
    printf("(n) Next move");
    gotoxy(POS_X_SIDEBAR, startY++);
    printf("(p) Prev move");
    gotoxy(POS_X_SIDEBAR, startY++);
    printf("(f) First move");
    gotoxy(POS_X_SIDEBAR, startY++);
    printf("(l) Last move");
    gotoxy(POS_X_SIDEBAR, startY++);
    printf("(q) Quit replay");
}

// Renders main title screen
void DrawTitle() {
    clrscr();
    gotoxy(1, 2);

    printf(ANSI_RED);
    printf("  ____             _                                              \n");
    printf(" |  _ \\           | |                                             \n");
    printf(" | |_) | __ _  ___| | ____ _  __ _ _ __ ___  _ __ ___   ___  _ __ \n");
    printf(" |  _ < / _` |/ __| |/ / _` |/ _` | '_ ` _ \\| '_ ` _ \\ / _ \\| '_ \\\n");
    printf(" | |_) | (_| | (__|   < (_| | (_| | | | | | | | | | | | (_) | | | |\n");
    printf(" |____/ \\__,_|\\___|_|\\_\\__, |\\__,_|_| |_| |_|_| |_| |_|\\___/|_| |_|\n");
    printf("                        __/ |                                     \n");
    printf("                       |___/                                      \n");
    printf(ANSI_RESET "\n");

    printf(ANSI_CYAN);
    printf("  ==============================================================\n");
    printf("  ||                                                          ||\n");
    printf("  ||       " ANSI_WHITE "(p) Play new game" ANSI_CYAN "                                  ||\n");
    printf("  ||       " ANSI_WHITE "(l) Load game from file" ANSI_CYAN "                            ||\n");
    printf("  ||       " ANSI_WHITE "(s) Show game replay" ANSI_CYAN "                               ||\n");
    printf("  ||       " ANSI_WHITE "(q) Quit" ANSI_CYAN "                                           ||\n");
    printf("  ||                                                          ||\n");
    printf("  ==============================================================\n");
    printf(ANSI_RESET "\n");

}