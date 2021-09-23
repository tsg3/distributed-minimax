#include <stdio.h>
#include <chess/chess.h>

int main () 
{
    init();
    
    player = true;

    State* state = create_state(true, 1, 2);
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
    state->whitePieces = piece_w_1;
    state->blackPieces = piece_b_1;
    pawn_passant = piece_w_1;

    int last_dir[2] = {-2, -2};
    int last_pos[2] = {-2, -2};
    int res[2] = {-2, -2};
    
    while (piece_w_2->posX != res[0] || piece_w_2->posY != res[1])
    {
        calcMove(state, piece_w_2, last_dir, last_pos, res);

        printf("%d %d -> (%d, %d)\n", last_dir[0], last_dir[1], res[0], res[1]);

        if (res[0] == -3) {
            break;
        }

        last_pos[0] = res[0];
        last_pos[1] = res[1];
    }
    
    delete_state(state);
    free(temp_passant);
    return 0;
}