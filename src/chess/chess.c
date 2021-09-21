#include "chess/chess.h"

int directions[8][2] = 
{
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, 1},
    {1, 1},
    {1, 0},
    {1, -1},
    {0, -1}
};

int knight_movements[8][2] = 
{
    {1, 2},
    {2, 1},
    {2, -1},
    {1, -2},
    {-1, -2},
    {-2, -1},
    {-2, 1},
    {-1, 2}
};

// Piece

Piece* create_piece(char type, int x, int y)
{
    Piece* piece = (Piece*)malloc(sizeof(piece));
    piece->type = type;
    piece->posX = x;
    piece->posY = y;
    piece->next = NULL;

    return piece;
}

void move(Piece *piece, int x, int y) 
{
    piece->posX = x;
    piece->posY = y;
}

void calcMove(State* state, Piece *piece, int last_dir[], int last_pos[], int res[]) 
{
    switch (piece->type) 
    {
        case 'K':
            calcMove_single_aux(piece, (int*)directions, last_dir, res);
            break;
    
        case 'Q':
            calcMove_extended_aux(state, piece, 0, 1, last_dir, last_pos, res);
            break; 

        case 'B':
            calcMove_extended_aux(state, piece, 0, 2, last_dir, last_pos, res);
            break; 

        case 'N':
            calcMove_single_aux(piece, (int*)knight_movements, last_dir, res);
            break;

        case 'R':
            calcMove_extended_aux(state, piece, 1, 2, last_dir, last_pos, res);
            break; 

        case 'P':
            calcMove_pawn_aux(state, piece, last_dir, res);
            break; 

        default:
            res[0] = -3;
            last_dir[0] = -3;
            res[1] = -3;
            last_dir[1] = -3;
            break;
    }
}

void calcMove_extended_aux(State* state, Piece* piece, int start, int step, int last_dir[], int last_pos[], int res[]) 
{
    if (last_dir[0] == -2 && last_dir[1] == -2
        && piece->posX + directions[start][0] < 8
        && piece->posX + directions[start][0] >= 0
        && piece->posY + directions[start][1] < 8
        && piece->posY + directions[start][1] >= 0
        && check_obstacle(state, 
            piece->posX + directions[start][0], piece->posY + directions[start][1]) != 2)
    {
        last_dir[0] = directions[start][0];
        last_dir[1] = directions[start][1];
        res[0] = piece->posX + last_dir[0];
        res[1] = piece->posY + last_dir[1];
    } 

    else 
    {
        int i, posX_temp, posY_temp;
        bool reached = false;
        bool found = false;
        int last_obstacle_check;
        if (last_dir[0] == -2 && last_dir[1] == -2) 
        {
            reached = true;
        }

        for (i = start; i < 8; i += step) 
        {
            if ((last_dir[0] != directions[i][0]
                || last_dir[1] != directions[i][1])
                && !reached) 
            {
                continue;
            }

            posX_temp = piece->posX;
            posY_temp = piece->posY;
            last_obstacle_check = 0;

            while (posX_temp < 8 && posX_temp >= 0
                && posY_temp < 8 && posY_temp >= 0)
            {
                if (reached 
                    && !(piece->posX == posX_temp && piece->posY == posY_temp)
                    && last_obstacle_check != 2) 
                {
                    found = true;
                    break;
                }

                if (last_obstacle_check == 2)
                {
                    break;
                }

                if (last_pos[0] == posX_temp
                    && last_pos[1] == posY_temp
                    && reached == false) 
                {
                    reached = true;
                }

                posX_temp += directions[i][0];
                posY_temp += directions[i][1];

                if (last_obstacle_check == 1)
                {
                    last_obstacle_check = 2;
                }
                
                else
                {
                    last_obstacle_check = check_obstacle(state, posX_temp, posY_temp);
                }
            }
            
            if (found) 
            {
                break;
            }
        }
        if (!found) 
        {
            last_dir[0] = -2;
            last_dir[1] = -2;
            res[0] = piece->posX;
            res[1] = piece->posY;
        }

        else
        {
            last_dir[0] = directions[i][0];
            last_dir[1] = directions[i][1];
            res[0] = posX_temp;
            res[1] = posY_temp;
        }
    }
}

