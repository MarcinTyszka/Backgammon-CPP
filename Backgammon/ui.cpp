#include "ui.h"
#include "conio2.h"
#include <stdio.h>



void drawinterface(int* WP, int* PR) {
    gotoxy(1, 1);
    printf("Marcin Tyszka Data Engineering gr.3 ID:198293 PROJECT BACKGAMMON\n");
    textcolor(COLORONE);
    printf("Player One: %d ", *PR);
    textcolor(COLORTWO);
    printf(" Player Two: %d", *WP);
    textcolor(BASECOLOR);
}

void DrawMenu() {
    gotoxy(MENUX, MENUY);
    printf("MENU:\n");
    gotoxy(MENUX, MENUY + 2);
    printf("(n)ext move\n");
    gotoxy(MENUX, MENUY + 4);
    printf("(s)ave game\n");
    gotoxy(MENUX, MENUY + 6);
    printf("(l)oad game\n");
    gotoxy(MENUX, MENUY + 8);
    printf("(q)uit game\n");
}

void DrawShowMenu() {
    gotoxy(MENUX, MENUY);
    printf("MENU:\n");
    gotoxy(MENUX, MENUY + 2);
    printf("(n)ext move\n");
    gotoxy(MENUX, MENUY + 4);
    printf("(p)revius move\n");
    gotoxy(MENUX, MENUY + 6);
    printf("(f)irst move\n");
    gotoxy(MENUX, MENUY + 8);
    printf("(l)ast move\n");
    gotoxy(MENUX, MENUY + 10);
    printf("(q)uit show\n");
}

void DrawTitle() {
    gotoxy(1, 1);
    printf("###################\n# BACKGAMMON GAME #\n###################\n\n");
    printf("(p)lay game\n\n(l)oad game from file\n\n(s)how the game\n\n(q)uit game\n");
}

