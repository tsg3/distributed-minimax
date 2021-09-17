#include "chess/chess.h"

int king_valid_moves[8][2] = 
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

void calcMove(Piece *piece, int posX, int posY, int last[], int res[]) 
{
    switch (piece->type) 
    {
        case 'K':
            if (last[0] == 1 && last[1] == 1)
            {
                last[0] = -2;
                last[1] = -2;
                res[0] = posX;
                res[1] = posY;
            }

            else if (last[0] == -2 && last[1] == -2
                && posX + king_valid_moves[0][0] < 8
                && posX + king_valid_moves[0][0] >= 0
                && posY + king_valid_moves[0][1] < 8
                && posY + king_valid_moves[0][1] >= 0)
            {
                last[0] = king_valid_moves[0][0];
                last[1] = king_valid_moves[0][1];
                res[0] = posX + last[0];
                res[1] = posY + last[1];
            } 

            else 
            {
                int i;
                bool reached = false;
                if (last[0] == -2 && last[1] == -2) 
                {
                    reached = true;
                }

                for (i = 0; i < 8; i++) 
                {
                    if (reached == true) 
                    {
                        if (posX + king_valid_moves[i][0] < 8
                            && posX + king_valid_moves[i][0] >= 0
                            && posY + king_valid_moves[i][1] < 8
                            && posY + king_valid_moves[i][1] >= 0) 
                        {
                            break;
                        }
                    }

                    if (last[0] == king_valid_moves[i][0] 
                        && last[1] == king_valid_moves[i][1]) 
                    {
                        reached = true;
                    }
                }
                last[0] = king_valid_moves[i][0];
                last[1] = king_valid_moves[i][1];
                res[0] = posX + last[0];
                res[1] = posY + last[1];
            }
            break;
    
        default:
            res[0] = -3;
            last[1] = -3;
            res[0] = -3;
            last[1] = -3;
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