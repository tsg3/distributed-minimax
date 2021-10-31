/**
 * @file configuration.h
 * @author Esteban Campos Granados (este0111@hotmail.com)
 * @brief Minimax configuration module.
 * @version 0.1
 * @date 2021-10-31
 */
#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <minimax/minimax.h>

/** @fn configure_minimax
 * @brief Executes the configuration of the minimax algorithm.
 * 
 * @return Pointer to the minimax configuration struct.
 * 
 * Script that invokes methods to create the minimax configuration struct.
 */
Configuration* configure_minimax();

/** @fn check_topology
 * @brief Checks the topology in the configuration file and sets it to a char variable.
 * 
 * @param topology  cJSON topology object parsed from the JSON configuration file.
 * @param conf      Pointer to final char variable.
 * @return Boolean value; 'true' if parsing succeded.
 */

/** @fn check_turn
 * @brief Checks the player in the configuration file and sets it to a bool variable.
 * 
 * @param turn  cJSON turn object parsed from the JSON configuration file.
 * @param conf  Pointer to final bool variable.
 * @return Boolean value; 'true' if parsing succeded.
 */

/** @fn check_iters
 * @brief Checks the iterations in the configuration file and sets them to a int variable.
 * 
 * @param iters cJSON iterations object parsed from the JSON configuration file.
 * @param conf  Pointer to final integer variable.
 * @return Boolean value; 'true' if parsing succeded.
 */

/** @fn check_nodes
 * @brief Checks the nodes selected in the configuration file and sets them to the
 * configuration struct.
 * 
 * @param nodes cJSON nodes array object parsed from the JSON configuration file.
 * @param conf  Pointer to configuration struct.
 * @return Boolean value; 'true' if parsing succeded.
 */

/** @fn check_pieces
 * @brief Checks the pieces indicated in the configuration file and sets it to a state
 * configuration struct.
 * 
 * @param item  cJSON array of piece JSON objects parsed from the JSON configuration file.
 * @param state Pointer to the configured state struct.
 * @return Boolean value; 'true' if parsing succeded.
 */
bool check_topology(const cJSON*, char*);
bool check_turn(const cJSON*, bool*);
bool check_iters(const cJSON*, int*);
bool check_nodes(const cJSON*, Configuration*);
bool check_pieces(const cJSON*, State*);

/** @fn add_piece
 * @brief Adds a piece in the list of pieces of a player in a chess state struct.
 * 
 * @param state     Chess state struct.
 * @param piece     Chess piece.
 * @param player    Boolean player value; 1 = white pieces, 0 = black pieces.
 * @return void
 */
void add_piece(State*, Piece*, bool);

/** @fn cJSON_bool_to_bool
 * @brief Sets the value of a bool variable according to the boolean value of a 
 * cJSON_bool object.
 * 
 * @param item  Pointer to the cJSON_bool variable.
 * @param var   Pointer to final bool variable.
 * @return Boolean value; 'true' if parsing succeded.
 */
bool cJSON_bool_to_bool(const cJSON*, bool*);

/** @fn print_configuration
 * @brief Prints the variables' values of a configuration struct.
 * 
 * @param conf  Configuration struct variable.
 * @return void
 */
void print_configuration(Configuration*);

/** @fn delete_configuration
 * @brief Frees the memory allocated for a configuration struct variable.
 * 
 * @param conf  Configuration struct variable.
 * @return void
 */
void delete_configuration(Configuration*);

#endif