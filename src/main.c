#include <stdio.h>
#include <configuration.h>

int main () 
{
    init_chess_models();
    conf = configure_minimax();

    if (conf == NULL)
    {
        clean_passant_temp();
        return -1;
    }

    print_configuration(conf);
    printf("%d\n", calc_level(conf->initial_state, 0));

    clean_passant_temp();
    delete_configuration(conf);
    return 0;
}