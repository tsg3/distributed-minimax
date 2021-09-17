#ifndef CHESS_H_
#define CHESS_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int king_valid_moves[8][2];

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
void calcMove(Piece*, int, int, int[], int[]);

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