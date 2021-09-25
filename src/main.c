#include <stdio.h>
#include <minimax/minimax.h>

int main () 
{
    init_chess_models(false);

    State* state = create_state(false, 7, 7, NULL, false);
    Piece* piece_w_1 = create_piece('K', 4, 0);
    Piece* piece_w_2 = create_piece('R', 5, 0);
    Piece* piece_w_3 = create_piece('N', 5, 1);
    Piece* piece_b_1 = create_piece('K', 2, 5);
    Piece* piece_b_2 = create_piece('B', 3, 5);
    Piece* piece_b_3 = create_piece('B', 7, 4);
    state->whitePieces = piece_w_1;
    piece_w_1->next = piece_w_2;
    piece_w_2->next =  piece_w_3;
    state->blackPieces = piece_b_1;
    piece_b_1->next = piece_b_2;
    piece_b_2->next =  piece_b_3;

    printf("%d\n", calc_level(state, 0));

    clean_passant_temp();
    delete_state(state);
    return 0;
}