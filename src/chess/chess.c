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
}

// Piece

Piece* create_piece(char type, int x, int y)
{
    Piece* piece = (Piece*)malloc(sizeof(Piece));
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

void calcMove(State* state, Piece *piece, int last_dir[], int last_pos[], int res[], bool check_if_attacked) 
{
    switch (piece->type) 
    {
        case 'K':
            calcMove_single_aux(state, piece, (int*)directions, last_dir, res, check_if_attacked);
            break;
    
        case 'Q':
            calcMove_extended_aux(state, piece, 0, 1, last_dir, last_pos, res, check_if_attacked);
            break; 

        case 'B':
            calcMove_extended_aux(state, piece, 0, 2, last_dir, last_pos, res, check_if_attacked);
            break; 

        case 'N':
            calcMove_single_aux(state, piece, (int*)knight_movements, last_dir, res, check_if_attacked);
            break;

        case 'R':
            calcMove_extended_aux(state, piece, 1, 2, last_dir, last_pos, res, check_if_attacked);
            break; 

        case 'P':
            calcMove_pawn_aux(state, piece, last_dir, res, check_if_attacked);
            break; 

        default:
            res[0] = -3;
            last_dir[0] = -3;
            res[1] = -3;
            last_dir[1] = -3;
            break;
    }
}

void calcMove_extended_aux(State* state, Piece* piece, int start, int step, int last_dir[], int last_pos[], int res[], bool check_if_attacked) 
{
    if (last_dir[0] == -2 && last_dir[1] == -2
        && piece->posX + directions[start][0] < 8
        && piece->posX + directions[start][0] >= 0
        && piece->posY + directions[start][1] < 8
        && piece->posY + directions[start][1] >= 0
        && check_obstacle(state, 
            piece->posX + directions[start][0], piece->posY + directions[start][1]) != 2)
    {
        if (check_if_attacked)
        {
            if (!check_possible_attack(state, piece, 
                piece->posX + directions[start][0], piece->posY + directions[start][1]))
            {
                goto first_case;
            }
        }

        else
        {
        first_case:
            last_dir[0] = directions[start][0];
            last_dir[1] = directions[start][1];
            res[0] = piece->posX + last_dir[0];
            res[1] = piece->posY + last_dir[1];
            return;
        }
    } 

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
                if (check_if_attacked)
                {
                    if (!check_possible_attack(state, piece, posX_temp, posY_temp))
                    {
                        found = true;
                        break;
                    }
                }

                else
                {
                    found = true;
                    break;
                }
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

void calcMove_single_aux(State* state, Piece* piece, int* movements, int last_dir[], int res[], bool check_if_attacked)
{
    if (last_dir[0] == 0 && last_dir[1] == -1)
    {
        last_dir[0] = -2;
        last_dir[1] = -2;
        res[0] = piece->posX;
        res[1] = piece->posY;
        return;
    }

    if (last_dir[0] == -2 && last_dir[1] == -2
        && piece->posX + movements[0] < 8
        && piece->posX + movements[0] >= 0
        && piece->posY + movements[1] < 8
        && piece->posY + movements[1] >= 0
        && check_obstacle(state, piece->posX + movements[0], piece->posY + movements[1]) != 2)
    {
        if (check_if_attacked)
        {
            if (!check_possible_attack(state, piece, 
                piece->posX + movements[0], 
                piece->posY + movements[1]))
            {
                goto first_case;
            }
        }
        
        else
        {
        first_case:
            last_dir[0] = movements[0];
            last_dir[1] = movements[1];
            res[0] = piece->posX + last_dir[0];
            res[1] = piece->posY + last_dir[1];
            return;
        }
    } 

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
                if (check_if_attacked)
                {
                    if (!check_possible_attack(state, piece, 
                            piece->posX + movements[2 * i], 
                            piece->posY + movements[2 * i + 1]))
                    {
                        break;
                    }
                }
                
                else
                {
                    break;
                }
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

void calcMove_pawn_aux(State* state, Piece* piece, int last_dir[], int res[], bool check_if_attacked)
{
    int initial_pos = state->turn == true ? 1 : 6;
    int dir = state->turn == true ? 1 : -1;
    if (last_dir[0] == -2)
    {
        last_dir[0] = 0;
    }

    // Moves two spaces

    bool main_condition = last_dir[1] == -2 && piece->posY == initial_pos
        && check_obstacle(state, piece->posX, piece->posY + dir) == 0
        && check_obstacle(state, piece->posX, piece->posY + 2 * dir) == 0;

    if (main_condition && check_if_attacked) 
    {
        if (!check_possible_attack(state, piece, piece->posX, piece->posY + 2 * dir))
        {
            goto two_spaces;
        }
    }

    if (main_condition && !check_if_attacked)
    {
    two_spaces:
        last_dir[1] = 2;
        res[0] = piece->posX;
        res[1] = piece->posY + 2 * dir;
        return;
    }

    // Moves one space

    bool range = state->turn == true ? piece->posY < 7 : piece->posY > 0;
    bool y_pos_check = piece->posY + dir < 8 && piece->posY + dir >= 0;

    main_condition = range && (last_dir[1] == 2 || last_dir[1] == -2)
        && check_obstacle(state, piece->posX, piece->posY + dir) == 0;

    if (main_condition && check_if_attacked) 
    {
        if (!check_possible_attack(state, piece, piece->posX, piece->posY + dir))
        {
            goto one_space;
        }
    }

    if (main_condition && !check_if_attacked) 
    {
    one_space:
        last_dir[1] = dir;
        res[0] = piece->posX;
        res[1] = piece->posY + dir;
        return;
    }

    // Eats on +dir

    main_condition = piece->posX + dir < 8 && piece->posX + dir >= 0 && y_pos_check
        && last_dir[0] == 0
        && (check_obstacle(state, piece->posX + dir, piece->posY + dir) == 1
        || (check_obstacle(state, piece->posX + dir, piece->posY) == 1 
        && piece->posX + dir == state->pawn_passant->posX
        && piece->posY == state->pawn_passant->posY && state->can_passant));

    if (main_condition && check_if_attacked)
    {
        if (!check_possible_attack(state, piece, 
            piece->posX + dir, piece->posY + dir))
        {
            goto plus_dir;
        }
    }

    if (main_condition && !check_if_attacked)
    {
    plus_dir:
        last_dir[0] = dir;
        last_dir[1] = dir;
        res[0] = piece->posX + dir;
        res[1] = piece->posY + dir;
        return;
    }

    // Eats on -dir

    main_condition = piece->posX - dir < 8 && piece->posX - dir >= 0 && y_pos_check
        && (last_dir[0] == dir || last_dir[0] == 0)
        && (check_obstacle(state, piece->posX - dir, piece->posY + dir) == 1
        || (check_obstacle(state, piece->posX - dir, piece->posY) == 1 
        && piece->posX - dir == state->pawn_passant->posX
        && piece->posY == state->pawn_passant->posY && state->can_passant));

    if (main_condition && check_if_attacked)
    {
        if (!check_possible_attack(state, piece, 
            piece->posX - dir, piece->posY + dir))
        {
            goto minus_dir;
        }
    }

    if (main_condition && !check_if_attacked)
    {
    minus_dir:
        last_dir[0] = -dir;
        last_dir[1] = dir;
        res[0] = piece->posX - dir;
        res[1] = piece->posY + dir;
        return;
    }

    // else

    last_dir[0] = -2;
    last_dir[1] = -2;
    res[0] = piece->posX;
    res[1] = piece->posY;
}

void move_piece(State* state, Piece* piece, int x, int y, char promotion)
{
    Piece* temp = state->turn ? state->blackPieces : state->whitePieces;
    bool conflict = false;

    while (temp != NULL)
    {
        if (state->can_passant && piece->type == 'P' && x != piece->posX
            && temp->posX == x && temp->posY == piece->posY
            && temp == state->pawn_passant)
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

    state->pawn_passant = temp_passant;
    state->can_passant = false;

    if (piece->type == 'P' && x == 0 && y == 2)
    {
        state->pawn_passant = piece;
        state->can_passant = true;
    }

    if ((promotion == 'Q' || promotion == 'B' || promotion == 'N' || promotion == 'R')
        && piece->type == 'P'
        && ((y == 7 && state->turn) 
        || (y == 0 && !state->turn)))
    {
        piece->type = promotion;
    }

    if (conflict || piece->type == 'P')
    {
        state->fifty_rule = 0;
    }

    else if (!state->turn == conf->turn)
    {
        state->fifty_rule++;
    }

    move(piece, x, y);
}

void check_castling(State* state, int* res)
{
    int castling = state->turn ? state->white_castling : state->black_castling;
    int y = state->turn ? 0 : 7;

    if ((castling & 0b101) == 0 && *res > 2
        && !calc_value(state, false)
        && check_obstacle(state, 5, y) == 0
        && check_obstacle(state, 6, y) == 0
        && !check_castle_interrupt(state, 2))
    {
        *res = 2;
        return;
    }
    
    if ((castling & 0b110) == 0 && *res > 1
        && !calc_value(state, false)
        && check_obstacle(state, 1, y) == 0
        && check_obstacle(state, 2, y) == 0
        && check_obstacle(state, 3, y) == 0
        && !check_castle_interrupt(state, 1))
    {
        *res = 1;
        return;
    }

    if (*res > 0)
    {
        *res = 0;
        return;
    }
}

bool check_castle_interrupt(State* state, int type)
{
    int intermediate_cells[2][2];
    int final_rook_cell[2];
    int y = state->turn ? 0 : 7;
    int x_rook;

    if (type == 2)
    {
        intermediate_cells[0][0] = 5;
        intermediate_cells[0][1] = y;
        intermediate_cells[1][0] = 6;
        intermediate_cells[1][1] = y;

        final_rook_cell[0] = 5;
        final_rook_cell[1] = y;

        x_rook = 7;
    }

    else if (type == 1)
    {
        intermediate_cells[0][0] = 3;
        intermediate_cells[0][1] = y;
        intermediate_cells[1][0] = 2;
        intermediate_cells[1][1] = y;

        final_rook_cell[0] = 3;
        final_rook_cell[1] = y;

        x_rook = 0;
    }

    else
    {
        return true;
    }

    // Look if the next cell is attacked

    Piece* associated_piece = state->turn 
        ? state->whitePieces : state->blackPieces;

    while (associated_piece != NULL)
    {
        if (associated_piece->posX == 4
            && associated_piece->posY == y
            && associated_piece->type == 'K')
        {
            break;
        }

        associated_piece = associated_piece->next;
    }

    if (associated_piece == NULL)
    {
        return true;
    }

    if (check_possible_attack(state, associated_piece, 
        intermediate_cells[0][0], intermediate_cells[0][1]))
    {
        return true;
    }

    // Look if final cell is attacked

    State* temp_state = create_copy(state);
    associated_piece = temp_state->turn 
        ? temp_state->whitePieces : temp_state->blackPieces;
    Piece *king, *rook;

    while (associated_piece != NULL)
    {
        if (associated_piece->posX == 4
            && associated_piece->posY == y
            && associated_piece->type == 'K')
        {
            king = associated_piece;
        }

        else if (associated_piece->posX == x_rook
            && associated_piece->posY == y
            && associated_piece->type == 'R')
        {
            rook = associated_piece;
        }

        associated_piece = associated_piece->next;
    }

    if (king == NULL || rook == NULL)
    {
        delete_state(temp_state);
        return true;
    }

    move_piece(temp_state, king, intermediate_cells[1][0], intermediate_cells[1][1], '\0');
    move_piece(temp_state, rook, final_rook_cell[0], final_rook_cell[1], '\0');

    if (calc_value(temp_state, false))
    {
        delete_state(temp_state);
        return true;
    }

    delete_state(temp_state);
    return false;
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

State* create_state(bool turn, int white_castling, int black_castling, Piece* pawn_passant, bool can_passant)
{
    State* state = (State*)malloc(sizeof(State));
    state->whitePieces = NULL;
    state->blackPieces = NULL;
    state->value = -2;
    state->turn = turn;
    state->white_castling = white_castling;
    state->black_castling = black_castling;
    state->pawn_passant = pawn_passant != NULL ? pawn_passant : temp_passant;
    state->can_passant = can_passant;
    state->fifty_rule = 0;

    return state;
}

State* create_copy(State* old_state)
{
    State* new_state = (State*)malloc(sizeof(State));
    new_state->value = old_state->value;
    new_state->turn = old_state->turn;
    new_state->white_castling = old_state->white_castling;
    new_state->black_castling = old_state->black_castling;
    new_state->pawn_passant = NULL;
    new_state->can_passant = old_state->can_passant;
    new_state->fifty_rule = old_state->fifty_rule;
    
    Piece* temp = old_state->whitePieces;
    Piece* last_added = NULL;
    while (temp != NULL)
    {
        Piece* new_piece = create_piece(temp->type, temp->posX, temp->posY);

        if (new_piece->type == old_state->pawn_passant->type
            && new_piece->posX == old_state->pawn_passant->posX
            && new_piece->posY == old_state->pawn_passant->posY)
        {
            new_state->pawn_passant = new_piece;
        }

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

        if (new_piece->type == old_state->pawn_passant->type
            && new_piece->posX == old_state->pawn_passant->posX
            && new_piece->posY == old_state->pawn_passant->posY)
        {
            new_state->pawn_passant = new_piece;
        }

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

    if (new_state->pawn_passant == NULL)
    {
        new_state->pawn_passant = temp_passant;
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
    Piece* temp;
    while (state->whitePieces != NULL) 
    {
        temp = state->whitePieces;
        state->whitePieces = temp->next;
        free(temp);
    }

    while (state->blackPieces != NULL) 
    {
        temp = state->blackPieces;
        state->blackPieces = temp->next;
        free(temp);
    }

    free(state);
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
            calcMove(state, temp, last_dir, last_pos, res, false);

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

    // Fifty-move rule
    if (state->fifty_rule == 50)
    {
        state->value = 0;
        return true;
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
            calcMove(state, temp, last_dir, last_pos, res, false);

            if (res[0] == -3
                || (temp->posX == res[0] && temp->posY == res[1])) {
                break;
            }

            if (!check_possible_attack(state, temp, res[0], res[1]))
            {
                possibility = true;
                break;
            }

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

    else if (state->turn == conf->turn)
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

bool check_possible_attack(State* state, Piece* piece, int x, int y)
{
    State* temp_state = create_copy(state);
    Piece* associated_piece = temp_state->turn 
        ? temp_state->whitePieces : temp_state->blackPieces;

    while (associated_piece != NULL)
    {
        if (associated_piece->posX == piece->posX
            && associated_piece->posY == piece->posY
            && associated_piece->type == piece->type)
        {
            break;
        }

        associated_piece = associated_piece->next;
    }

    if (associated_piece == NULL)
    {
        delete_state(temp_state);
        return true;
    }

    move_piece(temp_state, associated_piece, x, y, '\0');

    if (calc_value(temp_state, false))
    {
        delete_state(temp_state);
        return true;
    }

    delete_state(temp_state);
    return false;
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