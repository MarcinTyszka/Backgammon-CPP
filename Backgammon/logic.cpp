#include "logic.h"
#include "ui.h"
#include "board.h"
#include "conio2.h"
#include "file_io.h"

#include <stdlib.h>
#include <stdio.h>



bool IsAllHome(Pawns Board[], int player) {
    int PawnsInHome = 0;

    if (player == 1) {
        for (int i = 19; i <= 24; ++i) {
            if (Board[i].RedPawns > 0) {
                PawnsInHome += Board[i].RedPawns;
            }
        }
        if (PawnsInHome == MAXPAWNS - Board[COURT].RedPawns) {
            return true;
        }
    }
    else if (player == 2) {
        for (int i = 6; i >= 1; --i) {
            if (Board[i].WhitePawns > 0) {
                PawnsInHome += Board[i].WhitePawns;
            }
        }
        if (PawnsInHome == MAXPAWNS - Board[COURT].WhitePawns) {
            return true;
        }
    }

    return false;
}

bool Bar(Pawns Board[], int player) {
    if (player == 1) {
        if (Board[BAR].RedPawns > 0) {
            return true;
        }
    }
    if (player == 2) {
        if (Board[BAR].WhitePawns > 0) {
            return true;
        }
    }
    return false;
}

bool RedWin(Pawns Board[]) {
    if (Board[COURT].RedPawns == MAXPAWNS) {
        return true;
    }
    return false;
}

