/**
 * @file test_states.c
 * @author Esteban Campos Granados (este0111@hotmail.com)
 * @brief Testing of 'configure_state'. 
 * @version 0.1
 * @date 2021-10-29
 */

#include <stdio.h>
#include <HWController.h>

/** @fn main 
 * @brief Main execution method.
 * 
 * Tests the 'configure_state' method from the Hardware Controller library.
 * 
 * @return int 
 */

int main()
{
    printf("State #0 ...\n");
    configure_state('0');
    sleep(1);

    printf("State #1 ...\n");
    configure_state('1');
    sleep(1);

    printf("State #2 ...\n");
    configure_state('2');
    sleep(1);

    printf("State #3 ...\n");
    configure_state('3');
    sleep(1);

    printf("State #0 ...\n");
    configure_state('0');
    sleep(1);

    return 0;
}