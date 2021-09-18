#include <stdio.h>
#include <chess/chess.h>

int main () 
{
    Piece* piece = create_piece('Q', 0, 0);
    int last_dir[2] = {-2, -2};
    int last_value[2] = {-2, -2};
    int res[2] = {-2, -2};

    while (piece->posX != res[0] || piece->posY != res[1])
    {
        calcMove(piece, last_dir, last_value, res);
        printf("%d %d -> %d %d\n", last_dir[0], last_dir[1], res[0], res[1]);
        last_value[0] = res[0];
        last_value[1] = res[1];
    }   
    return 0;
}