bool WhiteWin(Pawns Board[]) {
    if (Board[COURT].WhitePawns == MAXPAWNS) {
        return true;
    }
    return false;
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

bool ValidCourtmoveRed(Pawns Board[], int player, int dice, int chosenPawn) {
    bool EqualCourt = false;
    int FurthestPawn = 0;

    if (player == 1 && IsAllHome(Board, player) && !Bar(Board, player)) {
        for (int i = LASTFIELD; i >= 19; --i) {
            if (Board[i].RedPawns > 0) {
                FurthestPawn = i;
                if (i + dice == 25) {
                    EqualCourt = true;
                    if (chosenPawn == i) {
                        return true;
                    }
                }
            }
        }
        if (!EqualCourt && (FurthestPawn + dice > 25)) {
            if (chosenPawn == FurthestPawn) {
                return true;
            }
        }
    }
    return false;
}

bool ValidCourtmoveWhite(Pawns Board[], int player, int dice, int chosenPawn) {
    bool EqualCourt = false;
    int FurthestPawn = 0;

    if (player == 2 && IsAllHome(Board, player) && !Bar(Board, player)) {
        for (int i = 1; i <= 6; ++i) {
            if (Board[i].WhitePawns > 0) {
                FurthestPawn = i;
                if (i - dice == 0) {
                    EqualCourt = true;
                    if (chosenPawn == i) {
                        return true;
                    }
                }
            }
        }
        if (!EqualCourt && (FurthestPawn - dice < 0)) {
            if (chosenPawn == FurthestPawn) {
                return true;
            }
        }
    }
    return false;
}

bool ValidCourtmove(Pawns Board[], int player, int dice, int chosenPawn) {
    if (player == 1 && ValidCourtmoveRed(Board, player, dice, chosenPawn)) {
        return true;
    }

    if (player == 2 && ValidCourtmoveWhite(Board, player, dice, chosenPawn)) {
        return true;
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

bool IspossibleMoveRed(Pawns Board[], int player, int dice) {
    bool Possible = false;
    int Capture = 0;

    if (player == 1) {
        printf("Possible moves:");

        for (int i = FIRSTFIELD; i <= LASTFIELD; ++i) {
            if (Board[i].RedPawns > 0) {
                if (Board[i + dice].WhitePawns == 1 && i + dice <= LASTFIELD) {
                    Capture = i;
                }
            }
        }

        for (int i = FIRSTFIELD; i <= LASTFIELD; ++i) {
            if (Capture != 0) {
                Possible = true;
                printf(" Capture for (%d)", Capture);
                break;
            }
            if (Board[i].RedPawns > 0 && Board[i + dice].WhitePawns == 0 && i + dice <= LASTFIELD) {
                Possible = true;
                printf(" (%d)", i);
            }
        }
    }

    if (Possible) {
        return true;
    }
    return false;
}

bool IspossibleMoveWhite(Pawns Board[], int player, int dice) {
    bool Possible = false;
    int Capture = 0;

    if (player == 2) {
        printf("Possible moves:");

        for (int i = FIRSTFIELD; i <= LASTFIELD; ++i) {
            if (Board[i].WhitePawns > 0) {
                if (Board[i - dice].RedPawns == 1 && i - dice >= FIRSTFIELD) {
                    Capture = i;
                }
            }
        }

        for (int i = FIRSTFIELD; i <= LASTFIELD; ++i) {
            if (Capture != 0) {
                Possible = true;
                printf(" Capture for (%d)", Capture);
                break;
            }
            if (Board[i].WhitePawns > 0 && Board[i - dice].RedPawns == 0 && i - dice >= FIRSTFIELD) {
                Possible = true;
                printf(" (%d)", i);
            }
        }
    }

    if (Possible) {
        return true;
    }
    return false;
}

bool IspossibleMove(Pawns Board[], int player, int dice) {
    if (player == 1 && IspossibleMoveRed(Board, player, dice)) {
        return true;
    }
    if (player == 2 && IspossibleMoveWhite(Board, player, dice)) {
        return true;
    }

    return false;
}

bool CanGoToCourtRed(Pawns Board[], int player, int dice) {
    bool IsPossible = false;
    bool EqualCourt = false;
    int FurthestPawn = 0;

    if (player == 1 && IsAllHome(Board, player) && !Bar(Board, player)) {
        printf("Court moves: ");

        for (int i = LASTFIELD; i >= 19; --i) {
            if (Board[i].RedPawns > 0) {
                FurthestPawn = i;
                if (i + dice == 25) {
                    printf(" (%d)", i);
                    IsPossible = true;
                    EqualCourt = true;
                }
            }
        }
        if (!EqualCourt && (FurthestPawn + dice > 25)) {
            printf(" (%d)", FurthestPawn);
            IsPossible = true;
        }
    }

    if (IsPossible) {
        return true;
    }
    return false;
}

bool CanGoToCourtWhite(Pawns Board[], int player, int dice) {
    bool IsPossible = false;
    bool EqualCourt = false;
    int FurthestPawn = 0;

    if (player == 2 && IsAllHome(Board, player) && !Bar(Board, player)) {
        printf("Court moves: ");

        for (int i = 1; i <= 6; ++i) {
            if (Board[i].WhitePawns > 0) {
                FurthestPawn = i;
                if (i - dice == 0) {
                    printf(" (%d)", i);
                    IsPossible = true;
                    EqualCourt = true;
                }
            }
        }
        if (!EqualCourt && (FurthestPawn - dice < 0)) {
            printf(" (%d)", FurthestPawn);
            IsPossible = true;
        }
    }

    if (IsPossible) {
        return true;
    }
    return false;
}

bool CanGoToCourt(Pawns Board[], int player, int dice) {
    if (player == 1 && IsAllHome(Board, player) && !Bar(Board, player) && CanGoToCourtRed(Board, player, dice)) {
        return true;
    }
    else if (player == 2 && IsAllHome(Board, player) && !Bar(Board, player) && CanGoToCourtWhite(Board, player, dice)) {
        return true;
    }
    return false;
}

bool CanGoToCourtNORed(Pawns Board[], int player, int dice) {
    bool Possible = false;
    bool Equal = false;
    int Furthest = 0;

    if (player == 1 && IsAllHome(Board, player) && !Bar(Board, player)) {
        for (int i = LASTFIELD; i >= 19; --i) {
            if (Board[i].RedPawns > 0) {
                Furthest = i;
                if (i + dice == 25) {
                    Possible = true;
                    Equal = true;
                }
            }
        }
        if (!Equal && (Furthest + dice > 25)) {
            Possible = true;
        }
    }
    if (Possible) {
        return true;
    }
    return false;
}

bool CanGoToCourtNOWhite(Pawns Board[], int player, int dice) {
    bool Possible = false;
    bool Equal = false;
    int Furthest = 0;

    if (player == 2 && IsAllHome(Board, player) && !Bar(Board, player)) {
        for (int i = 1; i <= 6; ++i) {
            if (Board[i].WhitePawns > 0) {
                Furthest = i;
                if (i - dice == 0) {
                    Possible = true;
                    Equal = true;
                }
            }
        }
        if (!Equal && (Furthest - dice < 0)) {
            Possible = true;
        }
    }
    if (Possible) {
        return true;
    }
    return false;
}

bool CanGoToCourtNO(Pawns Board[], int player, int dice) {
    if (player == 1 && IsAllHome(Board, player) && !Bar(Board, player) && CanGoToCourtNORed(Board, player, dice)) {
        return true;
    }
    else if (player == 2 && IsAllHome(Board, player) && !Bar(Board, player) && CanGoToCourtNOWhite(Board, player, dice)) {
        return true;
    }
    return false;
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

void CaptureMoveFromBar(Pawns Board[], int player, int newRed, int newWhite) {
    if (player == 1) {
        Board[BAR].RedPawns--;
        Board[newRed].WhitePawns--;
        Board[BAR].WhitePawns++;
        Board[newRed].RedPawns++;
    }
    if (player == 2) {
        Board[BAR].WhitePawns--;
        Board[newWhite].RedPawns--;
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

        if (Capture(Board, player, newRed, newWhite)) {
            CaptureMoveFromBar(Board, player, newRed, newWhite);
        }
        else {

            if (player == 1) {
                Board[BAR].RedPawns--;
                Board[newRed].RedPawns++;
            }
            else if (player == 2) {
                Board[BAR].WhitePawns--;
                Board[newWhite].WhitePawns++;
            }
        }
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
        if (CanGoToCourtNO(Board, player, dice)) {

            CanGoToCourt(Board, player, dice);
            if (scanf_s("%d", &chosenPawn) != 1) {
                printf("Invalid input.\nPress any key to continue.\n");
                scanf_s("%*s");
            }

            if (ValidCourtmove(Board, player, dice, chosenPawn)) {
                CourtMove(Board, chosenPawn, player);
            }
            else {
                printf(" Wrong Move\n");
                MoveSystem(Board, dice, player);
            }
        }
        else if (!CanGoToCourtNO(Board, player, dice)) {
            printf("Can't go to court ");
        }
    }
}

void NormalSystem(Pawns Board[], int player, int dice, int chosenPawn) {
    if (!Bar(Board, player) && !CanGoToCourtNO(Board, player, dice)) {

        if (IspossibleMove(Board, player, dice)) {

            if (scanf_s("%d", &chosenPawn) != 1) {
                printf("Invalid input\nPress any key to continue\n");
                scanf_s("%*s");
            }

            int newRed = chosenPawn + dice;
            int newWhite = chosenPawn - dice;

            if (ValidMove(Board, chosenPawn, player, newRed, newWhite)) {
                NormalMove(Board, player, chosenPawn, newRed, newWhite);
            }
            else {
                printf("Wrong Move\n");
                MoveSystem(Board, dice, player);
            }
        }
        else if (!IspossibleMove(Board, player, dice)) {
            printf("No possible move");
        }
    }
}

void BarSystem(Pawns Board[], int player, int dice) {
    if (Bar(Board, player)) {
        if (IspossibleMoveFromBar(Board, dice, player)) {
            printf("Moving from bar for (%d):\n", dice);
            FromBarMove(Board, player, dice);
        }
        else {
            printf("Can't move\n");
        }
    }
}

int RollDice() {
    return (rand() % 6) + 1;
}

int WhoFirst() {
    gotoxy(1, 1);
    int P1Roll = RollDice();
    int P2Roll = RollDice();

    printf("Throwing dice to decide who is starting\nPlayer One (red) throw a dice\n");
    getch();
    printf("Player One: %d\nPlayer Two (white) throw a dice\n", P1Roll);
    getch();
    printf("Player Two: %d\n", P2Roll);

    if (P1Roll > P2Roll) {
        printf("Red starts the game!\nPress 'n' to play");
        getch();
        return 1;
    }
    else if (P2Roll > P1Roll) {
        printf("White starts the game!\nPress 'n' to play");
        getch();
        return 2;
    }
    else {
        printf("Draw! Throw again.\n");
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

void DiceChoice(Pawns Board[], int player, int dice1, int dice2) {
    printf("Choose dice\npress '1' or '2'\n");
    char key = getchar();
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
    printf("\n\nPlayer %d rolled %d and %d\nYou need to remove pawns from bar\n", player, dice1, dice2);

    bool First = IspossibleMoveFromBar(Board, dice1, player);
    bool Second = IspossibleMoveFromBar(Board, dice2, player);

    if (!First && !Second) {
        printf("\nFirst move:\nCan't move\nSecond move: \nCan't move\n");
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
        getch();
    }
}

void MovingDisplay(Pawns Board[], int player) {
    int dice1, dice2;
    dice1 = RollDice();
    dice2 = RollDice();

    if (Bar(Board, player)) {
        IfBarDisplay(Board, player, dice1, dice2);
    }
    else if (!Bar(Board, player)) {
        printf("\n\nPlayer %d rolled %d and %d\n", player, dice1, dice2);
        DiceChoice(Board, player, dice1, dice2);
    }

    if (dice1 == dice2) {
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
    if (RedWin(Board)) {
        RedWinInfo(Board, RP);
    }
    else if (WhiteWin(Board)) {
        WhiteWinInfo(Board, WP);
    }
    DrawTitle();
}

void SaveStats(int* WP, int* RP) {
    gotoxy(1, 1);
    char playerName[MAXNL];
    int WhiteScore = *(WP);
    int RedScore = *(RP);
    if (RedScore != 0) {
        printf("\nPlayer one (Red) write your name:");
        scanf_s("%s", playerName, sizeof(playerName));
        SaveScore(playerName, RedScore);
    }
    if (WhiteScore != 0) {
        printf("\n\nPlayer two (White) write your name:\n");
        scanf_s("%s", playerName, sizeof(playerName));
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

    while (!RedWin(Board) && !WhiteWin(Board)) {
        if (X == 1) {
            break;
        }

        clrscr();
        drawinterface(WP, RP);
        DrawMenu();
        drawboard(Board);
        K = getchar();
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

    BoardInitialization(Board);
    drawboard(Board);

    while (!X) {
        DrawShowMenu();
        Key = getchar();
        gotoxy(1, 1);

        switch (Key) {
        case 'n':
            clrscr();
            if (move < GetLastMoveNumber()) {
                move++;
            }
            Replay(move, Board);
            break;
        case 'p':
            clrscr();
            if (move > 0) {
                move--;
                Replay(move, Board);
            }
            if (move == 0) {
                BoardInitialization(Board);
                drawboard(Board);
            }
            break;
        case 'f':
            clrscr();
            move = 1;
            Replay(move, Board);
            break;
        case 'l':
            clrscr();
            move = GetLastMoveNumber();
            Replay(move, Board);
            break;
        case 'q':
            X = 1;
            break;
        default:
            break;
        }
    }
}