#include "logic.h"
#include "ui.h"
#include "board.h"
#include "conio2.h"
#include "file_io.h"

#include <stdlib.h>
#include <stdio.h>

// Clear prompt area safely without triggering terminal scroll
void ClearPromptArea() {
    for (int i = 0; i < 20; i++) {
        gotoxy(POS_X_PROMPT, POS_Y_PROMPT + i);
        // Overwrite the line with spaces (80 characters)
        printf("                                                                                ");
    }
    gotoxy(POS_X_PROMPT, POS_Y_PROMPT);
}

int GetMoveInput() {
    int number = 0;
    char ch;

    printf("\x1b[37;1m");

    while (true) {
        ch = getch();

        if (ch == '\r' || ch == '\n') {
            break;
        }
        else if (ch >= '0' && ch <= '9') {
            printf("%c", ch);
            number = number * 10 + (ch - '0');
        }
        else if (ch == '\b' && number > 0) {
            number = number / 10;
            printf("\b \b");
        }
    }

    printf("\x1b[0m\n");
    return number;
}


// Reads text input from console and handles visual echo
void GetStringInput(char* buffer, int maxLength) {
    int index = 0;
    char ch;

    printf("\x1b[37;1m");

    while (true) {
        ch = getch();

        if (ch == '\r' || ch == '\n') {
            break;
        }
        else if (ch == '\b' && index > 0) {
            index--;
            printf("\b \b");
        }
        else if (ch >= 32 && ch <= 126 && index < maxLength - 1) {
            buffer[index++] = ch;
            printf("%c", ch);
        }
    }

    buffer[index] = '\0';
    printf("\x1b[0m\n");
}


// Checks if all player's active pawns are in their home board
bool IsAllHome(Pawns Board[], int player) {
    int pawnsInHome = 0;
    int startIdx = (player == 1) ? 19 : 1;
    int endIdx = (player == 1) ? 24 : 6;

    for (int i = startIdx; i <= endIdx; ++i) {
        pawnsInHome += (player == 1) ? Board[i].RedPawns : Board[i].WhitePawns;
    }

    int courtPawns = (player == 1) ? Board[COURT].RedPawns : Board[COURT].WhitePawns;
    return pawnsInHome == (MAXPAWNS - courtPawns);
}

bool Bar(Pawns Board[], int player) {
    return (player == 1) ? (Board[BAR].RedPawns > 0) : (Board[BAR].WhitePawns > 0);
}

bool CheckWin(Pawns Board[], int player) {
    return (player == 1) ? (Board[COURT].RedPawns == MAXPAWNS) : (Board[COURT].WhitePawns == MAXPAWNS);
}

bool ValidMove(Pawns Board[], int chosenPawn, int player, int newRedPosition, int newWhitePosition) {
    if (chosenPawn < FIRSTFIELD || chosenPawn > LASTFIELD) {
        return false;
    }

    if (player == 1) {
        if (Board[chosenPawn].RedPawns == 0 || Board[newRedPosition].WhitePawns > 1) {
            return false;
        }
        if (newRedPosition < FIRSTFIELD || newRedPosition > LASTFIELD) {
            return false;
        }
    }

    if (player == 2) {
        if (Board[chosenPawn].WhitePawns == 0 || Board[newWhitePosition].RedPawns > 1) {
            return false;
        }
        if (newWhitePosition < FIRSTFIELD || newWhitePosition > LASTFIELD) {
            return false;
        }
    }

    return true;
}


bool ValidCourtmove(Pawns Board[], int player, int dice, int chosenPawn) {
    if (!IsAllHome(Board, player) || Bar(Board, player)) return false;

    bool equalCourt = false;
    int furthestPawn = 0;

    if (player == 1) {
        for (int i = LASTFIELD; i >= 19; --i) {
            if (Board[i].RedPawns > 0) {
                furthestPawn = i;
                if (i + dice == 25) {
                    equalCourt = true;
                    if (chosenPawn == i) return true;
                }
            }
        }
        return !equalCourt && (furthestPawn + dice > 25) && (chosenPawn == furthestPawn);
    }
    else if (player == 2) {
        for (int i = 1; i <= 6; ++i) {
            if (Board[i].WhitePawns > 0) {
                furthestPawn = i;
                if (i - dice == 0) {
                    equalCourt = true;
                    if (chosenPawn == i) return true;
                }
            }
        }
        return !equalCourt && (furthestPawn - dice < 0) && (chosenPawn == furthestPawn);
    }

    return false;
}

