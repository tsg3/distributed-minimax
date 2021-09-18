#include <stdio.h>
#include <chess/chess.h>

int main () 
{
    Piece* king = create_piece('Q', 2, 3);
    int last_dir[2] = {-2, -2};
    int last_value[2] = {-1, -1};
    int res[2] = {-1, -1};

    while (king->posX != res[0] || king->posY != res[1])
    {
        calcMove(king, king->posX, king->posY, last_dir, last_value, res);
        printf("%d %d -> %d %d\n", last_dir[0], last_dir[1], res[0], res[1]);
        last_value[0] = res[0];
        last_value[1] = res[1];
    }   
    return 0;
}