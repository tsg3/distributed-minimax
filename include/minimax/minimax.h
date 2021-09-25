#ifndef MINIMAX_H_
#define MINIMAX_H_

#include <chess/chess.h>
#include <limits.h>

void init_chess_models(bool);
void clean_passant_temp();
int minmax(int, int, int);
int calc_level(State*, int);
int check_possible_final(State*, Piece*, int, int);

#endif