bool ValidMoveFromBar(Pawns Board[], int player, int newRedPosition, int newWhitePosition) {
    if (player == 1) {
        if (Board[newRedPosition].WhitePawns > 1) {
            return false;
        }
    }

    if (player == 2) {
        if (Board[newWhitePosition].RedPawns > 1) {
            return false;
        }
    }

    if ((newRedPosition < FIRSTFIELD || newRedPosition > LASTFIELD) || (newWhitePosition < FIRSTFIELD || newWhitePosition > LASTFIELD)) {
        return false;
    }

    return true;
}

bool Capture(Pawns Board[], int player, int newRedPosition, int newWhitePosition) {
    if (player == 1) {
        if (Board[newRedPosition].WhitePawns == 1) {
            return true;
        }
    }

    if (player == 2) {
        if (Board[newWhitePosition].RedPawns == 1) {
            return true;
        }
    }

    return false;
}

bool IspossibleMoveFromBar(Pawns Board[], int dice, int player) {
    if (player == 1) {
        if (Board[dice].WhitePawns > 1) {
            return false;
        }
    }
    if (player == 2) {
        if (Board[BAR - dice].RedPawns > 1) {
            return false;
        }
    }
    return true;
}



// Check and print possible normal moves
bool IspossibleMove(Pawns Board[], int player, int dice) {
    bool possible = false;
    int capturePawn = 0;

    ClearPromptArea();

    printf("\x1b[33mValid moves by %d:\x1b[0m ", dice);

    for (int i = FIRSTFIELD; i <= LASTFIELD; ++i) {
        if (player == 1 && Board[i].RedPawns > 0 && i + dice <= LASTFIELD && Board[i + dice].WhitePawns == 1) {
            capturePawn = i;
        }
        else if (player == 2 && Board[i].WhitePawns > 0 && i - dice >= FIRSTFIELD && Board[i - dice].RedPawns == 1) {
            capturePawn = i;
        }
    }

    for (int i = FIRSTFIELD; i <= LASTFIELD; ++i) {
        if (capturePawn != 0 && i == capturePawn) {
            possible = true;
            printf(" \x1b[31;1m[%d (Capture)]\x1b[0m", capturePawn);
            break;
        }

        if (player == 1 && Board[i].RedPawns > 0 && i + dice <= LASTFIELD && Board[i + dice].WhitePawns == 0) {
            possible = true;
            printf(" [%d]", i);
        }
        else if (player == 2 && Board[i].WhitePawns > 0 && i - dice >= FIRSTFIELD && Board[i - dice].RedPawns == 0) {
            possible = true;
            printf(" [%d]", i);
        }
    }

    if (possible) printf("\nSelect pawn: ");
    return possible;
}

// Checks if the player has any valid move to the court without printing anything
bool HasValidCourtMove(Pawns Board[], int player, int dice) {
    if (!IsAllHome(Board, player) || Bar(Board, player)) return false;

    bool possible = false;
    bool exactMatch = false;
    int furthest = 0;

    if (player == 1) {
        for (int i = LASTFIELD; i >= 19; --i) {
            if (Board[i].RedPawns > 0) {
                furthest = i;
                if (i + dice == 25) {
                    possible = true;
                    exactMatch = true;
                }
            }
        }
        if (!exactMatch && (furthest + dice > 25)) {
            possible = true;
        }
    }
    else {
        for (int i = 1; i <= 6; ++i) {
            if (Board[i].WhitePawns > 0) {
                furthest = i;
                if (i - dice == 0) {
                    possible = true;
                    exactMatch = true;
                }
            }
        }
        if (!exactMatch && (furthest - dice < 0)) {
            possible = true;
        }
    }

    return possible;
}

