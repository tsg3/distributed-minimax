#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <chess/chess.h>

typedef struct Configuration
{
    char topology;
    bool turn;
    int iters;
    bool node_1;
    bool node_2;
    bool node_3;
    bool node_4;
    State* initial_state;
} Configuration;

Configuration* configure_minimax();

bool check_topology(const cJSON*, char*);
bool check_turn(const cJSON*, bool*);
bool check_iters(const cJSON*, int*);
bool check_nodes(const cJSON*, Configuration*);
bool check_pieces(const cJSON*, State*);

void add_piece(State*, Piece*, bool);

bool cJSON_bool_to_bool(const cJSON*, bool*);

void print_configuration(Configuration*);

#endif