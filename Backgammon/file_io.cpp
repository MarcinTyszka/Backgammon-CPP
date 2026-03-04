#include "file_io.h"
#include <stdio.h>

// Retrieves the last move number from the save file
int GetLastMoveNumber() {
    FILE* file;
    int lastMove = -1;

    if (fopen_s(&file, "game_info.txt", "r") != 0) {
        printf("\nUnable to open file");
        return -1;
    }

    if (file != NULL) {
        char buffer[MAXMOVES];
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
    char buff[MAXBUFFER];
    int currentMove = -1;

    while (fgets(buff, sizeof(buff), file) != NULL) {
        if (sscanf_s(buff, "Save Number: %d", &currentMove) == 1 && currentMove == Nmove) {
            for (int i = 0; i <= MAXFIELDS; ++i) {
                fgets(buff, sizeof(buff), file);
                int whitePawns, redPawns;
                if (sscanf_s(buff, "Position %*d: White Pawns - %d, Red Pawns - %d", &whitePawns, &redPawns) == 2) {
                    Board[i].WhitePawns = whitePawns;
                    Board[i].RedPawns = redPawns;
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

    if (fopen_s(&file, "game_info.txt", "r") != 0) {
        printf("\nUnable to open file");
        return;
    }

    printf("Move Number: %d\n", moveNumber);
    UpdateBoard(file, Board, moveNumber);
    fclose(file);
}

// Appends current board state to the game history file
void AlwaysSave(struct Pawns Board[]) {
    static int moveNumber = 1;
    FILE* file;

    if (fopen_s(&file, "game_info.txt", "a") != 0) {
        return;
    }

    if (file != NULL) {
        fprintf(file, "Save Number: %d\n", moveNumber);
        for (int i = 0; i <= MAXFIELDS; ++i) {
            fprintf(file, "Position %d: White Pawns - %d, Red Pawns - %d\n", i, Board[i].WhitePawns, Board[i].RedPawns);
        }
        fprintf(file, "\n");
        fclose(file);
        moveNumber++;
    }
}

// Clears game history before starting a new session
void ClearGameInfoFile() {
    FILE* file;
    errno_t err = fopen_s(&file, "game_info.txt", "w");

    if (err != 0) {
        printf("\nUnable to open file for clearing.");
        return;
    }
    fclose(file);
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

// Appends player score to the scoreboard file
void SaveScore(const char* player, int score) {
    FILE* file = NULL;
    if (fopen_s(&file, "Scores.txt", "a") != 0) {
        if (fopen_s(&file, "Scores.txt", "w") != 0) {
            return;
        }
    }
    fprintf(file, "%s %d\n", player, score);
    fclose(file);
}

// Internal function to read and aggregate scores
void readScores(FILE* Ifile, char Name[MAXP][MAXNL], int scores[MAXP], int total[MAXP], int* numP) {
    while (fscanf_s(Ifile, "%s %d", Name[*numP], MAXNL, &scores[*numP]) != EOF && (*numP) < MAXP) {
        int Found = 0;
        for (int i = 0; i < (*numP); ++i) {
            int match = 1;
            for (int j = 0; Name[i][j] != '\0' || Name[*numP][j] != '\0'; ++j) {
                if (Name[i][j] != Name[*numP][j]) {
                    match = 0;
                    break;
                }
            }
            if (match) {
                total[i] += scores[*numP];
                Found = 1;
                break;
            }
        }
        if (!Found) {
            total[*numP] += scores[*numP];
            (*numP)++;
        }
    }
}

// Internal function to write aggregated scores
void writeScores(FILE* Wfile, char Name[MAXP][MAXNL], int total[MAXP], int numP) {
    for (int i = 0; i < numP; ++i) {
        fprintf(Wfile, "%s %d\n", Name[i], total[i]);
    }
}

// Updates the Hall of Fame file by aggregating all player scores
void HallOfFame() {
    FILE* Ifile = NULL;
    FILE* Wfile = NULL;

    fopen_s(&Ifile, "Scores.txt", "r");
    fopen_s(&Wfile, "Hall_Of_Fame.txt", "w");

    if (Ifile == NULL || Wfile == NULL) {
        return;
    }

    char Name[MAXP][MAXNL];
    int scores[MAXP] = { 0 };
    int total[MAXP] = { 0 };
    int numP = 0;

    readScores(Ifile, Name, scores, total, &numP);
    writeScores(Wfile, Name, total, numP);

    fclose(Ifile);
    fclose(Wfile);
}