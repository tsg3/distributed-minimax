#include "chess/chess.h"

void init()
{
    directions[0][0] = -1;
    directions[0][1] = -1;
    directions[1][0] = -1;
    directions[1][1] = 0;
    directions[2][0] = -1;
    directions[2][1] = 1;
    directions[3][0] = 0;
    directions[3][1] = 1;
    directions[4][0] = 1;
    directions[4][1] = 1;
    directions[5][0] = 1;
    directions[5][1] = 0;
    directions[6][0] = 1;
    directions[6][1] = -1;
    directions[7][0] = 0;
    directions[7][1] = -1;
    
    knight_movements[0][0] = 1;
    knight_movements[0][1] = 2;
    knight_movements[1][0] = 2;
    knight_movements[1][1] = 1;
    knight_movements[2][0] = 2;
    knight_movements[2][1] = -1;
    knight_movements[3][0] = 1;
    knight_movements[3][1] = -2;
    knight_movements[4][0] = -1;
    knight_movements[4][1] = -2;
    knight_movements[5][0] = -2;
    knight_movements[5][1] = -1;
    knight_movements[6][0] = -2;
    knight_movements[6][1] = 1;
    knight_movements[7][0] = -1;
    knight_movements[7][1] = 2;

    temp_passant = create_piece('X', -3, -4);
    pawn_passant = temp_passant;
    can_passant = false;
}

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
            calcMove_single_aux(state, piece, (int*)directions, last_dir, res);
            break;
    
        case 'Q':
            calcMove_extended_aux(state, piece, 0, 1, last_dir, last_pos, res);
            break; 

        case 'B':
            calcMove_extended_aux(state, piece, 0, 2, last_dir, last_pos, res);
            break; 

        case 'N':
            calcMove_single_aux(state, piece, (int*)knight_movements, last_dir, res);
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

void calcMove_single_aux(State* state, Piece* piece, int* movements, int last_dir[], int res[])
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
        && piece->posY + movements[1] >= 0
        && check_obstacle(state, piece->posX + movements[0], piece->posY + movements[1]) != 2)
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
                    && piece->posY + movements[2 * i + 1] >= 0
                    && check_obstacle(state, piece->posX + movements[2 * i], 
                        piece->posY + movements[2 * i + 1]) != 2) 
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
        if (i == 8) 
        {
            last_dir[0] = -2;
            last_dir[1] = -2;
            res[0] = piece->posX;
            res[1] = piece->posY;
        }

        else
        {
            last_dir[0] = movements[2 * i];
            last_dir[1] = movements[2 * i + 1];
            res[0] = piece->posX + last_dir[0];
            res[1] = piece->posY + last_dir[1];
        }
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
        pawn_passant = piece;
        can_passant = true;
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
            && last_dir[0] == 0
            && (check_obstacle(state, piece->posX + dir, piece->posY + dir) == 1
            || (check_obstacle(state, piece->posX + dir, piece->posY) == 1 
            && piece->posX + dir == pawn_passant->posX
            && piece->posY == pawn_passant->posY && can_passant)))
        {
            last_dir[0] = dir;
            last_dir[1] = dir;
            res[0] = piece->posX + dir;
            res[1] = piece->posY + dir;
        }

        else if (piece->posX - dir < 8 && piece->posX - dir >= 0 && y_pos_check
            && (last_dir[0] == dir || last_dir[0] == 0)
            && (check_obstacle(state, piece->posX - dir, piece->posY + dir) == 1
            || (check_obstacle(state, piece->posX - dir, piece->posY) == 1 
            && piece->posX - dir == pawn_passant->posX
            && piece->posY == pawn_passant->posY && can_passant)))
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

