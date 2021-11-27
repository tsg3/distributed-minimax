/**
 * @file test_ids.c
 * @author Esteban Campos Granados (este0111@hotmail.com)
 * @brief Testing of 'configure_id'.
 * @version 0.1
 * @date 2021-10-29
 */

#include <stdio.h>
#include <HWController.h>

/** @fn main
 * @brief Main execution method.
 * 
 * Tests 'configure_id' method from the Hardware Controller library.
 * 
 * @return int 
 */

int main()
{
    printf("ID #0 ...\n");
    configure_id('0');
    sleep(1);

    printf("ID #1 ...\n");
    configure_id('1');
    sleep(1);

    printf("ID #2 ...\n");
    configure_id('2');
    sleep(1);

    printf("ID #3 ...\n");
    configure_id('3');
    sleep(1);

    printf("ID #0 ...\n");
    configure_id('0');
    sleep(1);

    return 0;
}