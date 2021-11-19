#ifndef MESSAGES_H_
#define MESSAGES_H_

#include <chess/chess.h>

#include <cJSON.h>

char* data_to_json(State*, int);
void json_to_data(char*, State**, int*);

/** TEST */
void test(State**, int*);

#endif