void move_piece(State* state, Piece* piece, int x, int y)
{
    Piece* temp = state->turn ? state->blackPieces : state->whitePieces;
    bool conflict = false;

    while (temp != NULL)
    {
        if (can_passant && piece->type == 'P' && x != piece->posX
            && temp->posX == x && temp->posY == piece->posY
            && temp == pawn_passant)
        {
            conflict = true;
            break;
        }

        if (temp->posX == x && temp->posY == y)
        {
            conflict = true;
            break;
        }

        temp = temp->next;
    }

    if (conflict)
    {
        delete_piece(state, temp, !state->turn);
        pawn_passant = temp_passant;
    }

    if (piece->type == 'K')
    {
        if (state->turn)
        {
            state->white_castling = state->white_castling | 0b100;
        }

        else
        {
            state->black_castling = state->black_castling | 0b100;
        }
    }

    else if (piece->type == 'R' && piece->posX == 0
        && ((piece->posY == 0 && state->turn)
        || (piece->posY == 7 && !state->turn)))
    {
        if (state->turn)
        {
            state->white_castling = state->white_castling | 0b010;
        }

        else
        {
            state->black_castling = state->black_castling | 0b010;
        }
    }

    else if (piece->type == 'R' && piece->posX == 7
        && ((piece->posY == 0 && state->turn)
        || (piece->posY == 7 && !state->turn)))
    {
        if (state->turn)
        {
            state->white_castling = state->white_castling | 0b001;
        }

        else
        {
            state->black_castling = state->black_castling | 0b001;
        }
    }

    move(piece, x, y);
}

void check_castling(State* state, int* res)
{
    int castling = state->turn ? state->white_castling : state->black_castling;

    if ((castling & 0b101) == 0 && *res > 2)
    {
        *res = 2;
    }
    
    else if ((castling & 0b110) == 0 && *res > 1)
    {
        *res = 1;
    }

    else
    {
        *res = 0;
    }
}

void castle(State* state, int castle_type, Piece** king, Piece** rook, int* final_king_pos, int* final_rook_pos)
{
    int rook_x;
    int rook_y = state->turn ? 0 : 7;
    final_king_pos[1] = rook_y;
    final_rook_pos[1] = rook_y;
    
    if (castle_type == 1)
    {
        rook_x = 0;
        final_king_pos[0] = 2;
        final_rook_pos[0] = 3;
    }

    else if (castle_type == 2)
    {
        rook_x = 7;
        final_king_pos[0] = 6;
        final_rook_pos[0] = 5;
    }

    else
    {
        return;
    }
    
    Piece* temp = state->turn ? state->whitePieces : state->blackPieces;

    while (temp != NULL)
    {
        if (temp->type == 'K')
        {
            *king = temp;
        }

        else if (temp->type == 'R' && temp->posX == rook_x && temp->posY == rook_y)
        {
            *rook = temp;
        }

        temp = temp->next;
    }
}

State* create_state(bool turn, int white_castling, int black_castling)
{
    State* state = (State*)malloc(sizeof(State));
    state->whitePieces = NULL;
    state->blackPieces = NULL;
    state->value = -2;
    state->turn = turn;
    state->white_castling = white_castling;
    state->black_castling = black_castling;

    return state;
}

State* create_copy(State* old_state)
{
    State* new_state = (State*)malloc(sizeof(State));
    new_state->value = old_state->value;
    new_state->turn = old_state->turn;
    
    Piece* temp = old_state->whitePieces;
    Piece* last_added = NULL;
    while (temp != NULL)
    {
        Piece* new_piece = create_piece(temp->type, temp->posX, temp->posY);

        if (last_added == NULL)
        {
            new_state->whitePieces = new_piece;
            last_added = new_state->whitePieces;
        }

        else
        {
            last_added->next = new_piece;
            last_added = last_added->next;
        }

        temp = temp->next;
    }

    temp = old_state->blackPieces;
    last_added = NULL;
    while (temp != NULL)
    {
        Piece* new_piece = create_piece(temp->type, temp->posX, temp->posY);

        if (last_added == NULL)
        {
            new_state->blackPieces = new_piece;
            last_added = new_state->blackPieces;
        }

        else
        {
            last_added->next = new_piece;
            last_added = last_added->next;
        }

        temp = temp->next;
    }

    return new_state;
}

