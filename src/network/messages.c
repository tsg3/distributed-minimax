#include <network/messages.h>

char* data_to_json(State* state, int level)
{
    cJSON* object = cJSON_CreateObject();

    cJSON* c_state = cJSON_CreateObject();

    cJSON* wp_attr = cJSON_CreateArray();
    cJSON* wp_item;
    Piece* temp = state->whitePieces;
    while (temp != NULL)
    {
        wp_item = cJSON_CreateObject();

        cJSON_AddStringToObject(wp_item, "t", &temp->type);
        cJSON_AddNumberToObject(wp_item, "x", temp->posX);
        cJSON_AddNumberToObject(wp_item, "y", temp->posY);
        cJSON_AddItemToArray(wp_attr, wp_item);

        temp = temp->next;
    }
    cJSON_AddItemToObject(c_state, "wp", wp_attr);

    cJSON* bp_attr = cJSON_CreateArray();
    cJSON* bp_item;
    temp = state->blackPieces;
    while (temp != NULL)
    {
        bp_item = cJSON_CreateObject();

        cJSON_AddStringToObject(bp_item, "t", &temp->type);
        cJSON_AddNumberToObject(bp_item, "x", temp->posX);
        cJSON_AddNumberToObject(bp_item, "y", temp->posY);
        cJSON_AddItemToArray(bp_attr, bp_item);

        temp = temp->next;
    }
    cJSON_AddItemToObject(c_state, "bp", bp_attr);

    cJSON_AddNumberToObject(c_state, "v", state->value);

    if (state->turn)
    {
        cJSON_AddBoolToObject(c_state, "t", cJSON_True);
    }
    else
    {
        cJSON_AddBoolToObject(c_state, "t", cJSON_False);
    }

    cJSON_AddNumberToObject(c_state, "wc", state->white_castling);
    cJSON_AddNumberToObject(c_state, "bc", state->black_castling);

    cJSON* pp_attr = cJSON_CreateObject();
    cJSON_AddNumberToObject(pp_attr, "x", state->pawn_passant->posX);
    cJSON_AddNumberToObject(pp_attr, "y", state->pawn_passant->posY);
    cJSON_AddItemToObject(c_state, "pp", pp_attr);

    if (state->can_passant)
    {
        cJSON_AddItemToObject(c_state, "cp", cJSON_CreateTrue());
    }
    else
    {
        cJSON_AddItemToObject(c_state, "cp", cJSON_CreateFalse());
    }

    cJSON_AddNumberToObject(c_state, "f", state->fifty_rule);

    cJSON_AddItemToObject(object, "s", c_state);

    cJSON_AddNumberToObject(object, "l", level);

    char* buffer = cJSON_Print(object);

    cJSON_Delete(object);
    return buffer;
}

void json_to_data(char* buffer, State** state_ptr, int* level_ptr)
{
    *state_ptr = (State*)malloc(sizeof(State));

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
            (*state_ptr)->whitePieces = new_piece;
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
            (*state_ptr)->blackPieces = new_piece;
        }

        else
        {
            last->next = new_piece;
        }

        last = new_piece;
        c_temp = c_temp->next;
    }

    const cJSON* c_st_v_recv = cJSON_GetObjectItemCaseSensitive(c_state_recv, "v");
    (*state_ptr)->value = c_st_v_recv->valueint;

    const cJSON* c_st_t_recv = cJSON_GetObjectItemCaseSensitive(c_state_recv, "t");
    if (cJSON_IsTrue(c_st_t_recv))
    {
        (*state_ptr)->turn = true;
    }
    else
    {
        (*state_ptr)->turn = false;
    }

    const cJSON* c_st_wc_recv = cJSON_GetObjectItemCaseSensitive(c_state_recv, "wc");
    (*state_ptr)->white_castling = c_st_wc_recv->valueint;

    const cJSON* c_st_bc_recv = cJSON_GetObjectItemCaseSensitive(c_state_recv, "bc");
    (*state_ptr)->black_castling = c_st_bc_recv->valueint;

    const cJSON* c_st_pp_recv = cJSON_GetObjectItemCaseSensitive(c_state_recv, "pp");

    const cJSON* c_st_pp_x = cJSON_GetObjectItemCaseSensitive(c_st_pp_recv, "x");
    const cJSON* c_st_pp_y = cJSON_GetObjectItemCaseSensitive(c_st_pp_recv, "y");
    bool found = false;
    Piece* temp = (*state_ptr)->whitePieces;
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
        temp = (*state_ptr)->blackPieces;
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

    if (found)
    {
        (*state_ptr)->pawn_passant = temp;
    }
    else
    {
        (*state_ptr)->pawn_passant = temp_passant;
    }

    const cJSON* c_st_cp_recv = cJSON_GetObjectItemCaseSensitive(c_state_recv, "cp");
    if (cJSON_IsTrue(c_st_cp_recv))
    {
        (*state_ptr)->can_passant = true;
    }
    else
    {
        (*state_ptr)->can_passant = false;
    }

    const cJSON* c_st_f_recv = cJSON_GetObjectItemCaseSensitive(c_state_recv, "f");
    (*state_ptr)->fifty_rule = c_st_f_recv->valueint;

    const cJSON* c_level_recv = cJSON_GetObjectItemCaseSensitive(message, "l");
    *level_ptr = c_level_recv->valueint;

    cJSON_Delete(message);
}

