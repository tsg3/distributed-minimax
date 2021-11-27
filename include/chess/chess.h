/**
 * @file chess.h
 * @author Esteban Campos Granados (este0111@hotmail.com)
 * @brief Chess models module header.
 * @version 0.1
 * @date 2021-10-31
 */
#ifndef CHESS_H_
#define CHESS_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/** @typedef struct Piece
 * @brief Represents a chess piece. Used as a list.
 */
typedef struct Piece
{
    char type;          /**< Chess piece type:
                            P=Pawn  B=Bishop
                            Q=Queen K=King
                            R=Rook  N=Knight */
    int posX;           /**< X position. */
    int posY;           /**< Y position. */
    struct Piece *next; /**< Next piece on the list. */
} Piece;

/** @typedef struct State
 * @brief Represents a chess game position.
 */
typedef struct State
{
    Piece* whitePieces;     /**< List of white pieces. */
    Piece* blackPieces;     /**< List of black pieces. */
    int value;              /**< Game value. Initialized in a final state. */
    bool turn;              /**< Player turn. */
    int white_castling;     /**< Bit-flags for white castling. */
    int black_castling;     /**< Bit-flags for black castling. */
    Piece* pawn_passant;    /**< Pawn that moved two spaces last turn. */
    bool can_passant;       /**< Indicates when a pawn has been moved two spaces last
                                turn. */
    int fifty_rule;         /**< Counter for the fifty movement rule. */
} State;

/** @typedef struct Configuration
 * @brief Minimax execution initial configuration.
 */
typedef struct Configuration
{
    char topology;          /**< Distributed network topology. */
    bool turn;              /**< Player turn. True if white; otherwise, false.*/
    int iters;              /**< Number of iterations. */
    bool node_1;            /**< Selection of Node 1. */
    bool node_2;            /**< Selection of Node 2. */
    bool node_3;            /**< Selection of Node 3. */
    bool node_4;            /**< Selection of Node 4. */
    State* initial_state;   /**< Custom chess position. */
} Configuration;

/** Configuration* conf
 * @brief Minimax configuration.
 */
Configuration* conf;

/** @var int directions[8][2]
 * @brief Array with the 8 possible directions for a queen.
 */

/** @var int knight_movements[8][2]
 * @brief Array with the 8 possible directions for a knight.
 */
int directions[8][2];
int knight_movements[8][2];

/** @var Piece* temp_passant
 * @brief Tracking of pawn pieces for 'en passant' moves.
 */
Piece* temp_passant;

/** @fn init
 * @brief Initialize the chess directions arrays and 'temp_passant' pointer.
 * 
 * @return void
 */
void init();

/**
 * CRUD pieces and states.
 */

/** @fn create_piece
 * @brief Creates a piece object.
 * 
 * @param type  Type of piece.
 * @param x     X position.
 * @param y     Y position.
 * @return Piece* 
 */

/** @fn create_state
 * @brief Creates a state object.
 * 
 * @param turn              Player turn.
 * @param white_castling    White castling bit-flags.
 * @param black_castling    Black castling bit-flags.
 * @param pawn_passant      Pawn passant piece.
 * @param can_passant       Indicates if last turn a pawn moved two spaces.
 * @return State* 
 */

/** @fn create_copy
 * @brief Creates a copy of a state object.
 * 
 * @param old_state State to copy.
 * @return State* 
 */

/** @fn delete_piece
 * @brief Deletes a piece object from a state.
 * 
 * @param state     State.
 * @param piece     Piece from the state given.
 * @param player    Player who owns the piece.
 * @return void
 */

/** @fn delete_state
 * @brief Deletes a state object.
 * 
 * @param state State to be deleted.
 * @return void 
 */
Piece* create_piece(char, int, int);
State* create_state(bool, int, int, Piece*, bool);
State* create_copy(State*);
void delete_piece(State*, Piece*, bool);
void delete_state(State*);

/**
 * Direct movements functions.
 */

/** @fn move
 * @brief Updates the position of a piece.
 * 
 * @param piece Piece to be moved.
 * @param x     New X position.
 * @param y     New Y position.
 * @return void
 */

/** @fn calcMove
 * @brief Searches for the next valid move for a piece.
 * 
 * @param state             Current state.
 * @param piece             Piece attempting to move.
 * @param last_dir          Last direction analized.
 * @param last_pos          Last position retrieved (for extended directional movements).
 * @param res               Result tuple (int[2]), with a valid move.
 * @param check_if_attacked True if the algorithm has to check if a move leaves the
 *                          player's king in danger.
 * @return void
 */

