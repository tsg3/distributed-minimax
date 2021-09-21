#ifndef CHESS_H_
#define CHESS_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Piece

typedef struct Piece
{
    char type;
    int posX;
    int posY;
    struct Piece *next;
} Piece;

typedef struct State
{
    Piece* whitePieces;
    Piece* blackPieces;
    int value;
    bool turn;
} State;

int directions[8][2];
int knight_movements[8][2];

Piece* create_piece(char, int, int);
State* create_state(bool);

void move(Piece*, int, int);
void calcMove(State*, Piece*, int[], int[], int[]);
void calcMove_extended_aux(State*, Piece*, int, int, int[], int[], int[]);
void calcMove_single_aux(Piece*, int*, int[], int[]);

int get_value(State* state);
void calc_value(State* state, bool player);

int check_obstacle(State*, int, int);

#endif