void calcMove_single_aux(Piece* piece, int* movements, int last_dir[], int res[])
{
    if (last_dir[0] == 0 && last_dir[1] == -1)
    {
        last_dir[0] = -2;
        last_dir[1] = -2;
        res[0] = piece->posX;
        res[1] = piece->posY;
    }

    else if (last_dir[0] == -2 && last_dir[1] == -2
        && piece->posX + movements[0] < 8
        && piece->posX + movements[0] >= 0
        && piece->posY + movements[1] < 8
        && piece->posY + movements[1] >= 0)
    {
        last_dir[0] = movements[0];
        last_dir[1] = movements[1];
        res[0] = piece->posX + last_dir[0];
        res[1] = piece->posY + last_dir[1];
    } 

    else 
    {
        int i;
        bool reached = false;
        if (last_dir[0] == -2 && last_dir[1] == -2) 
        {
            reached = true;
        }

        for (i = 0; i < 8; i++) 
        {
            if (reached) 
            {
                if (piece->posX + movements[2 * i] < 8
                    && piece->posX + movements[2 * i] >= 0
                    && piece->posY + movements[2 * i + 1] < 8
                    && piece->posY + movements[2 * i + 1] >= 0) 
                {
                    break;
                }
            }

            if (last_dir[0] == movements[2 * i] 
                && last_dir[1] == movements[2 * i + 1]) 
            {
                reached = true;
            }
        }
        last_dir[0] = movements[2 * i];
        last_dir[1] = movements[2 * i + 1];
        res[0] = piece->posX + last_dir[0];
        res[1] = piece->posY + last_dir[1];
    }
}

void calcMove_pawn_aux(State* state, Piece* piece, int last_dir[], int res[])
{
    int initial_pos = state->turn == true ? 1 : 6;
    int dir = state->turn == true ? 1 : -1;
    if (last_dir[0] == -2)
    {
        last_dir[0] = 0;
    }

    if (last_dir[1] == -2 && piece->posY == initial_pos
        && check_obstacle(state, piece->posX, piece->posY + dir) == 0
        && check_obstacle(state, piece->posX, piece->posY + 2 * dir) == 0) 
    {
        last_dir[1] = 2;
        res[0] = piece->posX;
        res[1] = piece->posY + 2 * dir;
    }

    else
    {
        bool range = state->turn == true ? piece->posY < 7 : piece->posY > 0;
        bool y_pos_check = piece->posY + dir < 8 && piece->posY + dir >= 0;

        if (range && (last_dir[1] == 2 || last_dir[1] == -2)
            && check_obstacle(state, piece->posX, piece->posY + dir) == 0) 
        {
            last_dir[1] = dir;
            res[0] = piece->posX;
            res[1] = piece->posY + dir;
        }

        else if (piece->posX + dir < 8 && piece->posX + dir >= 0 && y_pos_check
            && check_obstacle(state, piece->posX + dir, piece->posY + dir) == 1
            && last_dir[0] == 0)
        {
            last_dir[0] = dir;
            last_dir[1] = dir;
            res[0] = piece->posX + dir;
            res[1] = piece->posY + dir;
        }

        else if (piece->posX - dir < 8 && piece->posX - dir >= 0 && y_pos_check
            && check_obstacle(state, piece->posX - dir, piece->posY + dir) == 1
            && (last_dir[0] == dir || last_dir[0] == 0))
        {
            last_dir[0] = -dir;
            last_dir[1] = dir;
            res[0] = piece->posX - dir;
            res[1] = piece->posY + dir;
        }

        else
        {
            last_dir[0] = -2;
            last_dir[1] = -2;
            res[0] = piece->posX;
            res[1] = piece->posY;
        }
    }
}

State* create_state(bool turn)
{
    State* state = (State*)malloc(sizeof(State));
    state->whitePieces = NULL;
    state->blackPieces = NULL;
    state->value = -2;
    state->turn = turn;

    return state;
}

// State

int get_value(State* state)
{
    return state->value;
}

// Not finished
void calc_value(State* state, bool player)
{
    // Set players
    Piece *king, *currentPlayer, *currentOpponent;
    if (state->turn == true)
    {
        currentPlayer = state->whitePieces;
        currentOpponent = state->blackPieces;
    } 
    else
    {
        currentPlayer = state->blackPieces;
        currentOpponent = state->whitePieces;
    }

    // Look for player's king
    Piece* temp = currentPlayer;
    while (temp->type != 'K')
    {
        temp = temp->next;
    }

    // Look if attacked
    Piece* temp2 = currentOpponent;
    while (temp != NULL)
    {
        temp = temp->next;
    }
}

int check_obstacle(State* state, int x, int y)
{
    int obstacle = 1;
    Piece* pieces_temp = state->turn ? state->whitePieces : state->blackPieces;

    while (pieces_temp != NULL)
    {
        if (pieces_temp->posX == x && pieces_temp->posY == y)
        {
            obstacle = 2;
            break;
        }

        pieces_temp = pieces_temp->next;
    }

    if(obstacle == 2)
    {
        return 2;
    }

    obstacle = 0;
    pieces_temp = state->turn ? state->blackPieces : state->whitePieces;

    while (pieces_temp != NULL)
    {
        if (pieces_temp->posX == x && pieces_temp->posY == y)
        {
            obstacle = 1;
            break;
        }

        pieces_temp = pieces_temp->next;
    }

    return obstacle;
}