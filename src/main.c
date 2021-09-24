#include <stdio.h>
#include <chess/chess.h>

int main () 
{
    init();
    
    player = true;

    State* state = create_state(true, 0, 0, NULL, false);
    Piece* piece_w_1 = create_piece('K', 0, 0);
    Piece* piece_w_2 = create_piece('Q', 0, 1);
    piece_w_1->next = piece_w_2;
    Piece* piece_w_3 = create_piece('P', 1, 1);
    piece_w_2->next = piece_w_3;
    Piece* piece_w_4 = create_piece('P', 6, 7);
    piece_w_3->next = piece_w_4;
    Piece* piece_w_5 = create_piece('P', 0, 2);
    piece_w_4->next = piece_w_5;
    Piece* piece_w_6 = create_piece('P', 5, 4);
    piece_w_5->next = piece_w_6;
    Piece* piece_b_1 = create_piece('K', 0, 6);
    Piece* piece_b_2 = create_piece('P', 1, 2);
    piece_b_1->next = piece_b_2;
    Piece* piece_b_3 = create_piece('B', 2, 2);
    piece_b_2->next = piece_b_3;
    Piece* piece_b_4 = create_piece('P', 3, 6);
    piece_b_3->next = piece_b_4;
    Piece* piece_b_5 = create_piece('N', 1, 4);
    piece_b_4->next = piece_b_5;
    Piece* piece_b_6 = create_piece('N', 1, 0);
    piece_b_5->next = piece_b_6;
    Piece* piece_b_7 = create_piece('P', 4, 4);
    piece_b_6->next = piece_b_7;
    state->whitePieces = piece_w_1;
    state->blackPieces = piece_b_1;
    state->pawn_passant = piece_b_7;
    state->can_passant = true;

    int last_dir[2] = {-2, -2};
    int last_pos[2] = {-2, -2};
    int res[2] = {-2, -2};

    Piece* temp = state->whitePieces;
    while (temp != NULL)
    {
        last_dir[0] = -2;
        last_dir[1] = -2;
        last_pos[0] = -2;
        last_pos[1] = -2;
        res[0] = -2;
        res[1] = -2;

        printf("-> %c\n", temp->type);

        while (temp->posX != res[0] || temp->posY != res[1])
        {
            calcMove(state, temp, last_dir, last_pos, res, true);

            printf("%d %d -> (%d, %d)\n", last_dir[0], last_dir[1], res[0], res[1]);

            if (res[0] == -3) {
                break;
            }

            last_pos[0] = res[0];
            last_pos[1] = res[1];
        }

        temp = temp->next;
    }
    
    delete_state(state);
    free(temp_passant);
    return 0;
}