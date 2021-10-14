#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct configuration
{
    char topology;
    bool turn;
    int iters;
} configuration;

configuration* configure_minimax();

bool check_topology(const cJSON*, configuration*);
bool check_turn(const cJSON*, configuration*);
bool check_iters(const cJSON*, configuration*);

#endif