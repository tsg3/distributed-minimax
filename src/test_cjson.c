#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <cJSON.h>

/** Samples */

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
    Piece* pawn_passant;
    bool can_passant;
    int fifty_rule;
} State;

/** ------- */

int main()
{
    /** INIT */

    State* s = (State*)malloc(sizeof(State));

    Piece* p1 = (Piece*)malloc(sizeof(Piece));
    Piece* p2 = (Piece*)malloc(sizeof(Piece));
    Piece* p3 = (Piece*)malloc(sizeof(Piece));

    s->whitePieces = p1;
    s->blackPieces = p3;
    s->value = -2;
    s->turn = true;
    s->white_castling = 0b111;
    s->black_castling = 0b111;
    s->pawn_passant = p2;
    s->can_passant = false;
    s->fifty_rule = 3;

    p1->type = 'K';
    p1->posX = 1;
    p1->posY = 2;
    p1->next = p2;

    p2->type = 'Q';
    p2->posX = 4;
    p2->posY = 3;
    p2->next = NULL;

    p3->type = 'K';
    p3->posX = 5;
    p3->posY = 7;
    p3->next = NULL;

    int level = 6;

    /** Data to JSON */

    cJSON* object = cJSON_CreateObject();

    cJSON* state = cJSON_CreateObject();

    cJSON* wp_attr = cJSON_CreateArray();
    cJSON* wp_item;
    Piece* temp = s->whitePieces;
    while (temp != NULL)
    {
        wp_item = cJSON_CreateObject();

        cJSON_AddStringToObject(wp_item, "t", &temp->type);
        cJSON_AddNumberToObject(wp_item, "x", temp->posX);
        cJSON_AddNumberToObject(wp_item, "y", temp->posY);
        cJSON_AddItemToArray(wp_attr, wp_item);

        temp = temp->next;
    }
    cJSON_AddItemToObject(state, "wp", wp_attr);

    cJSON* bp_attr = cJSON_CreateArray();
    cJSON* bp_item;
    temp = s->blackPieces;
    while (temp != NULL)
    {
        bp_item = cJSON_CreateObject();

        cJSON_AddStringToObject(bp_item, "t", &temp->type);
        cJSON_AddNumberToObject(bp_item, "x", temp->posX);
        cJSON_AddNumberToObject(bp_item, "y", temp->posY);
        cJSON_AddItemToArray(bp_attr, bp_item);

        temp = temp->next;
    }
    cJSON_AddItemToObject(state, "bp", bp_attr);

    cJSON_AddNumberToObject(state, "v", s->value);

    if (s->turn)
    {
        cJSON_AddBoolToObject(state, "t", cJSON_True);
    }
    else
    {
        cJSON_AddBoolToObject(state, "t", cJSON_False);
    }

    cJSON_AddNumberToObject(state, "wc", s->white_castling);
    cJSON_AddNumberToObject(state, "bc", s->black_castling);

    cJSON* pp_attr = cJSON_CreateObject();
    cJSON_AddNumberToObject(pp_attr, "x", s->pawn_passant->posX);
    cJSON_AddNumberToObject(pp_attr, "y", s->pawn_passant->posY);
    cJSON_AddItemToObject(state, "pp", pp_attr);

    if (s->can_passant)
    {
        cJSON_AddItemToObject(state, "cp", cJSON_CreateTrue());
    }
    else
    {
        cJSON_AddItemToObject(state, "cp", cJSON_CreateFalse());
    }

    cJSON_AddNumberToObject(state, "f", s->fifty_rule);

    cJSON_AddItemToObject(object, "s", state);

    cJSON_AddNumberToObject(object, "l", level);

    char* buffer = cJSON_Print(object);

    printf("%s\n", buffer);

    free(s);
    free(p1);
    free(p2);
    free(p3);
    cJSON_Delete(object);

    /** JSON to data */

    State* state_recv = (State*)malloc(sizeof(State));
    int level_recv;

    cJSON* message = cJSON_Parse(buffer);

    const cJSON* c_state_recv = cJSON_GetObjectItemCaseSensitive(message, "s");
    const cJSON* c_st_wp_recv = cJSON_GetObjectItemCaseSensitive(c_state_recv, "wp");
    cJSON* c_temp = c_st_wp_recv->child;
    cJSON *type_recv, *x_recv, *y_recv;
    Piece* new_piece; 
    Piece* last = NULL;
    while (c_temp != NULL)
    {
        type_recv = cJSON_GetObjectItemCaseSensitive(c_temp, "t");
        x_recv = cJSON_GetObjectItemCaseSensitive(c_temp, "x");
        y_recv = cJSON_GetObjectItemCaseSensitive(c_temp, "y");

        new_piece = (Piece*)malloc(sizeof(Piece));
        new_piece->type = type_recv->valuestring[0];
        new_piece->posX = x_recv->valueint;
        new_piece->posY = y_recv->valueint;
        new_piece->next = NULL;

        if (last == NULL)
        {
            state_recv->whitePieces = new_piece;
        }

        else
        {
            last->next = new_piece;
        }

        last = new_piece;
        c_temp = c_temp->next;
    }
    const cJSON* c_st_bp_recv = cJSON_GetObjectItemCaseSensitive(c_state_recv, "bp");
    c_temp = c_st_bp_recv->child;
    last = NULL;
    while (c_temp != NULL)
    {
        type_recv = cJSON_GetObjectItemCaseSensitive(c_temp, "t");
        x_recv = cJSON_GetObjectItemCaseSensitive(c_temp, "x");
        y_recv = cJSON_GetObjectItemCaseSensitive(c_temp, "y");

        new_piece = (Piece*)malloc(sizeof(Piece));
        new_piece->type = type_recv->valuestring[0];
        new_piece->posX = x_recv->valueint;
        new_piece->posY = y_recv->valueint;
        new_piece->next = NULL;

        if (last == NULL)
        {
            state_recv->blackPieces = new_piece;
        }

        else
        {
            last->next = new_piece;
        }

        last = new_piece;
        c_temp = c_temp->next;
    }
    const cJSON* c_st_v_recv = cJSON_GetObjectItemCaseSensitive(c_state_recv, "v");
    state_recv->value = c_st_v_recv->valueint;
    const cJSON* c_st_t_recv = cJSON_GetObjectItemCaseSensitive(c_state_recv, "t");
    if (cJSON_IsTrue(c_st_t_recv))
    {
        state_recv->turn = true;
    }
    else
    {
        state_recv->turn = false;
    }
    const cJSON* c_st_wc_recv = cJSON_GetObjectItemCaseSensitive(c_state_recv, "wc");
    state_recv->white_castling = c_st_wc_recv->valueint;
    const cJSON* c_st_bc_recv = cJSON_GetObjectItemCaseSensitive(c_state_recv, "bc");
    state_recv->black_castling = c_st_bc_recv->valueint;
    const cJSON* c_st_pp_recv = cJSON_GetObjectItemCaseSensitive(c_state_recv, "pp");
    const cJSON* c_st_pp_x = cJSON_GetObjectItemCaseSensitive(c_st_pp_recv, "x");
    const cJSON* c_st_pp_y = cJSON_GetObjectItemCaseSensitive(c_st_pp_recv, "y");
    bool found = false;
    temp = state_recv->whitePieces;
    Piece* temp_found;
    while (temp != NULL)
    {
        if (c_st_pp_x->valueint == temp->posX && c_st_pp_y->valueint == temp->posY)
        {
            found = true;
            break;
        }
        temp = temp->next;
    }
    if (!found)
    {
        temp = state_recv->blackPieces;
        while (temp != NULL)
        {
            if (c_st_pp_x->valueint == temp->posX && c_st_pp_y->valueint == temp->posY)
            {
                found = true;
                break;
            }
            temp = temp->next;
        }
    }
    state_recv->pawn_passant = temp;
    const cJSON* c_st_cp_recv = cJSON_GetObjectItemCaseSensitive(c_state_recv, "cp");
    if (cJSON_IsTrue(c_st_cp_recv))
    {
        state_recv->can_passant = true;
    }
    else
    {
        state_recv->can_passant = false;
    }
    const cJSON* c_st_f_recv = cJSON_GetObjectItemCaseSensitive(c_state_recv, "f");
    state_recv->fifty_rule = c_st_f_recv->valueint;

    const cJSON* c_level_recv = cJSON_GetObjectItemCaseSensitive(message, "l");
    level_recv = c_level_recv->valueint;

    free(buffer);
    cJSON_Delete(message);

    printf("----------------------------------------------------------------\n");
    printf("state:\n");
    temp = state_recv->whitePieces;
    while(temp != NULL)
    {
        printf("\t%c\n\t\t%d\n\t\t%d\n", temp->type, temp->posX, temp->posY);
        temp = temp->next;
    }
    temp = state_recv->blackPieces;
    while(temp != NULL)
    {
        printf("\t%c\n\t\t%d\n\t\t%d\n", temp->type, temp->posX, temp->posY);
        temp = temp->next;
    }
    printf("\tvalue: %d\n\tturn: %d\n\twhite_castling: %d\n\tblack_castling: %d\n",
        state_recv->value, state_recv->turn, 
        state_recv->white_castling, state_recv->black_castling);
    printf("\tPawn passant:\n\t\t%c\n\t\t\t%d\n\t\t\t%d\n",
        state_recv->pawn_passant->type, 
        state_recv->pawn_passant->posX, 
        state_recv->pawn_passant->posY);
    printf("\tCan passant?: %d\n\tFifty Rule: %d\nLevel: %d\n",
        state_recv->can_passant, state_recv->fifty_rule, level_recv);

    while (state_recv->whitePieces != NULL)
    {
        temp = state_recv->whitePieces;
        state_recv->whitePieces = temp->next;
        free(temp);
    }
    while (state_recv->blackPieces != NULL)
    {
        temp = state_recv->blackPieces;
        state_recv->blackPieces = temp->next;
        free(temp);
    }
    free(state_recv);

    return 0;
}