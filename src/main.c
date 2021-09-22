#include <stdio.h>
#include <chess/chess.h>

int main () 
{
    player = true;

    State* state = create_state(true);
    Piece* piece_w_1 = create_piece('K', 0, 0);
    Piece* piece_w_2 = create_piece('Q', 0, 1);
    piece_w_1->next = piece_w_2;
    Piece* piece_w_3 = create_piece('P', 1, 1);
    piece_w_2->next = piece_w_3;
    Piece* piece_w_4 = create_piece('P', 6, 7);
    piece_w_3->next = piece_w_4;
    Piece* piece_w_5 = create_piece('P', 0, 2);
    piece_w_4->next = piece_w_5;
    Piece* piece_b_1 = create_piece('K', 0, 6);
    Piece* piece_b_2 = create_piece('P', 1, 2);
    piece_b_1->next = piece_b_2;
    Piece* piece_b_3 = create_piece('P', 2, 2);
    piece_b_2->next = piece_b_3;
    Piece* piece_b_4 = create_piece('P', 3, 6);
    piece_b_3->next = piece_b_4;
    Piece* piece_b_5 = create_piece('N', 1, 4);
    piece_b_4->next = piece_b_5;
    Piece* piece_b_6 = create_piece('N', 1, 0);
    piece_b_5->next = piece_b_6;
    // Piece* piece_b_7 = create_piece('N', 2, 1);
    // piece_b_6->next = piece_b_7;
    state->whitePieces = piece_w_1;
    state->blackPieces = piece_b_1;

    int last_dir[2] = {-2, -2};
    int last_pos[2] = {-2, -2};
    int res[2] = {-2, -2};
    
    delete_state(state);
    return 0;
}