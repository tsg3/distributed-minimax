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
    int white_castling;
    int black_castling;
} State;

int directions[8][2];
int knight_movements[8][2];
int player;
Piece* pawn_passant;
Piece* temp_passant;
bool can_passant;

void init();

Piece* create_piece(char, int, int);
State* create_state(bool, int, int);
State* create_copy(State*);
void delete_piece(State*, Piece*, bool);
void delete_state(State*);

void move(Piece*, int, int);
void calcMove(State*, Piece*, int[], int[], int[]);
void calcMove_extended_aux(State*, Piece*, int, int, int[], int[], int[]);
void calcMove_single_aux(State*, Piece*, int*, int[], int[]);
void calcMove_pawn_aux(State*, Piece*, int[], int[]);
void move_piece(State*, Piece*, int, int);

void check_castling(State*, int*);
bool check_castle_interrupt(State*, int);
void castle(State*, int, Piece**, Piece**, int*, int*);

int get_value(State*);
bool calc_value(State*, bool);

int check_obstacle(State*, int, int);

#endif