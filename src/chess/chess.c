#include "chess/chess.h"

int king_queen_directions[8][2] = 
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

void calcMove(Piece *piece, int posX, int posY, int last_dir[], int last_pos[], int res[]) 
{
    switch (piece->type) 
    {
        case 'K':
            if (last_dir[0] == 1 && last_dir[1] == 1)
            {
                last_dir[0] = -2;
                last_dir[1] = -2;
                res[0] = posX;
                res[1] = posY;
            }

            else if (last_dir[0] == -2 && last_dir[1] == -2
                && posX + king_queen_directions[0][0] < 8
                && posX + king_queen_directions[0][0] >= 0
                && posY + king_queen_directions[0][1] < 8
                && posY + king_queen_directions[0][1] >= 0)
            {
                last_dir[0] = king_queen_directions[0][0];
                last_dir[1] = king_queen_directions[0][1];
                res[0] = posX + last_dir[0];
                res[1] = posY + last_dir[1];
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
                        if (posX + king_queen_directions[i][0] < 8
                            && posX + king_queen_directions[i][0] >= 0
                            && posY + king_queen_directions[i][1] < 8
                            && posY + king_queen_directions[i][1] >= 0) 
                        {
                            break;
                        }
                    }

                    if (last_dir[0] == king_queen_directions[i][0] 
                        && last_dir[1] == king_queen_directions[i][1]) 
                    {
                        reached = true;
                    }
                }
                last_dir[0] = king_queen_directions[i][0];
                last_dir[1] = king_queen_directions[i][1];
                res[0] = posX + last_dir[0];
                res[1] = posY + last_dir[1];
            }
            break;
    
        case 'Q':
            if (last_dir[0] == -2 && last_dir[1] == -2
                && posX + king_queen_directions[0][0] < 8
                && posX + king_queen_directions[0][0] >= 0
                && posY + king_queen_directions[0][1] < 8
                && posY + king_queen_directions[0][1] >= 0)
            {
                last_dir[0] = king_queen_directions[0][0];
                last_dir[1] = king_queen_directions[0][1];
                res[0] = posX + last_dir[0];
                res[1] = posY + last_dir[1];
            } 

            else 
            {
                int i, posX_temp, posY_temp;
                bool reached = false;
                bool found = false;

                for (i = 0; i < 8; i++) 
                {
                    if ((last_dir[0] != king_queen_directions[i][0]
                        || last_dir[1] != king_queen_directions[i][1])
                        && !reached) 
                    {
                        continue;
                    }

                    posX_temp = posX;
                    posY_temp = posY;

                    while (posX_temp < 8 && posX_temp >= 0
                        && posY_temp < 8 && posY_temp >= 0)
                    {
                        if (reached && !(posX == posX_temp && posY == posY_temp)) 
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

                        posX_temp += king_queen_directions[i][0];
                        posY_temp += king_queen_directions[i][1];
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
                    res[0] = posX;
                    res[1] = posY;
                }

                else
                {
                    last_dir[0] = king_queen_directions[i][0];
                    last_dir[1] = king_queen_directions[i][1];
                    res[0] = posX_temp;
                    res[1] = posY_temp;
                }
            }
            break; 

        default:
            res[0] = -3;
            last_dir[0] = -3;
            res[1] = -3;
            last_dir[1] = -3;
            break;
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