#include <stdio.h>
#include <minimax/minimax.h>

int main () 
{
    bool initial_player = true;

    init_chess_models(initial_player);

    State* state = create_state(initial_player, 7, 7, NULL, false);
    Piece* piece_w_1 = create_piece('K', 3, 0);
    Piece* piece_w_2 = create_piece('R', 1, 0);
    Piece* piece_w_3 = create_piece('R', 6, 4);
    Piece* piece_w_4 = create_piece('B', 3, 3);
    Piece* piece_w_5 = create_piece('P', 0, 1);
    Piece* piece_w_6 = create_piece('P', 1, 6);
    Piece* piece_w_7 = create_piece('P', 2, 5);
    Piece* piece_b_1 = create_piece('K', 0, 5);
    state->whitePieces = piece_w_1;
    piece_w_1->next = piece_w_2;
    piece_w_2->next =  piece_w_3;
    piece_w_3->next =  piece_w_4;
    piece_w_4->next =  piece_w_5;
    piece_w_5->next =  piece_w_6;
    piece_w_6->next =  piece_w_7;
    state->blackPieces = piece_b_1;

    printf("%d\n", calc_level(state, 0));

    clean_passant_temp();
    delete_state(state);
    return 0;
}