// Checks and prints available court moves
bool PrintCourtMoves(Pawns Board[], int player, int dice) {
    if (!IsAllHome(Board, player) || Bar(Board, player)) return false;

    bool possible = false;
    bool exactMatch = false;
    int furthest = 0;

    ClearPromptArea();

    printf("\x1b[33mCourt moves by %d:\x1b[0m ", dice);

    if (player == 1) {
        for (int i = LASTFIELD; i >= 19; --i) {
            if (Board[i].RedPawns > 0) {
                furthest = i;
                if (i + dice == 25) {
                    printf(" [%d]", i);
                    possible = true;
                    exactMatch = true;
                }
            }
        }
        if (!exactMatch && (furthest + dice > 25)) {
            printf(" [%d]", furthest);
            possible = true;
        }
    }
    else {
        for (int i = 1; i <= 6; ++i) {
            if (Board[i].WhitePawns > 0) {
                furthest = i;
                if (i - dice == 0) {
                    printf(" [%d]", i);
                    possible = true;
                    exactMatch = true;
                }
            }
        }
        if (!exactMatch && (furthest - dice < 0)) {
            printf(" [%d]", furthest);
            possible = true;
        }
    }
    if (possible) printf("\nSelect pawn: ");
    return possible;
}

void CaptureMove(Pawns Board[], int chosenPawn, int player, int newRed, int newWhite) {
    if (player == 1) {
        Board[newRed].WhitePawns--;
        Board[chosenPawn].RedPawns--;
        Board[BAR].WhitePawns++;
        Board[newRed].RedPawns++;
    }
    if (player == 2) {
        Board[newWhite].RedPawns--;
        Board[chosenPawn].WhitePawns--;
        Board[BAR].RedPawns++;
        Board[newWhite].WhitePawns++;
    }
}


void NormalMove(Pawns Board[], int player, int chosenPawn, int newRed, int newWhite) {
    if (Capture(Board, player, newRed, newWhite)) {
        CaptureMove(Board, chosenPawn, player, newRed, newWhite);
    }
    else {

        if (player == 1) {
            Board[chosenPawn].RedPawns--;
            Board[newRed].RedPawns++;
        }
        else if (player == 2) {
            Board[chosenPawn].WhitePawns--;
            Board[newWhite].WhitePawns++;
        }
    }
}

void FromBarMove(Pawns Board[], int player, int dice) {
    int newRed = dice;
    int newWhite = BAR - dice;

    if (ValidMoveFromBar(Board, player, newRed, newWhite)) {
        NormalMove(Board, player, BAR, newRed, newWhite);
    }
}

void CourtMove(Pawns Board[], int chosenPawn, int player) {
    if (player == 1) {
        Board[chosenPawn].RedPawns--;
        Board[COURT].RedPawns++;
    }
    else if (player == 2) {
        Board[chosenPawn].WhitePawns--;
        Board[COURT].WhitePawns++;
    }
}

void CourtSystem(Pawns Board[], int player, int dice, int chosenPawn);
void NormalSystem(Pawns Board[], int player, int dice, int chosenPawn);
void BarSystem(Pawns Board[], int player, int dice);

void MoveSystem(Pawns Board[], int dice, int player) {
    int chosenPawn = 0;

    CourtSystem(Board, player, dice, chosenPawn);

    NormalSystem(Board, player, dice, chosenPawn);

    BarSystem(Board, player, dice);
}

void CourtSystem(Pawns Board[], int player, int dice, int chosenPawn) {
    if (IsAllHome(Board, player) && !Bar(Board, player)) {
        if (HasValidCourtMove(Board, player, dice)) {

            PrintCourtMoves(Board, player, dice);

            chosenPawn = GetMoveInput();

            if (ValidCourtmove(Board, player, dice, chosenPawn)) {
                CourtMove(Board, chosenPawn, player);
                drawboard(Board);
            }
            else {
                printf(" Wrong Move\n");
                MoveSystem(Board, dice, player);
            }
        }
        else if (!HasValidCourtMove(Board, player, dice)) {
            printf("Can't go to court ");
        }
    }
}

// Handles normal moves on the board
void NormalSystem(Pawns Board[], int player, int dice, int chosenPawn) {
    if (!Bar(Board, player) && !HasValidCourtMove(Board, player, dice)) {

        if (IspossibleMove(Board, player, dice)) {

            chosenPawn = GetMoveInput();

            int newRed = chosenPawn + dice;
            int newWhite = chosenPawn - dice;

            if (ValidMove(Board, chosenPawn, player, newRed, newWhite)) {
                NormalMove(Board, player, chosenPawn, newRed, newWhite);
                drawboard(Board);
            }
            else {
                printf("Wrong Move\n");
                MoveSystem(Board, dice, player);
            }
        }
        else {
            printf("No possible move\n");
        }
    }
}

