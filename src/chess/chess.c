#include "chess/chess.h"

int directions[8][2] = 
{
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, -1},
    {0, 1},
    {1, -1},
    {1, 0},
    {1, 1}
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

void calcMove(Piece *piece, int last_dir[], int last_pos[], int res[]) 
{
    switch (piece->type) 
    {
        case 'K':
            if (last_dir[0] == 1 && last_dir[1] == 1)
            {
                last_dir[0] = -2;
                last_dir[1] = -2;
                res[0] = piece->posX;
                res[1] = piece->posY;
            }

            else if (last_dir[0] == -2 && last_dir[1] == -2
                && piece->posX + directions[0][0] < 8
                && piece->posX + directions[0][0] >= 0
                && piece->posY + directions[0][1] < 8
                && piece->posY + directions[0][1] >= 0)
            {
                last_dir[0] = directions[0][0];
                last_dir[1] = directions[0][1];
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
                        if (piece->posX + directions[i][0] < 8
                            && piece->posX + directions[i][0] >= 0
                            && piece->posY + directions[i][1] < 8
                            && piece->posY + directions[i][1] >= 0) 
                        {
                            break;
                        }
                    }

                    if (last_dir[0] == directions[i][0] 
                        && last_dir[1] == directions[i][1]) 
                    {
                        reached = true;
                    }
                }
                last_dir[0] = directions[i][0];
                last_dir[1] = directions[i][1];
                res[0] = piece->posX + last_dir[0];
                res[1] = piece->posY + last_dir[1];
            }
            break;
    
        case 'Q':
            calcMove_aux(piece, 0, 1, last_dir, last_pos, res);
            break; 

        default:
            res[0] = -3;
            last_dir[0] = -3;
            res[1] = -3;
            last_dir[1] = -3;
            break;
    }
}

void calcMove_aux(Piece* piece, int start, int step, int last_dir[], int last_pos[], int res[]) 
{
    if (last_dir[0] == -2 && last_dir[1] == -2
        && piece->posX + directions[start][0] < 8
        && piece->posX + directions[start][0] >= 0
        && piece->posY + directions[start][1] < 8
        && piece->posY + directions[start][1] >= 0)
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

            while (posX_temp < 8 && posX_temp >= 0
                && posY_temp < 8 && posY_temp >= 0)
            {
                if (reached 
                    && !(piece->posX == posX_temp && piece->posY == posY_temp)) 
                {
                    found = true;
                    break;
                }

                else if (last_pos[0] == posX_temp
                    && last_pos[1] == posY_temp
                    && reached == false) 
                {
                    reached = true;
                }

                posX_temp += directions[i][0];
                posY_temp += directions[i][1];
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

// State

int get_value(State* state)
{
    return state->value;
}

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