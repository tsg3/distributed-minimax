/**
 * @file test_checking.c
 * @author Esteban Campos Granados (este0111@hotmail.com)
 * @brief Testing of waiting and checking methods. 
 * @version 0.1
 * @date 2021-10-29
 */

#include <stdio.h>
#include <HWController.h>

/** @fn main
 * @brief Main execution method.
 * 
 * Tests waiting and checking methods from the Hardware Controller library for the 
 * kernel module.
 * 
 * @return int 
 */

int main()
{
    wait_continue();
    printf("¿Shutdown? = %d\n", check_shutdown());
    return 0;
}