void delete_piece(State* state, Piece* piece, bool player)
{
    Piece* prev = player ? state->whitePieces : state->blackPieces;
    Piece* temp;

    if (prev != NULL)
    {
        temp = prev->next;
    }

    if (prev == piece)
    {
        if (player)
        {
            state->whitePieces = piece->next;
        }

        else
        {
            state->blackPieces = piece->next;
        }

        free(piece);
    }

    else
    {
        bool found = false;

        while(temp != NULL)
        {
            if (temp == piece)
            {
                found = true;
                break;
            }

            prev = temp;
            temp = temp->next;
        }

        if (found)
        {
            prev->next = temp->next;
            free(temp);
        }
    }
}

void delete_state(State* state)
{
    Piece* temp = state->whitePieces;
    while (temp != NULL) 
    {
        delete_piece(state, temp, true);
        temp = state->whitePieces;
    }

    temp = state->blackPieces;
    while (temp != NULL) 
    {
        delete_piece(state, temp, false);
        temp = state->blackPieces;
    }
}

// State

int get_value(State* state)
{
    return state->value;
}

// Not finished
bool calc_value(State* state, bool check_posibilities)
{
    // Set players
    Piece *players_king, *currentPlayer, *currentOpponent;

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
    players_king = currentPlayer;
    while (players_king->type != 'K')
    {
        players_king = players_king->next;
    }

    // Look if attacked
    Piece* temp = currentOpponent;
    int last_dir[2], last_pos[2], res[2];
    bool attacked = false;
    state->turn = !state->turn; // just to check

    while (temp != NULL)
    {
        last_dir[0] = -2;
        last_dir[1] = -2;
        last_pos[0] = -2;
        last_pos[1] = -2;
        res[0] = -2;
        res[1] = -2;
        
        while (temp->posX != res[0] || temp->posY != res[1])
        {
            calcMove(state, temp, last_dir, last_pos, res);

            if (players_king->posX == res[0] && players_king->posY == res[1])
            {
                attacked = true;
                break;
            }

            if (res[0] == -3) {
                break;
            }

            last_pos[0] = res[0];
            last_pos[1] = res[1];
        }

        if (attacked)
        {
            break;
        }

        temp = temp->next;
    }

    state->turn = !state->turn;

    if (!check_posibilities)
    {
        return attacked;
    }

    bool possibility = false;
    temp = currentPlayer;
    State* temp_state;
    Piece* associated_piece; 

    while (temp != NULL)
    {
        last_dir[0] = -2;
        last_dir[1] = -2;
        last_pos[0] = -2;
        last_pos[1] = -2;
        res[0] = -2;
        res[1] = -2;
        
        while (true)
        {
            calcMove(state, temp, last_dir, last_pos, res);

            if (res[0] == -3
                || (temp->posX == res[0] && temp->posY == res[1])) {
                break;
            }

            temp_state = create_copy(state);
            associated_piece = temp_state->turn 
                ? temp_state->whitePieces : temp_state->blackPieces;

            while (associated_piece != NULL)
            {
                if (associated_piece->posX == temp->posX
                    && associated_piece->posY == temp->posY)
                {
                    break;
                }

                associated_piece = associated_piece->next;
            }

            move_piece(temp_state, associated_piece, res[0], res[1]);

            if (!calc_value(temp_state, false))
            {
                possibility = true;
                break;
            }

            delete_state(temp_state);

            last_pos[0] = res[0];
            last_pos[1] = res[1];
        }

        if (possibility)
        {
            break;
        }

        temp = temp->next;
    }

    if (possibility)
    {
        return false;
    }

    else if (!attacked && !possibility)
    {
        state->value = 0;
        return true;
    }

    else if (state->turn == player)
    {
        state->value = -1;
        return true;
    }

    else
    {
        state->value = 1;
        return true;
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