void BarSystem(Pawns Board[], int player, int dice) {
    if (Bar(Board, player)) {
        ClearPromptArea();
        if (IspossibleMoveFromBar(Board, dice, player)) {
            printf("\x1b[36;1mMoving from bar using dice [%d].\x1b[0m\n", dice);
            printf("Press Enter to confirm move...");
            while (getch() != '\r');
            FromBarMove(Board, player, dice);
            drawboard(Board);
        }
        else {
            printf("\x1b[31;1mCannot move from bar using dice [%d].\x1b[0m\n", dice);
            printf("Press any key...");
            getch();
        }
    }
}

int RollDice() {
    return (rand() % 6) + 1;
}

// Clean and aesthetic dice roll for the first turn
int WhoFirst() {
    ClearPromptArea();
    printf("\x1b[36;1m=== DECIDING STARTING PLAYER ===\x1b[0m\n\n");

    printf("Player One (Red), press any key to roll...\n");
    getch();
    int P1Roll = RollDice();
    printf("Red rolled: \x1b[31;1m[ %d ]\x1b[0m\n\n", P1Roll);

    printf("Player Two (White), press any key to roll...\n");
    getch();
    int P2Roll = RollDice();
    printf("White rolled: \x1b[37;1m[ %d ]\x1b[0m\n\n", P2Roll);

    if (P1Roll > P2Roll) {
        printf("\x1b[31;1mRED STARTS THE GAME!\x1b[0m\nPress any key to begin...");
        getch();
        return 1;
    }
    else if (P2Roll > P1Roll) {
        printf("\x1b[37;1mWHITE STARTS THE GAME!\x1b[0m\nPress any key to begin...");
        getch();
        return 2;
    }
    else {
        printf("\x1b[33mDRAW! Rolling again...\x1b[0m\nPress any key...");
        getch();
        return WhoFirst();
    }
}
void FirstDice(Pawns Board[], int dice1, int dice2, int player) {
    printf("First move:\n");
    MoveSystem(Board, dice1, player);
    printf("\nSecond move:\n");
    MoveSystem(Board, dice2, player);
}

void SecondDice(Pawns Board[], int dice1, int dice2, int player) {
    printf("First move:\n");
    MoveSystem(Board, dice2, player);
    printf("\nSecond move:\n");
    MoveSystem(Board, dice1, player);
}

// User interface for choosing dice order
void DiceChoice(Pawns Board[], int player, int dice1, int dice2) {
    printf("\x1b[36;1mChoose dice to play first:\x1b[0m\n");
    printf("(1) Play \x1b[33m[%d]\x1b[0m then \x1b[33m[%d]\x1b[0m\n", dice1, dice2);
    printf("(2) Play \x1b[33m[%d]\x1b[0m then \x1b[33m[%d]\x1b[0m\n", dice2, dice1);

    char key = getch(); // Changed from getchar

    ClearPromptArea();
    if (key == '1') {
        FirstDice(Board, dice1, dice2, player);
    }
    else if (key == '2') {
        SecondDice(Board, dice1, dice2, player);
    }
    else {
        DiceChoice(Board, player, dice1, dice2);
    }
}

void IfBarDisplay(Pawns Board[], int player, int dice1, int dice2) {
    ClearPromptArea();
    printf("\x1b[33m[!] PAWNS ON THE BAR [!]\x1b[0m\n");
    printf("Player %d rolled \x1b[33m[%d]\x1b[0m and \x1b[33m[%d]\x1b[0m\n", player, dice1, dice2);
    printf("You must remove pawns from bar first.\n\n");

    bool First = IspossibleMoveFromBar(Board, dice1, player);
    bool Second = IspossibleMoveFromBar(Board, dice2, player);

    if (!First && !Second) {
        printf("\x1b[31;1mNo possible moves from bar.\x1b[0m\nPress any key to skip turn...");
        getch();
    }
    else if (!First && Second) {
        SecondDice(Board, dice1, dice2, player);
    }
    else if (First && !Second) {
        FirstDice(Board, dice1, dice2, player);
    }
    else if (First && Second) {
        DiceChoice(Board, player, dice1, dice2);
    }
}

