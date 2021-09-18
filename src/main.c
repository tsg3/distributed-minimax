#include <stdio.h>
#include <chess/chess.h>

int main () 
{
    Piece* piece = create_piece('N', 6, 6);
    int last_dir[2] = {-2, -2};
    int last_pos[2] = {-2, -2};
    int res[2] = {-2, -2};

    while (piece->posX != res[0] || piece->posY != res[1])
    {
        calcMove(piece, last_dir, last_pos, res);
        printf("%d %d -> %d %d\n", last_dir[0], last_dir[1], res[0], res[1]);

        if (res[0] == -3) {
            break;
        }

        last_pos[0] = res[0];
        last_pos[1] = res[1];
    }   
    return 0;
}