#include "file_io.h"
#include <string.h>
#include "conio2.h"
#include <stdio.h>

// Retrieves the last move number from the save file safely
int GetLastMoveNumber() {
    FILE* file;
    int lastMove = 0;

    if (fopen_s(&file, "game_info.txt", "r") == 0 && file != NULL) {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            int currentMove;
            if (sscanf_s(buffer, "Save Number: %d", &currentMove) == 1) {
                lastMove = currentMove;
            }
        }
        fclose(file);
    }
    return lastMove;
}

// Internal function to update board state during a replay
void UpdateBoard(FILE* file, Pawns Board[], int Nmove) {
    char buff[256];
    int currentMove = -1;

    while (fgets(buff, sizeof(buff), file) != NULL) {
        if (sscanf_s(buff, "Save Number: %d", &currentMove) == 1 && currentMove == Nmove) {
            for (int i = 0; i <= MAXFIELDS; ++i) {
                if (fgets(buff, sizeof(buff), file) != NULL) {
                    int whitePawns, redPawns;
                    if (sscanf_s(buff, "Position %*d: White Pawns - %d, Red Pawns - %d", &whitePawns, &redPawns) == 2) {
                        Board[i].WhitePawns = whitePawns;
                        Board[i].RedPawns = redPawns;
                    }
                }
            }
            break;
        }
    }
    drawboard(Board);
}

// Replays a specific move number from the game history
void Replay(int moveNumber, Pawns Board[]) {
    FILE* file;

    if (fopen_s(&file, "game_info.txt", "r") != 0 || file == NULL) {
        return;
    }

    gotoxy(62, 7);
    printf("\x1b[36;1mReplay Move:\x1b[33m %-4d\x1b[0m", moveNumber);

    UpdateBoard(file, Board, moveNumber);
    fclose(file);
}

// Appends current board state to the game history file dynamically
void AlwaysSave(struct Pawns Board[]) {
    FILE* file;

    int nextMove = GetLastMoveNumber() + 1;

    if (fopen_s(&file, "game_info.txt", "a") == 0 && file != NULL) {
        fprintf(file, "Save Number: %d\n", nextMove);
        for (int i = 0; i <= MAXFIELDS; ++i) {
            fprintf(file, "Position %d: White Pawns - %d, Red Pawns - %d\n", i, Board[i].WhitePawns, Board[i].RedPawns);
        }
        fprintf(file, "\n");
        fclose(file);
    }
}

// Clears game history before starting a new session
void ClearGameInfoFile() {
    FILE* file;
    if (fopen_s(&file, "game_info.txt", "w") == 0 && file != NULL) {
        fclose(file);
    }
}

// Saves the current game state and player turn to allow resuming later
void SaveGame(struct Pawns Board[], int player) {
    FILE* file;

    if (fopen_s(&file, "game_state.txt", "w") == 0) {
        if (file != NULL) {
            fprintf(file, "Current Player: %d\n", player);
            for (int i = 0; i <= MAXFIELDS; ++i) {
                fprintf(file, "Position %d: White Pawns - %d, Red Pawns - %d\n", i, Board[i].WhitePawns, Board[i].RedPawns);
            }
            fclose(file);
            printf("\n\nGame state has been saved");
        }
        else {
            printf("\nUnable to open file");
        }
    }
    else {
        printf("\nFailed to open file");
    }
}

// Loads game state and current player turn from file
void LoadGame(struct Pawns Board[], int* player) {
    FILE* file;

    errno_t err = fopen_s(&file, "game_state.txt", "r");
    if (err != 0) {
        printf("\nUnable to open file");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        int currentPlayer;

        if (sscanf_s(line, "Current Player: %d", &currentPlayer) == 1) {
            *player = currentPlayer;
        }
        else {
            int position, whitePawns, redPawns;
            if (sscanf_s(line, "Position %d: White Pawns - %d, Red Pawns - %d", &position, &whitePawns, &redPawns) == 3) {
                Board[position].WhitePawns = whitePawns;
                Board[position].RedPawns = redPawns;
            }
        }
    }
    fclose(file);
}

// Appends player score to the file
void SaveScore(const char* player, int score) {
    FILE* file;
    if (fopen_s(&file, "Scores.txt", "a") == 0) {
        fprintf(file, "%s %d\n", player, score);
        fclose(file);
    }
}

// Reads scores and aggregates them for the same player
void readScores(FILE* f, char N[MAXP][MAXNL], int T[MAXP], int* p) {
    char n[MAXNL];
    int s;
    *p = 0;

    while (fscanf_s(f, "%29s %d", n, (unsigned)sizeof(n), &s) == 2) {
        int fnd = 0;
        for (int i = 0; i < *p; ++i) {
            if (strcmp(N[i], n) == 0) {
                T[i] += s;
                fnd = 1;
                break;
            }
        }
        if (!fnd && *p < MAXP) {
            strcpy_s(N[*p], MAXNL, n);
            T[*p] = s;
            (*p)++;
        }
    }
}

// Sorts players by score in descending order
void SortScores(char N[MAXP][MAXNL], int T[MAXP], int p) {
    for (int i = 0; i < p - 1; ++i) {
        for (int j = 0; j < p - i - 1; ++j) {
            if (T[j] < T[j + 1]) {
                int ts = T[j];
                T[j] = T[j + 1];
                T[j + 1] = ts;

                char tn[MAXNL];
                strcpy_s(tn, MAXNL, N[j]);
                strcpy_s(N[j], MAXNL, N[j + 1]);
                strcpy_s(N[j + 1], MAXNL, tn);
            }
        }
    }
}

// Displays the formatted leaderboard on screen
void DisplayHallOfFame(char N[MAXP][MAXNL], int T[MAXP], int p) {
    clrscr();
    gotoxy(1, 1);
    printf("\x1b[36;1m=== HALL OF FAME ===\x1b[0m\n\n");
    printf("\x1b[33mRank  Player                         Score\x1b[0m\n");
    printf("------------------------------------------\n");

    int limit = (p < 10) ? p : 10;
    for (int i = 0; i < limit; ++i) {
        printf("%-4d  %-30s \x1b[32;1m%d\x1b[0m\n", i + 1, N[i], T[i]);
    }

    printf("\nPress any key to continue...");
    getch();
}

// Main logic for handling the Hall of Fame
void HallOfFame() {
    FILE* in;
    FILE* out;
    if (fopen_s(&in, "Scores.txt", "r") != 0) return;

    char N[MAXP][MAXNL];
    int T[MAXP] = { 0 };
    int p = 0;

    readScores(in, N, T, &p);
    fclose(in);
    SortScores(N, T, p);

    if (fopen_s(&out, "Hall_Of_Fame.txt", "w") == 0) {
        for (int i = 0; i < p; ++i) {
            fprintf(out, "%s %d\n", N[i], T[i]);
        }
        fclose(out);
    }

    DisplayHallOfFame(N, T, p);
}