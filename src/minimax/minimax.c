#include <minimax/minimax.h>

void init_chess_models(bool main_player)
{
    init();

    player = main_player;
}

void clean_passant_temp()
{
    free(temp_passant);
}

int minmax(int in_1, int in_2, int level)
{
    if (level % 2 == 0)
    {
        return in_1 > in_2 ? in_1 : in_2;
    }

    else
    {
        return in_1 < in_2 ? in_1 : in_2;
    }
}

// minimax algorithm

int calc_level(State* state, int level)
{
    int resp = level % 2 == 0 ? INT_MIN : INT_MAX;

    int last_dir[2], last_pos[2], res[2], possibility;
    Piece* temp = state->turn ? state->whitePieces : state->blackPieces;

    printf(" > Beginning of level %d\n", level);

    while (temp != NULL)
    {
        last_dir[0] = -2;
        last_dir[1] = -2;
        last_pos[0] = -2;
        last_pos[1] = -2;
        res[0] = -2;
        res[1] = -2;

        printf("-> %c\n", temp->type);

        while (true)
        {
            calcMove(state, temp, last_dir, last_pos, res, true);

            if (res[0] == -3 || (temp->posX == res[0] && temp->posY == res[1])) 
            {
                break;
            }

            printf("%d %d -> (%d, %d) [%c]\n", last_dir[0], last_dir[1], res[0], res[1], temp->type);

            if (temp->type == 'P' 
                && ((res[1] == 7 && state->turn) 
                || (res[1] == 0 && !state->turn)))
            {
                char promotions[4] = {'Q', 'B', 'N', 'R'};

                for (int i = 0; i < 4; i++)
                {
                    possibility = check_possible_final(state, temp, res[0], res[1], 
                        promotions[i]);

                    if(possibility == -2)
                    {
                        possibility = calc_level_aux(state, level, temp, res[0], res[1], 
                            promotions[i]);
                    }

                    if (possibility == -3)
                    {
                        continue;
                    }
                    printf(" > %d\n", possibility);

                    resp = minmax(resp, possibility, level);
                }
            }

            else
            {
                possibility = check_possible_final(state, temp, res[0], res[1], '\0');

                if(possibility == -2)
                {
                    possibility = calc_level_aux(state, level, temp, res[0], res[1], '\0');
                }

                if (possibility == -3)
                {
                    goto back_propagation;
                }

                resp = minmax(resp, possibility, level);
            }

back_propagation:
            last_pos[0] = res[0];
            last_pos[1] = res[1];
        }

        temp = temp->next;
    }

    if (resp == INT_MAX || resp == INT_MIN)
    {
        calc_value(state, true);
        resp = state->value;
    }

    return resp;
}

int calc_level_aux(State* state, int level, Piece* piece, int x, int y, char promotion)
{
    if (level < 2) // replacement of the 50 move rule
    {
        State* copy = create_copy(state);
        Piece* associated_piece = copy->turn 
            ? copy->whitePieces : copy->blackPieces;
        
        while (associated_piece != NULL)
        {
            if (associated_piece->type == piece->type
                && associated_piece->posX == piece->posX
                && associated_piece->posY == piece->posY)
            {
                break;
            }

            associated_piece = associated_piece->next;
        }

        move_piece(copy, associated_piece, x, y, promotion);
        copy->turn = !copy->turn;
        int possibility = calc_level(copy, level + 1);
        delete_state(copy);
        return possibility;
    }

    return level % 2 == 0 ? INT_MIN : INT_MAX;
}

int check_possible_final(State* state, Piece* piece, int x, int y, char promotion)
{
    State* possible_state = create_copy(state);
    Piece* associated_piece = 
        possible_state->turn ? possible_state->whitePieces : possible_state->blackPieces;
    
    while (associated_piece != NULL)
    {
        if (associated_piece->type == piece->type
            && associated_piece->posX == piece->posX
            && associated_piece->posY == piece->posY)
        {
            break;
        }

        associated_piece = associated_piece->next;
    }

    if (associated_piece == NULL)
    {
        return -3;
    }

    move_piece(possible_state, associated_piece, x, y, promotion);

    possible_state->turn = !possible_state->turn;

    if (calc_value(possible_state, true))
    {
        possible_state->turn = !possible_state->turn;
        int resp = possible_state->value;
        delete_state(possible_state);
        return resp;
    }
    
    delete_state(possible_state);
    return -2;
}