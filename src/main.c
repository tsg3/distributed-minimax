#include <stdio.h>
#include <chess/chess.h>

int main () 
{
    Piece* king = create_piece('K', 0, 0);
    int last[2] = {-2, -2};
    int res[2];

    calcMove(king, king->posX, king->posY, last, res);

    printf("%d %d -> %d %d\n", last[0], last[1], res[0], res[1]);    
    return 0;
}