/** @fn calcMove_extended_aux
 * @brief Computes a new valid move for a bishop, queen or rook.
 * 
 * @param state             Current state.
 * @param piece             Piece attempting to move.
 * @param start             Start value to iterate the directions array.
 * @param step              Step value to iterate the directions array.
 * @param last_dir          Last direction analized.
 * @param last_pos          Last position retrieved.
 * @param res               Result tuple (int[2]), with a valid move.
 * @param check_if_attacked True if the algorithm has to check if a move leaves the
 *                          player's king in danger.
 * @return void
 */

/** @fn calcMove_single_aux
 * @brief Computes a new valid move for a knight or a king.
 * 
 * @param state             Current state.
 * @param piece             Piece attempting to move.
 * @param movements         Pointer to a directions array.
 * @param last_dir          Last direction analized.
 * @param res               Result tuple (int[2]), with a valid move.
 * @param check_if_attacked True if the algorithm has to check if a move leaves the
 *                          player's king in danger.
 * @return void
 */

/** @fn calcMove_pawn_aux
 * @brief Computes a new valid move for a pawn.
 * 
 * @param state             Current state.
 * @param piece             Pawn attempting to move.
 * @param last_dir          Last direction analized.
 * @param res               Result tuple (int[2]), with a valid move.
 * @param check_if_attacked True if the algorithm has to check if a move leaves the
 *                          player's king in danger.
 * @return void
 */

/** @fn move_piece
 * @brief Moves a piece on the game.
 * 
 * @param state     Current state.
 * @param piece     Piece to move.
 * @param x         New X position for the piece.
 * @param y         New Y position for the piece.
 * @param promotion Indicates to which piece a pawn is going to be promoted ('\0' if not).
 * @return void
 */
void move(Piece*, int, int);
void calcMove(State*, Piece*, int[], int[], int[], bool);
void calcMove_extended_aux(State*, Piece*, int, int, int[], int[], int[], bool);
void calcMove_single_aux(State*, Piece*, int*, int[], int[], bool);
void calcMove_pawn_aux(State*, Piece*, int[], int[], bool);
void move_piece(State*, Piece*, int, int, char);

/** @fn check_castling
 * @brief Searches for the next valid castle move.
 * 
 * It updates @p res with the value of the next possible castle:
 *  1=Left castle
 *  2=Right castle
 * 
 * @param state Current state.
 * @param res   Next possible castle.
 * @return void
 */

/** @fn check_castle_interrupt
 * @brief Checks if a type of castle is valid in the current state.
 * 
 * @param state Current state.
 * @param type  Castling type.
 * @return false, if the castle can be made.
 * @return true, if the castle is blocked.
 */

/** @fn castle
 * @brief Looks for pieces and final positions of these after a castle.
 * 
 * @param state             Current state.
 * @param castle_type       Castling type.
 * @param king              Player's king.
 * @param rook              Rook to participate in the castle.
 * @param final_king_pos    Final king's position.
 * @param final_rook_pos    Final rook's position.
 * @return void
 */
void check_castling(State*, int*);
bool check_castle_interrupt(State*, int);
void castle(State*, int, Piece**, Piece**, int*, int*);

/** @fn get_value
 * @brief Get the value attribute of a state object.
 * 
 * @param state Current state.
 * @return State value.
 */

/** @fn calc_value
 * @brief Computes the value of a given state.
 * 
 * @p check_posibilities indicates the function mode:
 *  If false, it just checks if the current player's king is being attacked.
 *  Otherwise, it checks if the game ended and, in that case, updates the state's value.
 * 
 * @param state                 State struct.
 * @param check_posibilities    Function mode.
 * @return false, if @p check_posibilities is true and the king's is not being attacked,
 *              or if @p check_posibilities is false and the game is not finished.
 * @return true, if the conditions above are not reached (opposite case).
 */

/** @fn check_possible_attack
 * @brief Checks if the player's king is attacked after moving a piece.
 * 
 * @param state Current state.
 * @param piece Piece being moved.
 * @param x     Piece's new X position.
 * @param y     Piece's new Y position.
 * @return true, if the king is attacked after the move.
 * @return false, if the game can continue.
 */

/** @fn check_obstacle
 * @brief Checks if a certain space is being occupied by another piece.
 * 
 * @param state Current state.
 * @param x     Space's X position.
 * @param y     Space's Y position.
 * @return 2, if the space is being occupied by another piece of the player. 
 * @return 1, if the space is being occupied by a piece of the opponent player.
 * @return 0, if the space is unoccupied.
 */
int get_value(State*);
bool calc_value(State*, bool);
bool check_possible_attack(State*, Piece*, int, int);
int check_obstacle(State*, int, int);

#endif