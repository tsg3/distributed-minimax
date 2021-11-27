/**
 * @file minimax.h
 * @author Esteban Campos Granados (este0111@hotmail.com)
 * @brief Minimax execution module source file.
 * @version 0.1
 * @date 2021-10-31
 */
#ifndef MINIMAX_H_
#define MINIMAX_H_

#include <chess/chess.h>
#include <limits.h>
#include <network/messages.h>

char node_id;

/** @fn init_chess_models
 * @brief Initiates variables of the chess models module.
 * 
 * @return void
 */
void init_chess_models();

/** @fn clean_passant_temp
 * @brief Frees the 'temp_passant' variable.
 * 
 * @return void
 */
void clean_passant_temp();

/** @fn minmax
 * @brief It retrieves the maximun/minimum between two integers.
 * 
 * @param in_1  First integer.
 * @param in_2  Second integer.
 * @param level Integer that decides which value is choosed. 
 * @return If level is even, returns the greater integer; otherwise, it returns the
 * lesser.
 */
int minmax(int, int, int);

/**
 * Minimax chess algorithm's main methods.
 */

/** @fn calc_level
 * @brief Computation of minimax value at a certain level of a tree.
 * 
 * @param state Current state of the chess game.
 * @param level Subtree integer level (depth).
 * @return The minimax value of the subtree.
 */

/** @fn calc_level_aux
 * @brief It moves a piece to a new position and evaluates the minimax value of the new
 * subtree.
 * 
 * @param state     Current state of the chess game.
 * @param level     Subtree integer level (depth).
 * @param piece     The piece that's being moved.
 * @param x         X position where the piece is going to be placed.
 * @param y         Y position where the piece is going to be placed.
 * @param promotion Char value that indicates if the piece (only pawns) will be promoted.
 * @return The minimax value of the new subtree obtained after making the move.
 */

/** @fn calc_level_castle
 * @brief Makes the castling move and evaluates the minimax value of the new subtree.
 * 
 * @param state     Current state of the chess game.
 * @param level     Subtree integer level (depth).
 * @param king      King's piece of the current player.
 * @param rook      Rook's being used for castling.
 * @param king_pos  Tuple (int[2]), final position of the king.
 * @param rook_pos  Tuple (int[2]), final position of the rook.
 * @return The minimax value of the new subtree obtained after making castling.
 */
int calc_level(State*, int);
int calc_level_aux(State*, int, Piece*, int, int, char);
int calc_level_castle(State*, int, Piece*, Piece*, int*, int*);

/** @fn check_possible_final
 * @brief Checks if after a move is made, the game ends.
 * 
 * @param state     Current state of the chess game.
 * @param piece     The piece that's being moved.
 * @param x         X position where the piece is going to be placed.
 * @param y         Y position where the piece is going to be placed.
 * @param promotion Char value that indicates if the piece (only pawns) will be promoted.
 * @return It returns -2 if the game continues; otherwise, it returns the game value. If
 * there's an error, -3 is returned.
 */
int check_possible_final(State*, Piece*, int, int, char);

#endif