// Main turn display
void MovingDisplay(Pawns Board[], int player) {
    ClearPromptArea();

    int dice1 = RollDice();
    int dice2 = RollDice();

    if (player == 1) {
        printf("\x1b[31;1m--- RED PLAYER TURN ---\x1b[0m\n");
    }
    else {
        printf("\x1b[37;1m--- WHITE PLAYER TURN ---\x1b[0m\n");
    }
    printf("Rolled: \x1b[33m[ %d ]\x1b[0m and \x1b[33m[ %d ]\x1b[0m\n\n", dice1, dice2);

    if (Bar(Board, player)) {
        IfBarDisplay(Board, player, dice1, dice2);
    }
    else {
        DiceChoice(Board, player, dice1, dice2);
    }

    if (dice1 == dice2) {
        ClearPromptArea();
        printf("\x1b[36;1m--- DOUBLE ROLL BONUS ---\x1b[0m\n");
        printf("Rolled: \x1b[33m[ %d ]\x1b[0m and \x1b[33m[ %d ]\x1b[0m\n\n", dice1, dice2);

        printf("\nThird move:\n");
        MoveSystem(Board, dice1, player);
        printf("\nFourth move:\n");
        MoveSystem(Board, dice2, player);
    }
}

void RedWinInfo(Pawns Board[], int* RP) {
    clrscr();
    gotoxy(1, 1);
    printf("RED WIN");
    getch();
    clrscr();
    if (Board[BAR].WhitePawns > 0) {
        (*RP) += 3;
    }
    else if (Board[COURT].WhitePawns == 0) {
        (*RP) += 2;
    }
    else {
        (*RP)++;
    }
}

void WhiteWinInfo(Pawns Board[], int* WP) {
    clrscr();
    gotoxy(1, 1);
    printf("WHITE WIN");
    getch();
    clrscr();
    if (Board[BAR].RedPawns > 0) {
        (*WP) += 3;
    }
    else if (Board[COURT].RedPawns == 0) {
        (*WP) += 2;
    }
    else {
        (*WP)++;
    }
}

void WinningInfo(Pawns Board[], int* WP, int* RP) {
    if (CheckWin(Board,1)) {
        RedWinInfo(Board, RP);
    }
    else if (CheckWin(Board,2)) {
        WhiteWinInfo(Board, WP);
    }
    DrawTitle();
}

// Saves player scores at the end of the game
void SaveStats(int* WP, int* RP) {
    gotoxy(1, 1);
    char playerName[MAXNL];
    int WhiteScore = *(WP);
    int RedScore = *(RP);

    if (RedScore != 0) {
        printf("\nPlayer one (Red) write your name: ");
        GetStringInput(playerName, MAXNL);
        SaveScore(playerName, RedScore);
    }

    if (WhiteScore != 0) {
        printf("\n\nPlayer two (White) write your name: ");
        GetStringInput(playerName, MAXNL);
        SaveScore(playerName, WhiteScore);
    }

    HallOfFame();
}

void PlayGame(Pawns Board[], int player, int* WP, int* RP) {
    char K;
    int X = 0;
    if (player == 0) {
        player = WhoFirst();
    }

    while (!CheckWin(Board,1) && !CheckWin(Board,2)) {
        if (X == 1) {
            break;
        }

        clrscr();
        drawinterface(WP, RP);
        DrawMenu();
        drawboard(Board);
        K = getch();
        switch (K) {
        case 'n':
            MovingDisplay(Board, player);
            AlwaysSave(Board);
            player = (player == 1) ? 2 : 1;
            break;
        case 's':
            SaveGame(Board, player);
            break;
        case 'l':
            LoadGame(Board, &player);
            break;
        case 'q':
            clrscr();
            DrawTitle();
            X = 1;
            break;
        default:
            break;
        }
    }

    WinningInfo(Board, WP, RP);
}

void GameShow(Pawns Board[]) {
    int move = 0;
    char Key;
    int X = 0;

    clrscr();
    BoardInitialization(Board);
    drawboard(Board);

    while (!X) {
        DrawShowMenu();
        Key = getch();

        switch (Key) {
        case 'n':
            if (move < GetLastMoveNumber()) {
                move++;
            }
            Replay(move, Board);
            drawboard(Board);
            break;
        case 'p':
            if (move > 0) {
                move--;
                Replay(move, Board);
            }
            if (move == 0) {
                BoardInitialization(Board);
            }
            drawboard(Board);
            break;
        case 'f':
            move = 1;
            Replay(move, Board);
            drawboard(Board);
            break;
        case 'l':
            move = GetLastMoveNumber();
            Replay(move, Board);
            drawboard(Board);
            break;
        case 'q':
            clrscr();
            DrawTitle();
            X = 1;
            break;
        default:
            break;
        }
    }
}