char* resp_to_json(int res, int level)
{
    cJSON* object = cJSON_CreateObject();

    cJSON_AddNumberToObject(object, "r", res);
    cJSON_AddNumberToObject(object, "l", level);

    char* buffer = cJSON_Print(object);

    cJSON_Delete(object);
    return buffer;
}

void json_to_resp(char* buffer, int* res_ptr, int* level_ptr)
{
    cJSON* message = cJSON_Parse(buffer);

    const cJSON* res_recv = cJSON_GetObjectItemCaseSensitive(message, "r");
    *res_ptr = res_recv->valueint;

    const cJSON* level_recv = cJSON_GetObjectItemCaseSensitive(message, "l");
    *level_ptr = level_recv->valueint;

    cJSON_Delete(message);
}

int worker_available(bool mode)
{
    int res = -1;
    pthread_mutex_lock(&lock);

    if (mode)
    {
        res = BIT_CHECK(worker_status, 0) == 0 &&
            BIT_CHECK(worker_status, 1) == 0 &&
            BIT_CHECK(worker_status, 2) == 0 ? 1 : 0;
    }

    else
    {
        if (BIT_CHECK(worker_status, 0) == 0)
        {
            res = 0;
        }

        else if (BIT_CHECK(worker_status, 1) == 0)
        {
            res = 1;
        }

        else if (BIT_CHECK(worker_status, 2) == 0)
        {
            res = 2;
        }
    }

    pthread_mutex_unlock(&lock);
    return res;
}

void request_worker(State* state, int level, int pos)
{
    /** Call function to format the message into the char array buffer.
     *  [This is a test] 
     */
    // int temp = value, buff_size = 1; // Actual size
    // while (temp != 0)
    // {
    //     temp /= 10;
    //     buff_size++;
    // }
    // char* buffer = (char*)malloc(sizeof(char) * buff_size);
    // snprintf(buffer, buff_size, "%d", value);
    char* buffer = data_to_json(state, level);
    int buff_size = strlen(buffer);
    /** --------------------------------------------------------------- */

    pthread_mutex_lock(&lock);

    BIT_SET(worker_status, pos);
    MPI_Send(buffer, buff_size, MPI_BYTE, worker_ranks[pos], 1, MPI_COMM_WORLD);

    pthread_mutex_unlock(&lock);
    free(buffer);
}

/** TEST */
void test(State** state_ptr, int* level_ptr)
{
    *state_ptr = (State*)malloc(sizeof(State));

    Piece* p1 = (Piece*)malloc(sizeof(Piece));
    Piece* p2 = (Piece*)malloc(sizeof(Piece));
    Piece* p3 = (Piece*)malloc(sizeof(Piece));

    (*state_ptr)->whitePieces = p1;
    (*state_ptr)->blackPieces = p3;
    (*state_ptr)->value = -2;
    (*state_ptr)->turn = true;
    (*state_ptr)->white_castling = 0b111;
    (*state_ptr)->black_castling = 0b111;
    (*state_ptr)->pawn_passant = p2;
    (*state_ptr)->can_passant = false;
    (*state_ptr)->fifty_rule = 3;

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

    *level_ptr = 6;
}