#ifndef CHESS_H_
#define CHESS_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int directions[8][2];
int knight_movements[8][2];

// Piece

typedef struct Piece
{
    char type;
    int posX;
    int posY;
    struct Piece *next;
} Piece;

Piece* create_piece(char, int, int);
void move(Piece*, int, int);
void calcMove(Piece*, int[], int[], int[]);
void calcMove_extended_aux(Piece*, int, int, int[], int[], int[]);
void calcMove_single_aux(Piece*, int*, int[], int[]);

// State

typedef struct State
{
    Piece* whitePieces;
    Piece* blackPieces;
    int value;
    bool turn;
} State;

int get_value(State* state);
void calc_value(State* state, bool player);

#endif